#include "../timer.h"
#include "../gpio.h"
#include "../assert.h"

#define SECOND_ONES 3
#define SECOND_TENTHS 2
#define MINUTE_ONES 1
#define MINUTE_TENTHS 0

#define DIGIT_OFFSET 10
#define REFRESH_DELAY 2500
#define BUTTON_READ_DELAY 0.25
#define ONE_SECOND 1
#define SEC_TO_MILLISEC 1000
#define MILLISEC_TO_MSEC 1000

#define GPIO_LEV0 ((volatile unsigned int*) 0x20200034)

int segment_to_gpio_pin[] = {20, 21, 22, 23, 18, 16, 26, 27};

/*
 * Contains the array for the various possible digits to display.
 * Digits displayable - '0' to '9', and 'A' to 'F' 
 */

unsigned char digits[16] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7f, 0x39, 0x3f, 0x79, 0x71}; 

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
void configure_input_output_pins() {
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
    gpio_set_output(GPIO_PIN0); // No clue, but nothing lights up without this.

    //Setting input for push buttons
    gpio_set_input(GPIO_PIN2); 
    gpio_set_input(GPIO_PIN3); 
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
 * Adjust digits accordingly to work like a clock.
 * @param int* dsplay provides an array of length 4. 
 * Index 1 correspond to MINUTE_TENTHS, index 4 to SECOND_ONES.
 */
void update_value_to_display(int* display) {
    // Ensures seconds in ones place never exceed dispay value 9
    // Ensures Minutes in the tenths place never exceeds value 5. Resets if does.
    if (*(display + SECOND_ONES) >= 10 || *(display + MINUTE_TENTHS) >= 6) {
        if (*(display + MINUTE_TENTHS) >= 10) {
            for (int i = 0; i < 4; i ++) {*(display + i) = 0;}
        } 
        else { 
            *(display + SECOND_ONES) = 0;
            *(display + SECOND_TENTHS) += 1;
        }
    }

    // Ensures secondss in tenths place never exceed display value 6
    if (*(display + SECOND_TENTHS) >= 6) {
        *(display + SECOND_TENTHS) = 0;
        *(display + MINUTE_ONES) += 1;
    }

    // Ensures minutes in ones place never exceed display value 9
    if (*(display + MINUTE_ONES) >= 10) {
        *(display + MINUTE_ONES) = 0;
        *(display + MINUTE_TENTHS) += 1;
    }
 }

/*
 * Allows displaying of multiple different digits at once on the 7 digit segment.
 * Does so by swithch each display segment on and off very quickly. (2500 micro-Sec)
 */
void refresh_loop(int* display) {
    for (int i = 0; i < 4; i ++) {
        gpio_write(i + DIGIT_OFFSET, 1);
        display_digit(display[i]);
        if (i ==  MINUTE_ONES && display[MINUTE_ONES] >= 0) {
            gpio_write(GPIO_PIN27, 1); // Adds decimal point
        }
        timer_delay_us(REFRESH_DELAY);
        gpio_write(i + DIGIT_OFFSET, 0);
        gpio_write (GPIO_PIN27, 0); // turns decimal point off.
    }
}

/*
 * Refreshes the display.
 * @param int* display provides the int array for the display digits
 * @delay_in_milliseconds provides the duration of the display refresh.
 * In effect, the program isn't reading input during this delay preiod
 */
void refresh_duration(int* display, int delay_in_milliseconds){
  for (volatile int j = 0; 
          j < delay_in_milliseconds * MILLISEC_TO_MSEC / (REFRESH_DELAY * 4); 
          j ++) {          
      refresh_loop(display);
  }
}

/*
 * Starts the timer module that increments the display every second.
 * Scans Un-capped button to stop/stop the timer.
 * Scans Capped button to reset display and exit timer.
 */
int start_timer(int* display) {
    volatile unsigned int* lev_loc = GPIO_LEV0;
    while(1) {
        *(display + SECOND_ONES) += 1;
        update_value_to_display(display);
        refresh_duration(display, ONE_SECOND * SEC_TO_MILLISEC);
        if (!(*lev_loc & (1 << GPIO_PIN3))) { break; }
    }
    timer_delay_ms(BUTTON_READ_DELAY * SEC_TO_MILLISEC);

    // Scans for restarting/resetting the timer
    while (1) {
        refresh_duration(display, BUTTON_READ_DELAY * SEC_TO_MILLISEC);
        if (!(*lev_loc & (1 << GPIO_PIN3))) { 
            if (start_timer(display)) {break;} 
        } 
        if (!(*lev_loc & (1 << GPIO_PIN2))) {
            for (int i = 0; i < 4; i ++)
                display[i] = 0;
            timer_delay_ms(BUTTON_READ_DELAY * SEC_TO_MILLISEC);
            return 1;
        }
    }
    return 1;
}

void main(void) {
    configure_input_output_pins();

    int display[4] = {0, 0, 0, 0};
    while (1) {
        volatile unsigned int* lev_loc = GPIO_LEV0;
        // Checks for input
        if (!(*lev_loc & 12)) {start_timer(display);}
        if (!(*lev_loc & (1 << GPIO_PIN2))) { display[MINUTE_ONES] ++; }
        if (!(*lev_loc & (1 << GPIO_PIN3))) { display[SECOND_ONES] ++; }
        
        //Update displat to reflect input, if any
        update_value_to_display(display);
        refresh_duration(display, BUTTON_READ_DELAY * SEC_TO_MILLISEC);        
    }
}
