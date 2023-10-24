#include <cstdint>
#include <cstring>

#include "Hash.h"

namespace owl
{
    class _crc16_x25 final : public Hash
    {
    public:
        void init() override;
        void update(char input) override;
        void finalize(void* out) override;
        int output_size() override;

    private:
        uint16_t crc_;
    };

    void _crc16_x25::init()
    {
        crc_ = 0xffff;
    }

    int _crc16_x25::output_size()
    {
        return sizeof(crc_);
    }

    void _crc16_x25::update(char input)
    {
        crc_ ^= input;
        for (int i = 0; i < 8; ++i)
        {
            if (crc_ & 1)
            {
                crc_ = (crc_ >> 1) ^ 0x8408;
            }
            else
            {
                crc_ = crc_ >> 1;
            }
        }
    }

    void _crc16_x25::finalize(void* out)
    {
        crc_ = ~crc_;
        std::memcpy(out, &crc_, sizeof(crc_));
        init();
    }


    std::unique_ptr<Hash> createHash(std::string const& function)
    {
        if (function == "crc16/x25")
        {
            return std::make_unique<_crc16_x25>();
        }

        return nullptr;
    }
}