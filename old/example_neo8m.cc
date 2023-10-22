#include "hal/serial.h"
#include <algorithm>
#include <vector>
#include <string_view>
#include <iostream>
#include <charconv>
#include <cmath>

using namespace owl;

int main()
{
    Serial serial;
    hresult rc = serial.open("/dev/ttyUSB0", 9600);
    if (rc != error::S_OK)
    {
        printf("%s\n", rc.what());
        return -1;
    }

    char frame[128];
    while (true)
    {
        char c;
        int r = serial.read(&c, 1);
        if (r <= 0)
        {
            sleep(10ms);
            continue;
        }

        if (c != '$')
        {
            continue;
        }

        frame[0] = c;
        int i = 1;
        while (c != '\n')
        {
            int r = serial.read(&c, 1);
            if (r <= 0)
            {
                sleep(10ms);
                continue;
            }

            frame[i] = c;
            ++i;
        }

/*
        printf("rec (%d): %.*s", i, i, frame);
        char checksum = 0;
        for (int k = 1; k < (i - 5); ++k) // -5 => *cc\r\n
        {
            checksum ^= frame[k];
        }
        printf("=> %02x\n", checksum);
*/
        struct Command
        {
            std::string_view emitter;
            std::string_view command;
            std::vector<std::string_view> parameters;
        };

        Command cmd;
        cmd.emitter = std::string_view(frame + 1, 2);
        cmd.command = std::string_view(frame + 3, 3);

        char const* last = frame + 7;
        char const* end  = frame + i;
        char const* pos  = std::find(last, end, ',');
        while(pos != end)
        {
            uint32_t size = pos - last;
            cmd.parameters.push_back({last, size});

            last = pos + 1;
            pos  = std::find(last, end, ',');
        }
/*
        std::cout << " emitter: " << cmd.emitter << std::endl;
        std::cout << " command: " << cmd.command << std::endl;
        for (auto const& param : cmd.parameters)
        {
            std::cout << " -> " << param << std::endl;
        }
*/
        if (cmd.command != "GGA")
        {
            continue;
        }

        int fix_time = 0;
        float latitude = 0;
        float longitude = 0;
        int fix_qual = 0;
        int satellites = 0;
        float DOP;
        float MSL;

        auto extract = [&](int index, auto& output)
        {
            auto field = cmd.parameters.at(index);
            auto result = std::from_chars(field.data(), field.data() + field.size(), output);
            (void) result;
        };

        extract(0, fix_time);
        extract(1, latitude);
        extract(3, longitude);
        extract(5, fix_qual);
        extract(6, satellites);
        extract(7, DOP);
        extract(8, MSL);

        latitude = int(latitude / 100)  + std::fmod(latitude, 100) / 60;
        longitude = int(longitude / 100) + std::fmod(longitude, 100) / 60;
        if (cmd.parameters.at(2) == "S")
        {
            latitude = -latitude;
        }
        if (cmd.parameters.at(4) == "W")
        {
            longitude = -longitude;
        }

        if (fix_qual == 0)
        {
            printf("Waiting for GPS fix\n");
            continue;
        }


        printf("---- INFO ----\n");
        printf("latitude:  %f\n", latitude);
        printf("longitude: %f\n", longitude);
        printf("fix:   %d\n", fix_qual);
        printf("sats : %d\n", satellites);
        printf("DOP:   %.3f\n", DOP);
        printf("MSL:   %.3f\n", MSL);
    }
}
