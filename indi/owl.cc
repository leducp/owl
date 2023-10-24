#include "owl.h"

#include "config.h"
#include <connectionplugins/connectionserial.h>

// We declare an auto pointer to MyCustomDriver.
static std::unique_ptr<Owl> owl_driver = std::make_unique<Owl>();

Owl::Owl()
{
    setVersion(OWL_DRIVER_VERSION_MAJOR, OWL_DRIVER_VERSION_MINOR);

    SetCapability(FOCUSER_CAN_REL_MOVE | FOCUSER_HAS_VARIABLE_SPEED);
    setSupportedConnections(CONNECTION_SERIAL);
}

const char *Owl::getDefaultName()
{
    return "Owl";
}

void Owl::ISGetProperties(const char *dev)
{
    if (dev != nullptr && strcmp(dev, getDeviceName()) != 0)
    {
        return;
    }

    INDI::Focuser::ISGetProperties(dev);
}

bool Owl::initProperties()
{
    INDI::Focuser::initProperties();

    IUFillNumber(&TemperatureN[0], "TEMPERATURE", "Celsius", "%6.2f", -50., 70., 0., 0.);
    IUFillNumberVector(&TemperatureNP, TemperatureN, 1, getDeviceName(), "FOCUS_TEMPERATURE", "Temperature",
                       MAIN_CONTROL_TAB, IP_RW, 0, IPS_IDLE);

    FocusSpeedN[0].min   = 1;
    FocusSpeedN[0].max   = 6;
    FocusSpeedN[0].step  = 1;
    FocusSpeedN[0].value = 2;
    SetFocuserSpeed(FocusSpeedN[0].value);

    // Poll every 500ms
    setDefaultPollingPeriod(500);

    // Set default baud rate to 115200
    serialConnection->setDefaultBaudRate(Connection::Serial::B_115200);

    // For simulation
    //addAuxControls();

    return true;
}

bool Owl::updateProperties()
{
    INDI::Focuser::updateProperties();

    if (isConnected())
    {
        defineProperty(&TemperatureNP);
    }
    else
    {
        deleteProperty(TemperatureNP.name);
    }

    return true;
}

void Owl::TimerHit()
{
    if (!isConnected())
    {
        return;
    }

    TemperatureN[0].value = 25.0 + rand() % 10 / 10.0; 
    IDSetNumber(&TemperatureNP, nullptr);

    refresh();

    owl::Header header;
    char buffer[256];
    while (readFrame(header, buffer))
    {
        switch (header.type)
        {
            case owl::Type::TRACE:
            {
                buffer[255] = 0;
                LOGF_INFO("Trace: %.*s", header.size, buffer);
                break;
            }
            case owl::Type::FEEDBACK:
            {
                owl::Feedback* feedback = reinterpret_cast<owl::Feedback*>(buffer);
                LOGF_INFO("Feedback! %d %d", feedback->focus_state, feedback->focus_pos);
                FocusRelPosN[0].value = feedback->focus_pos;
                switch (feedback->focus_state)
                {
                    case owl::State::BUSY:  { FocusRelPosNP.s = IPS_BUSY;   break; }
                    case owl::State::FAULT: { FocusRelPosNP.s = IPS_ALERT;  break; }
                    case owl::State::READY: { FocusRelPosNP.s = IPS_OK;     break; }
                }
                IDSetNumber(&FocusRelPosNP, nullptr);
                
                break;
            }
            default:
            {

            }
        }
    }

    SetTimer(getCurrentPollingPeriod());
}

int Owl::readData(char* buffer, int size, int timeout, bool abort)
{
    int r;
    int count = 0;

    while (count < size)
    {
        int result = tty_read(PortFD, buffer + count, size - count, timeout, &r);
        count += r;
        if (abort and count == 0)
        {
            return 0;
        }

        if (timeout > 0 and result != TTY_OK)
        {
            return 0;
        }
    }

    return count;
}

int Owl::readFrame(owl::Header& header, char* buffer)
{
    int res = readData((char*)&header, sizeof(owl::Header), 0, true);
    if (res == 0)
    {
        LOG_INFO("nothing");
        return 0;
    }
    
    LOGF_INFO("read %d", header.size);
    return readData(buffer, header.size, 1, false);
}

void Owl::writeFrame(owl::Header const& header, char const* buffer)
{
    int w;
    int count;
    char* pos = (char*)&header;

    count = 0;
    while (count < sizeof(owl::Header))
    {
        tty_write(PortFD, pos + count, sizeof(owl::Header) - count, &w);
        count += w;
    }

    count = 0;
    while (count < header.size)
    {
        tty_write(PortFD, buffer + count, header.size - count, &w);
        count += w;
    }
}

void Owl::refresh()
{
    owl::Header header;
    header.type = owl::Type::REFRESH;
    header.size = 0;
    writeFrame(header, nullptr);
}

bool Owl::ISNewNumber(const char *dev, const char *name, double values[], char *names[], int n)
{
    // Let INDI::Focuser handle any other number properties
    return INDI::Focuser::ISNewNumber(dev, name, values, names, n);
}

IPState Owl::MoveRelFocuser(FocusDirection dir, uint32_t ticks)
{
    if (dir == FocusDirection::FOCUS_INWARD)
    {
        control.steps = -ticks;
    }
    else
    {
        control.steps = ticks;
    }

    owl::Header header;
    header.type = owl::Type::CONTROL;
    header.size = sizeof(owl::Control);

    writeFrame(header, (char*)&control);
    FocusRelPosNP.s = IPS_BUSY;
    return IPS_BUSY;
}

bool Owl::SetFocuserSpeed(int speed)
{
    control.speed = speed * 1000;
    return true;
}

bool Owl::Handshake()
{
    LOG_INFO("handshake!");
    return true;
}
