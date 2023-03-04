//#include "Time.h"

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

#include "hal/i2c.h"
#include "sensors/SHT31.h"

using namespace owl;

int main()
{
    stdio_init_all();
    sleep_ms(4500);

    I2C i2c(1, 100 * 1000);

    gpio_set_function(6, GPIO_FUNC_I2C);
    gpio_set_function(7, GPIO_FUNC_I2C);
    gpio_pull_up(6);
    gpio_pull_up(7);

    SHT31 sensor(i2c);


    while (1)
    {
        hresult rc = sensor.fetch();
        if (rc == E_GENERIC)
        {
            printf("nobody\n");
            sleep_ms(2000);
            continue;
        }

        printf("%d.%d°C %d.%d %%HR\n\r",
            sensor.temperature() / sensor.precision(), sensor.temperature() % sensor.precision(),
            sensor.humidity() / sensor.precision(), sensor.humidity() % sensor.precision());
    }
}
