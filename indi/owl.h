#ifndef OWN_INDI_OWL_H
#define OWN_INDI_OWL_H

#include <libindi/indifocuser.h>
#include <libindi/indigps.h>
#include <libindi/indipropertynumber.h>

#include "protocol.h"
#include "FrameIndi.h"

namespace Connection
{
    class Serial;
}

class Owl : public INDI::Focuser, public INDI::GPSInterface
{
public:
    Owl();
    virtual ~Owl() = default;

    void ISGetProperties(const char *dev) override;
    bool ISNewNumber(const char *dev, const char *name, double values[], char *names[], int n) override;
    bool ISNewSwitch(const char *dev, const char *name, ISState *states, char *names[], int n) override;

protected:
    const char* getDefaultName() override;
    bool saveConfigItems(FILE *fp) override;

    bool initProperties() override;
    bool updateProperties() override;

    //bool Connect() override;
    //bool Disconnect() override;

    void TimerHit() override;

    bool Handshake() override;

    IPState MoveRelFocuser(FocusDirection dir, uint32_t ticks) override;
    bool SetFocuserSpeed(int speed) override;

    // GPS Override
    IPState updateGPS() override;

private:

    // Temperature in celcius degrees
    INumberVectorProperty TemperatureNP;
    INumber TemperatureN[1];

    /// GPS Report
    enum
    {
        GPSFix,
        GPSTime,
        GPSDate,
        UTCOffset,
        Latitude,
        Longitude,
        Altitude,
        SatelliteNumber
    };
    INDI::PropertyNumber GPSNP {8};
    static constexpr char const* GPS_TAB {"GPS"};

    owl::Control control;
    owl::GPS gps_;

    owl::FrameIndi frame_{-1};
};

#endif
