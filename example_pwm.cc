#include <cstdio>

#include "Time.h"

#include "pico/stdlib.h"
#include "hardware/pwm.h"

using namespace owl;

constexpr int PWM_SPAN = 10000;

int rec_size = 0;
char bufferA[256];
char bufferB[256];

char* to_load = bufferA;
char* ready = nullptr;

// RX interrupt handler
extern "C" void on_uart_rx() 
{
    while (uart_is_readable(uart0)) 
    {
        char ch = uart_getc(uart0);
        to_load[rec_size] = ch;

        if (ch == '\n' or ch == '\r')
        {
            to_load[rec_size] = '\0';
            rec_size = 0;
            ready = to_load;
            if (to_load == bufferA)
            {
                to_load = bufferB;
            }
            else
            {
                to_load = bufferA;
            }
            return;
        }

        ++rec_size;
        if (rec_size > 255)
        {
            rec_size = 0;
        }
    }
}

int main()
{
    // printf
    stdio_init_all();
    gpio_set_function(16, GPIO_FUNC_UART);
    gpio_set_function(17, GPIO_FUNC_UART);
    
    // And set up and enable the interrupt handlers
    irq_set_exclusive_handler(UART0_IRQ, on_uart_rx);
    irq_set_enabled(UART0_IRQ, true);

    // Now enable the UART to send interrupts - RX only
    uart_set_irq_enables(uart0, true, false);

    gpio_set_function(0, GPIO_FUNC_PWM);
    gpio_set_function(1, GPIO_FUNC_PWM);

    uint slice0   = pwm_gpio_to_slice_num (0); 
    uint channel0 = pwm_gpio_to_channel (0);
    pwm_set_wrap (slice0, PWM_SPAN);
    pwm_set_chan_level (slice0, channel0, 0);
    pwm_set_enabled (slice0, true);
/*
    uint slice1   = pwm_gpio_to_slice_num (1); 
    uint channel1 = pwm_gpio_to_channel (1);
    //pwm_set_wrap (slice1, 6250);
    pwm_set_chan_level (slice1, channel1, 10);
    pwm_set_enabled (slice1, true);
*/
    int counter = 8000;
    constexpr int step = 10;
    constexpr nanoseconds pause = 1ms;

    bool dir = true;
    while (true)
    {
        owl::sleep(pause);
        pwm_set_chan_level (slice0, channel0, counter);

        if (dir)
        {
            counter += step;
            if (counter >= PWM_SPAN)
            {
                dir = false;
            }
        }
        else
        {
            counter -= step;
            if (counter <= 0)
            {
                dir = true;
            }
        }

        if (ready)
        {
            printf("echo: %s\n", ready);
            if (strcmp(ready, "ch"))
            ready = nullptr;
        }
    }
}
