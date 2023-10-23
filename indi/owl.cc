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
    FocusSpeedN[0].max   = 8;
    FocusSpeedN[0].step  = 1;
    FocusSpeedN[0].value = 2;

    // Poll every 500ms
    setDefaultPollingPeriod(500);
    setCurrentPollingPeriod(500);

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

    SetTimer(getCurrentPollingPeriod());
}

bool Owl::ISNewNumber(const char *dev, const char *name, double values[], char *names[], int n)
{
    // Let INDI::Focuser handle any other number properties
    return INDI::Focuser::ISNewNumber(dev, name, values, names, n);
}

IPState Owl::MoveRelFocuser(FocusDirection dir, uint32_t ticks)
{
    INDI_UNUSED(dir);
    INDI_UNUSED(ticks);
    return IPS_OK;
}

bool Owl::SetFocuserSpeed(int speed)
{
    INDI_UNUSED(speed);
    return true;
}

bool Owl::Handshake()
{
    LOG_INFO("handshake!");
    return true;
}
