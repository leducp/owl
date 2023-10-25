#include "Hash.h"
#include "FrameQueue.h"

using namespace owl;

int main(int argc, char* argv[])
{
    auto crcx25 = createHash("crc16/x25");

    auto test_x25 = [&](std::vector<char> const& data)
    {
        uint16_t result;
        crcx25->init();
        crcx25->update(data);
        crcx25->finalize(&result);
        printf("result: %02x\n", result);
    };

    test_x25({});
    test_x25({'a'});
    test_x25({'a', 'b', 'c'});
    test_x25({28, 42});

    struct Yolo
    {
        int data;
        char msg[8];
    };
    Yolo test{0xcafe7d, "Hello !"};

    std::queue<char> channel;
    FrameQueue frameOut(&channel);
    FrameQueue frameIn(&channel);

    channel.push(0x7e);
    frameOut.write(&test, sizeof(Yolo));
    channel.push(0xca);
    channel.push(0xfe);
    test.data = 0xdeca7e;
    frameOut.write(&test, sizeof(Yolo));
    channel.push(0xca);
    channel.push(0xfe);

    printf("in channel: %d\n", channel.size());

    while (frameIn.read())
    {
        Yolo rec;
        frameIn.fetch(rec);

        printf("yay %x %s\n", rec.data, rec.msg);
    }

    return 0;
}