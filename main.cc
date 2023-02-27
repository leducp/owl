#include "common/Ring.h"

using namespace owl;

int main()
{
    Ring<uint32_t, 16> ring;
    ring.push(42);

    return ring.size();
}
