#include "../timer.h"
#include "../gpio.h"
#include "../assert.h"

#define SECOND_ONES 3
#define SECOND_TENTHS 2
#define MINUTE_ONES 1
#define MINUTE_TENTHS 0

#define DIGIT_OFFSET 10
#define REFRESH_DELAY 2500
#define ONE_SECOND 1
#define SEC_IN_MSEC 1000000
#define NO_DISPLAY 0

int segment_to_gpio_pin[] = {20, 21, 22, 23, 18, 16, 26, 27};

/*
 * Contains the array for the various possible digits to display.
 * Digits displayable - '0' to '9', and 'A' to 'F' 
 */

unsigned char digits[17] = {0x00, 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7f, 0x39, 0x3f, 0x79, 0x71}; 

/*
 * Configures GPIO pin 10-13 and GPIO pin 20-27 to be output pins. 
 * GPIO pins 10 - 13 determine where the various segments are displayed. 
 * 10 correspond to the left most, 13 to the right most)
 * GPIO pins 20 to 27 determines which segment gets displayed.
 * 20 correspond to segment A while 17 correpsond to dp.
 * GPIO PINS 4, 9, 5, 6, 7, 8, 24, 25, and 8 doesn't seem to work. 
 * GPIO 24 has been swapped 18
 * GPIO 25 has been swapped for 16
 */
void configure_output_pins() {
    for (int pin = GPIO_PIN16; pin < GPIO_PIN28; pin ++) {
        gpio_set_output(pin);
        assert(gpio_get_function(pin) == GPIO_FUNC_OUTPUT);
    }

    for (int pin = GPIO_PIN10; pin < GPIO_PIN14; pin ++) {
        gpio_set_output(pin);
        assert(gpio_get_function(pin) == GPIO_FUNC_OUTPUT);
    }
    gpio_set_output(GPIO_PIN6); // segment f
    gpio_set_output(GPIO_PIN8); // segment E
    gpio_set_output(GPIO_PIN0);
}

/*
 * Displays a digit by lighting up appropriate segments.
 * @param digit provides the digit to light up. From 0-9, A-F.
 * Assumes that appropriate d1 - d4 pins are on to display digits.
 */
void display_digit(int digit) {
    char digit_to_display = digits[digit];
    for (int i = 0; i < 7; i ++) {
        gpio_write(segment_to_gpio_pin[i], digit_to_display & 0x1); 
        digit_to_display >>= 1;
    }
}   

/*
 * Updates the display digit array by 1 in fourth index. 
 * Adjust digits accordingly like a clock.
 * @param int* dsplay provides an array of length 4. 
 * Index 1 correspond to MINUTE_TENTHS, index 4 to SECOND_ONES.
 */
void update_value_to_display(int* display) {
   *(display + SECOND_ONES) += 1;

    // Ensures seconds in ones place never exceed dispay value 9
    // Ensures Minutes in the tenths place never exceeds value 5. Resets if does.
    if (*(display + SECOND_ONES) >= 11 || *(display + MINUTE_TENTHS) >= 7) {
        if (*(display + MINUTE_TENTHS) >= 11) {
            for (int i = 1; i < 4; i ++) {*(display + i) = 0;}
            *display = 1;
        } 
        else { 
            *(display + SECOND_ONES) = 1;
            *(display + SECOND_TENTHS) = *(display + SECOND_TENTHS)?  
                *(display + SECOND_TENTHS) + 1:
                2 ;
        }
    }

    // Ensures secondss in tenths place never exceed display value 6
    if (*(display + SECOND_TENTHS) >= 7) {
        *(display + SECOND_TENTHS) = 1;
        *(display + MINUTE_ONES) += 2;
    }

    // Ensures minutes in ones place never exceed display value 9
    if (*(display + MINUTE_ONES) >= 11) {
        *(display + MINUTE_ONES) = 1;
        *(display + MINUTE_TENTHS) += 2;
    }
 }

/*
 * Allows displaying of multiple different digits at once on the 7 digit segment.
 * Does so by swithch each display segment on and off very quickly. (2500 micro-Sec)
 * The refresh loop lasts for one second.
 */
void refresh_loop(int* display) {
    for (int j = 0; j < SEC_IN_MSEC / (REFRESH_DELAY * 4); j ++) {
      for (int i = 0; i < 4; i ++) {
            gpio_write(i + DIGIT_OFFSET, 1);
            display_digit(display[i]);
            timer_delay_us(REFRESH_DELAY);
            gpio_write(i + DIGIT_OFFSET, 0);
        }
    }
}

void main(void) {
    configure_output_pins();

    int display[4] = {NO_DISPLAY, NO_DISPLAY, NO_DISPLAY, 1};
    while (1) {
        refresh_loop(display);
        update_value_to_display(display);
    }
}
