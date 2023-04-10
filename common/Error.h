#ifndef OWL_ERROR_H
#define OWL_ERROR_H

#include <cstdint>

namespace owl
{
    class hresult
    {
    public:
        constexpr hresult() = default;
        ~hresult() = default;

        constexpr hresult(uint32_t code) : code_{code} {}
        constexpr hresult(uint32_t code, char const* arr) noexcept
            : code_{code}
            , what_{arr}
        { }

        constexpr operator unsigned int() const noexcept { return code_; }

        constexpr uint32_t code() const noexcept        { return code_; }
        constexpr char const* what() const noexcept     { return what_; }

    private:
        uint32_t code_{0};
        char const* what_{"Unknown hresult"};
    };

    namespace error
    {
        // Success - first bit to 0
        constexpr hresult S_OK                          { 0x00000000, "Success" };

        // Errors - first bit to 1
        constexpr hresult E_UNKNOWN_ERRNO               { 0x80000001, "Unknown errno" };
        constexpr hresult E_NOT_SUPPORTED               { 0x80000002, "Not supported" };
        constexpr hresult E_INVALID                     { 0x80000003, "Invalid value" };
        constexpr hresult E_GENERIC                     { 0x80000004, "Generic error" };
        constexpr hresult E_TIMEOUT                     { 0x80000005, "Timeout"       };
        constexpr hresult E_BAD_CHECKSUM                { 0x80000006, "Bad checksum"  };
    }
}

#endif
