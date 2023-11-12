#ifndef OWL_LIB_COBD_H
#define OWL_LIB_COBD_H

#include <cstdint>
#include <vector>

namespace owl
{
    constexpr uint8_t DELIMITER = 0x00;

    std::vector<uint8_t> encode(std::vector<uint8_t> const& buffer);
    std::vector<uint8_t> decode(std::vector<uint8_t> const& buffer);
    /*
    class COBD
    {
    public:
        COBD(void* buffer);
        ~COBD() = default;



    private:
        uint8_t* buffer_;

    };
    */


}

#endif
