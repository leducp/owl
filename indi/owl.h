#ifndef OWN_INDI_OWL_H
#define OWN_INDI_OWL_H

#include <libindi/indifocuser.h>
#include <libindi/indigps.h>
#include <libindi/indipropertynumber.h>

namespace Connection
{
    class Serial;
}

class Owl : public INDI::Focuser
{
public:
    Owl();
    virtual ~Owl() = default;

    const char* getDefaultName() override;
    void ISGetProperties(const char *dev) override;
    bool ISNewNumber(const char *dev, const char *name, double values[], char *names[], int n) override;

protected:
    bool initProperties() override;
    bool updateProperties() override;

    //bool Connect() override;
    //bool Disconnect() override;

    void TimerHit() override;

    bool Handshake() override;

    IPState MoveRelFocuser(FocusDirection dir, uint32_t ticks) override;
    bool SetFocuserSpeed(int speed) override;

private:
    // Temperature in celcius degrees
    INumberVectorProperty TemperatureNP;
    INumber TemperatureN[1];
};

#endif
