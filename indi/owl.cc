#include "owl.h"

#include "config.h"
#include <connectionplugins/connectionserial.h>

using namespace owl;

// We declare an auto pointer to MyCustomDriver.
static std::unique_ptr<Owl> owl_driver = std::make_unique<Owl>();

Owl::Owl() : GI(this)
{
    setVersion(OWL_DRIVER_VERSION_MAJOR, OWL_DRIVER_VERSION_MINOR);

    SetCapability(FOCUSER_CAN_REL_MOVE | FOCUSER_HAS_VARIABLE_SPEED);
    setSupportedConnections(CONNECTION_SERIAL);
}

const char *Owl::getDefaultName()
{
    return "Owl";
}

bool Owl::saveConfigItems(FILE * fp)
{
    bool res = true;
    res &= GI::saveConfigItems(fp);
    res &= INDI::Focuser::saveConfigItems(fp);
    return res;
}

void Owl::ISGetProperties(const char *dev)
{
    if (dev != nullptr && strcmp(dev, getDeviceName()) != 0)
    {
        return;
    }

    INDI::Focuser::ISGetProperties(dev);
}

bool Owl::ISNewNumber(const char *dev, const char *name, double values[], char *names[], int n)
{
    if (dev != nullptr and strcmp(dev, getDeviceName()) == 0)
    {
        if (GI::processNumber(dev, name, values, names, n))
        {
            return true;
        }
    }

    // Let INDI::Focuser handle any other number properties
    return INDI::Focuser::ISNewNumber(dev, name, values, names, n);
}

bool Owl::ISNewSwitch(const char *dev, const char *name, ISState *states, char *names[], int n)
{
    if (dev != nullptr and strcmp(dev, getDeviceName()) == 0)
    {
        if (GI::processSwitch(dev, name, states, names, n))
        {
            if (SystemTimeUpdateSP.isNameMatch(name))
                saveConfig(true, SystemTimeUpdateSP.getName());
            return true;
        }
    }

    // Let INDI::Focuser handle any other number properties
    return INDI::Focuser::ISNewSwitch(dev, name, states, names, n);
}

bool Owl::initProperties()
{
    INDI::Focuser::initProperties();
    GI::initProperties("Main Control");
    setDriverInterface(GPS_INTERFACE | FOCUSER_INTERFACE);

    IUFillNumber(&TemperatureN[0], "TEMPERATURE", "Celsius", "%6.2f", -50., 70., 0., 0.);
    IUFillNumberVector(&TemperatureNP, TemperatureN, 1, getDeviceName(), "FOCUS_TEMPERATURE", "Temperature",
                       MAIN_CONTROL_TAB, IP_RW, 0, IPS_IDLE);

    FocusSpeedN[0].min   = 1;
    FocusSpeedN[0].max   = 6;
    FocusSpeedN[0].step  = 1;
    FocusSpeedN[0].value = 2;
    SetFocuserSpeed(FocusSpeedN[0].value);

    // GPS Tab
    LocationNP.setGroupName(GPS_TAB);
    TimeTP.setGroupName(GPS_TAB);
    RefreshSP.setGroupName(GPS_TAB);
    PeriodNP.setGroupName(GPS_TAB);
    SystemTimeUpdateSP.setGroupName(GPS_TAB);

    GPSNP[GPSFix].fill("GPSFix",       "GPS Fix", "%.f", 0, 3, 1, 0);
    GPSNP[GPSTime].fill("GPSTime",     "GPS Time", "%.f", 0, 1e9, 10, 0);
    GPSNP[GPSDate].fill("GPSDate",     "GPS Date", "%.f", 0, 1e9, 10, 0);
    GPSNP[UTCOffset].fill("UTCOffset", "UTC Offset", "%.2f", -12, 12, 1, 0);
    GPSNP[Latitude].fill("Latitude",   "Latitude", "%.2f", -90, 90, 10, 0);
    GPSNP[Longitude].fill("Longitude", "Longitude", "%.2f", -180, 180, 10, 0);
    GPSNP[Altitude].fill("Altitude",   "Altitude", "%.2f", -1e9, 1e9, 10, 0);
    GPSNP[SatelliteNumber].fill("SatelliteNumber",  "Sat. #", "%.f", 0, 30, 10, 0);
    GPSNP.fill(getDeviceName(), "GPS", "GPS", GPS_TAB, IP_RO, 60, IPS_IDLE);


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
    GI::updateProperties();


    if (isConnected())
    {
        defineProperty(&TemperatureNP);
        defineProperty(GPSNP);
    }
    else
    {
        deleteProperty(TemperatureNP.name);
        deleteProperty(GPSNP);
    }

    return true;
}

void Owl::TimerHit()
{
    IDLog("TimerHit\n");
    if (not isConnected())
    {
        IDLog("Not connected\n");
        SetTimer(getCurrentPollingPeriod());
        return;
    }
    IDLog("Connected\n");

    TemperatureN[0].value = 25.0 + rand() % 10 / 10.0;
    IDSetNumber(&TemperatureNP, nullptr);

    owl::write_refresh(frame_);

    IDLog("read loop\n");
    while (frame_.read())
    {
        owl::Header const* header = reinterpret_cast<owl::Header const*>(frame_.data());
        uint8_t const* payload = frame_.data() + sizeof(owl::Header);

        switch (header->type)
        {
            case owl::Type::TRACE:
            {
                LOGF_INFO("Trace: %.*s", header->size, payload);
                break;
            }
            case owl::Type::FEEDBACK:
            {
                IDLog("Frame: Feedback\n");
                owl::Feedback const* feedback = reinterpret_cast<owl::Feedback const*>(payload);
                //LOGF_INFO("Feedback! %d %d", feedback->focus_state, feedback->focus_pos);
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
            case owl::Type::GPS:
            {
                IDLog("Frame: GPS\n");
                gps_ = *reinterpret_cast<owl::GPS const*>(payload);
                break;
            }
            default:
            {

            }
        }
        frame_.reset();
    }

    SetTimer(getCurrentPollingPeriod());
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
    owl::write_control(frame_, control);
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
    frame_.setPort(PortFD);
    LOG_INFO("handshake!");
    return true;
}

IPState Owl::updateGPS()
{
    // convert gps time/date to time_t (utc)
    struct tm utc;

    int remainder = gps_.time;
    utc.tm_sec= remainder % 100;
    remainder /= 100;
    utc.tm_min = remainder % 100;
    remainder /= 100;
    utc.tm_hour = remainder % 100;

    remainder = gps_.date;
    utc.tm_year = (remainder % 100) + 100;
    remainder /= 100;
    utc.tm_mon = (remainder % 100) - 1;
    remainder /= 100;
    utc.tm_mday = remainder % 100;


    time_t raw_time = mktime(&utc);
    m_GPSTime = raw_time;

    static char ts[32] = {0};
    strftime(ts, sizeof(ts), "%Y-%m-%dT%H:%M:%S", &utc);
    TimeTP[0].setText(ts);

    struct tm* local = localtime(&raw_time);
    snprintf(ts, sizeof(ts), "%4.2f", (local->tm_gmtoff / 3600.0));
    TimeTP[1].setText(ts);

    if ((gps_.time != 0) and (gps_.date != 0))
    {
        TimeTP.setState(IPS_OK);
    }
    else
    {
        TimeTP.setState(IPS_BUSY);
    }

    LocationNP[LOCATION_LATITUDE].value  = gps_.latitude;
    LocationNP[LOCATION_LONGITUDE].value = gps_.longitude;
    LocationNP[LOCATION_ELEVATION].value = gps_.altitude;

    GPSNP[GPSFix].setValue(gps_.fix_quality);
    GPSNP[GPSTime].setValue(gps_.time);
    GPSNP[GPSDate].setValue(gps_.date);
    GPSNP[UTCOffset].setValue(local->tm_gmtoff / 3600);
    GPSNP[Latitude].setValue(gps_.latitude);
    GPSNP[Longitude].setValue(gps_.longitude);
    GPSNP[Altitude].setValue(gps_.altitude);
    GPSNP[SatelliteNumber].setValue(gps_.satellites);

    if (gps_.fix_quality == 0)
    {
        GPSNP.setState(IPS_BUSY);
        LocationNP.setState(IPS_BUSY);
    }
    else
    {
        GPSNP.setState(IPS_OK);
        LocationNP.setState(IPS_OK);
    }
    GPSNP.apply();

    IDLog("---- INFO ----\n");
    IDLog("time:      %d\n",  gps_.time);
    IDLog("date:      %d\n",  gps_.date);
    IDLog("fix:   %d\n",      gps_.fix_quality);
    IDLog("sats : %d\n",      gps_.satellites);
    IDLog("DOP:   %.3f\n",    gps_.dop);

    if (gps_.fix_quality == 0)
    {
        return IPS_BUSY;
    }
    return IPS_OK;
}
