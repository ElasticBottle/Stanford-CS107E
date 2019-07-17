#include "gpio.h"

#define GPIO_FSEL0 ((unsigned int *) 0x20200000)
#define GPIO_FSEL1 ((unsigned int *) 0x20200004)
#define GPIO_FSEL2 ((unsigned int *) 0x20200008)
#define GPIO_FSEL3 ((unsigned int *) 0x2020000c)
#define GPIO_FSEL4 ((unsigned int *) 0x20200010)
#define GPIO_FSEL5 ((unsigned int *) 0x20200014)

#define GPIO_CLR0 ((volatile unsigned int *) 0x20200028)
#define GPIO_CLR1 ((volatile unsigned int *) 0x2020002c)

#define GPIO_SET0 ((volatile unsigned int *) 0x2020001c)
#define GPIO_SET1 ((volatile unsigned int *) 0x20200020)

#define GPIO_LEV0 ((volatile unsigned int *) 0x20200034)
#define GPIO_LEV1 ((volatile unsigned int *) 0x20200038)

#define PINS_PER_FSEL_REG (int) 10
#define BITS_PER_PIN (int) 3
#define MAX_VAL_IN_3_BITS (int) 7
#define SET (int) 1
#define CLR (int) 0
#define BYTEDIFF 4


void gpio_init(void) {
}


void gpio_set_function(unsigned int pin, unsigned int function) {
    if (pin > GPIO_PIN_LAST) { return; }
    unsigned int * loc = GPIO_FSEL0;
    loc += pin / PINS_PER_FSEL_REG;
    int shift_amount = (pin % PINS_PER_FSEL_REG) * BITS_PER_PIN;
    *loc &= ~(MAX_VAL_IN_3_BITS << shift_amount); 
    *loc |= function << shift_amount;
}

unsigned int gpio_get_function(unsigned int pin) {
    if (pin > GPIO_PIN_LAST) { return GPIO_INVALID_REQUEST; }
    unsigned int * loc = GPIO_FSEL0;
    loc += pin / PINS_PER_FSEL_REG;
    int shift_amount = (pin % PINS_PER_FSEL_REG) * BITS_PER_PIN;
    unsigned int mask = MAX_VAL_IN_3_BITS << shift_amount;
    mask = *loc & mask;
    mask >>= shift_amount;
    return mask;
}

void gpio_set_input(unsigned int pin) {
    gpio_set_function(pin, GPIO_FUNC_INPUT);
}

void gpio_set_output(unsigned int pin) {
    gpio_set_function(pin, GPIO_FUNC_OUTPUT);
}

void gpio_write(unsigned int pin, unsigned int value) {
    if (pin > GPIO_PIN_LAST) { return; } // Check GPIO pin number valid
    volatile unsigned int* setting_loc;
    setting_loc = (value)? GPIO_SET0: GPIO_CLR0; // Get base SET/CLR register
    setting_loc += pin / 32; // increment register to appropriate location
    *setting_loc = (1 << (pin % 32)); // Set the bit at the location
}

unsigned int gpio_read (unsigned int pin) {
	if (pin < GPIO_PIN_LAST) {		// Check GPIO pin number valid
		unsigned int mask_bit = 1 << (pin % 32);		// Create mask bit
		volatile unsigned int * lev_loc = GPIO_LEV0;
		lev_loc += (pin / 32);
        if (*lev_loc & mask_bit) { return 1;}
        else { return 0; }	// Return true if bit set
    }
    return GPIO_INVALID_REQUEST;	// Return false
}

