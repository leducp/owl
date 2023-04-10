#include "SHT31.h"
#include "common/crc.h"

#include <stdio.h>

namespace owl
{
    SHT31::SHT31(I2C& bus)
        : bus_{bus}
    { }

    hresult SHT31::fetch()
    {
        uint8_t txdata[] = {0x24, 0x00}; // hr measurement, no stretching
        hresult rc = bus_.write(address_, txdata, 2);
        if (rc != error::S_OK)
        {
            return rc;
        }

        sleep(16ms); // Up to 15.5ms to do a high repeatability measurement

        uint8_t rxdata[6];
        rc = bus_.read(address_, rxdata, 6);
        if (rc != error::S_OK)
        {
            return rc;
        }

        int raw_temp = (rxdata[0] << 8) | rxdata[1];
        uint8_t const& crc_temp = rxdata[2];
        uint8_t computed_crc_temp = crc8(rxdata + 0, 2, 0x31, 0xff);
        if (crc_temp != computed_crc_temp)
        {
            return error::E_BAD_CHECKSUM;
        }

        int raw_hr   = (rxdata[3] << 8) | rxdata[4];
        uint8_t const& crc_hr = rxdata[5];
        uint8_t computed_crc_hr = crc8(rxdata + 3, 2, 0x31, 0xff);
        if (crc_hr != computed_crc_hr)
        {
            return error::E_BAD_CHECKSUM;
        }

        temperature_ = -45 * precision_ + ((175 * raw_temp * precision_) / 65535);
        humidity_ = (100 * raw_hr * precision_) / 65535;

        return error::S_OK;
    }
}
