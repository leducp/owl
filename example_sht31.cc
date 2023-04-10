//#include "Time.h"

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "hardware/uart.h"

#include "hal/i2c.h"
#include "sensors/SHT31.h"

// NMEA
#include <algorithm>
#include <vector>
#include <string_view>
#include <iostream>
#include <charconv>
#include <cmath>

using namespace owl;

void on_uart_rx() 
{
    while (uart_is_readable(uart1)) 
    {
        uint8_t ch = uart_getc(uart1);
        //printf("%c", ch);
    }
}

int main()
{
    stdio_init_all();
    printf("**** Start ****\n");

    I2C i2c(1, 100 * 1000);

    gpio_set_function(10, GPIO_FUNC_I2C);
    gpio_set_function(11, GPIO_FUNC_I2C);
    //gpio_pull_up(6);
    //gpio_pull_up(7);

    // printf
    gpio_set_function(16, GPIO_FUNC_UART);
    gpio_set_function(17, GPIO_FUNC_UART);

    // GPS
    /*
    uart_init(uart1, 9600);
    gpio_set_function(4, GPIO_FUNC_UART);
    gpio_set_function(5, GPIO_FUNC_UART);
    uart_set_hw_flow(uart1, false, false);
    uart_set_format(uart1, 8, 1, UART_PARITY_NONE);
    uart_set_fifo_enabled(uart1, true);

    irq_set_exclusive_handler(UART1_IRQ, on_uart_rx);
    irq_set_enabled(UART1_IRQ, false);
*/
    SHT31 sensor(i2c);

    while (1)
    {
        hresult rc = sensor.fetch();
        if (rc == error::E_GENERIC)
        {
            printf("nobody\n");
            sleep_ms(2000);
            continue;
        }

        printf("%d.%d°C %d.%d %%HR\n\r",
            sensor.temperature() / sensor.precision(), sensor.temperature() % sensor.precision(),
            sensor.humidity() / sensor.precision(), sensor.humidity() % sensor.precision());
    }
/*
    while (1)
    {
        //printf("**** RUNNING ****\n");
        sleep_ms(1000);

        while (uart_is_readable(uart1)) 
        {
            uint8_t ch = uart_getc(uart1);
            printf("%c", ch);
        }
        printf("\n");

    }
    */
}
