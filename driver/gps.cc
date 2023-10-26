#include <nuttx/sensors/sht3x.h>

#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>

#include <string_view>
//#include <charconv> // Not available on NuttX

#include "gps.h"
#include "Time.h"

namespace owl
{
    static pthread_t thread;
    static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    static GPS gps;

    static void* worker(void* arg)
    {
        int fd = ::open("/dev/ttyS1", O_RDWR);
        if (fd < 0)
        {
            printf("aie: %s\n", strerror(errno));
            gps.state = State::FAULT;
            return nullptr;
        }

        char frame[128];
        while (true)
        {
            char c;
            int r = ::read(fd, &c, 1);
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
                int r = ::read(fd, &c, 1);
                if (r <= 0)
                {
                    sleep(10ms);
                    continue;
                }

                frame[i] = c;
                ++i;
            }

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
            printf("emitter: %.*s\n", cmd.emitter.size(), cmd.emitter.data());
            printf("command: %.*s\n", cmd.command.size(), cmd.command.data());
            for (auto const& param : cmd.parameters)
            {
                printf(" -> %.*s\n", param.size(), param.data());
            }
*/

            auto extract = [&](int index, auto& output)
            {
                auto field = cmd.parameters.at(index);
                char* end;;
                if constexpr (std::is_integral<decltype(output)>::value)
                {
                    output = strtol(field.data(), &end, 10);
                }
                else
                {
                    output = strtof(field.data(), &end);
                }
            };

            if (cmd.command == "GGA")
            {
                int fix_time = 0;
                float latitude = 0;
                float longitude = 0;
                int fix_quality = 0;
                int satellites = 0;
                float DOP;
                float MSL;

                extract(0, fix_time);
                extract(1, latitude);
                extract(3, longitude);
                extract(5, fix_quality);
                extract(6, satellites);
                extract(7, DOP);
                extract(8, MSL);

                latitude  = int(latitude / 100)  + std::fmod(latitude, 100) / 60;
                longitude = int(longitude / 100) + std::fmod(longitude, 100) / 60;
                if (cmd.parameters.at(2) == "S")
                {
                    latitude = -latitude;
                }
                if (cmd.parameters.at(4) == "W")
                {
                    longitude = -longitude;
                }

                pthread_mutex_lock(&mutex);
                gps.time        = fix_time;
                gps.latitude    = latitude;
                gps.longitude   = longitude;
                gps.altitude    = MSL;
                gps.fix_quality = fix_quality;
                gps.satellites  = satellites;
                gps.dop         = DOP;
                pthread_mutex_unlock(&mutex);

                printf("---- INFO ----\n");
                printf("time    :  %d\n", fix_time);
                printf("latitude:  %f\n", latitude);
                printf("longitude: %f\n", longitude);
                printf("fix:   %d\n", fix_quality);
                printf("sats : %d\n", satellites);
                printf("DOP:   %.3f\n", DOP);
                printf("MSL:   %.3f\n", MSL);
            }

            if (cmd.command == "RMC")
            {
                int date = 0;
                extract(8, date);

                pthread_mutex_lock(&mutex);
                gps.date = date;
                pthread_mutex_unlock(&mutex);
                printf("Date:   %d\n", date);
            }
        }
    }

    void init_gps()
    {
        pthread_create(&thread, nullptr, worker, nullptr);
    }

    void update(GPS& gps_out)
    {
        pthread_mutex_lock(&mutex);
        gps_out = gps;
        pthread_mutex_unlock(&mutex);
    }
}

/*
---- INFO ----
time    :  162043
latitude:  45.761253
longitude: 2.126517
fix:   1
sats : 9
DOP:   0.960
MSL:   837.000

*/