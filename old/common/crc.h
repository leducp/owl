#ifndef OWL_COMMON_CRC
#define OWL_COMMON_CRC

#include <cstdint>

namespace owl
{

    uint8_t crc8(uint8_t const* buffer, int len, uint8_t polynom, uint8_t initial_value = 0x00)
    {
        uint8_t crc = initial_value;
        
        while (len--)
        {
            crc = crc ^ *buffer++; 
        
            for (int i = 0; i < 8; ++i)
            {
                if (crc & 0x80)
                {
                    crc = (crc << 1) ^ polynom;
                }
                else
                {
                    crc = (crc << 1);
                }
            }
        }
        
        return crc;
    }
}

#endif
