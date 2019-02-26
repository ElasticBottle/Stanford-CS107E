#include "gpio.h"
#include "gpioextra.h"
#include "uart.h"
#include "interrupts.h"
#include "ringbuffer.h"
#include "printf.h"
#include "assert.h"

static const int BUTTON = GPIO_PIN25;

void button_init(void)
{
}

static bool button_press(unsigned int pc) 
{
    return false;
}

void setup_interrupts(void) 
{
    gpio_enable_event_detection(BUTTON, GPIO_DETECT_FALLING_EDGE);
    interrupts_attach_handler(button_press);
    interrupts_enable_source(INTERRUPTS_GPIO3);
    interrupts_global_enable();
}

void wait_for_click(void)
{
}

void main(void) 
{
    gpio_init();
    uart_init();
    button_init();

    while (1) {
       wait_for_click();
    }
}
