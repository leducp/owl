#ifndef OWL_LIB_HASH_H
#define OWL_LIB_HASH_H

#include <vector>
#include <memory>
#include <cstdint>

namespace owl
{
    class Hash
    {
    public:
        Hash() = default;
        virtual ~Hash() = default;

        virtual void init() = 0;
        virtual void update(uint8_t input) = 0;
        virtual void finalize(void* out) = 0;
        virtual int output_size() = 0;

        void update(std::vector<uint8_t> const& input)
        {
            for (auto const& c : input)
            {
                update(c);
            }
        }

        void update(void const* data, int size)
        {
            uint8_t const* pos = reinterpret_cast<uint8_t const*>(data);
            for (int i = 0; i < size; ++i)
            {
                update(pos[i]);
            }
        }

        template<typename T>
        T finalize()
        {
            T output;
            finalize(&output);
            return output;
        }
    };

    std::unique_ptr<Hash> createHash(char const* function);
}

#endif
