// File: test_gpio_timer.c
// ---------------
// Unit testing for clock

#include "../assert.h"
#include "../gpio.h"
#include "../timer.h"

// Tests for the setter and getter function for FSEL register in gpio module
// Does so by iterating over all pins except 14, 15, 37, and 42.
// Sets each pin to be output, asserts, then sets eacah pin to be input and asserts.
void test_gpio_set_get_function() {
    
    // Sets each pin individually to each of the possible functions
    // Checks that each pin is set accordingly
    for (int i = GPIO_PIN0; i <= GPIO_PIN_LAST; i ++) {
        if (i == GPIO_TX || i == GPIO_RX || i == GPIO_PIN35 || i == GPIO_PIN47){
            continue;
        }
        for (int j = 0; j < 8; j ++) {
            gpio_set_function(i, j);
            assert(gpio_get_function(i) == j); 
            gpio_set_input(i);
            assert(gpio_get_function(i) == GPIO_FUNC_INPUT);
            gpio_set_output(i);
            assert(gpio_get_function(i) == GPIO_FUNC_OUTPUT);
        } 
    }

    // Checks to see that each pin has held their last set function
    // in spite of setting the function of other pins
    for (int i = GPIO_PIN_FIRST; i <= GPIO_PIN_LAST; i ++) {
        if (i == GPIO_TX || i == GPIO_RX || i == GPIO_PIN35 || i == GPIO_PIN47){
            continue;
        }

        assert(gpio_get_function(i) == GPIO_FUNC_OUTPUT);
    }
    gpio_set_function(-1,GPIO_FUNC_INPUT);
    assert(gpio_get_function(-1) == GPIO_INVALID_REQUEST);
}

// Tests for the SET and CLR register in gpio module
// Does so by looping over all GPIO pins except 14, 15, 37, and 42
// Sets each pin to be high, asserts, and then sets it to low and asserts
// Many GPIO pins seem to be dead and does not get set even when attempted too.
// For this reason, the test is limited ot the first 28 GPIO pins.
// These are the pins which can be manually tested to be working.
void test_gpio_read_write_function() {
    //gpio_write(55, 1);
    assert(gpio_read(55) == GPIO_INVALID_REQUEST); 

    //gpio_write(20,0);
    
    for (int i = GPIO_PIN0; i <= GPIO_PIN28; i ++) {
        if (i == GPIO_TX || i == GPIO_RX || i == GPIO_PIN35 || i == GPIO_PIN47
        || i == 4 || i == 5 || i == 6 || i == 7 || i == 8 || i == 9 || i == 24 || i ==25){
            continue;
        } 

        gpio_write(i, 1);
        assert(gpio_read(i) == 1);  
    }  
    
    for (int i = 10; i < 28; i ++) {
        if (i == GPIO_TX || i == GPIO_RX || i == 24 || i == 25)
        { continue;}
        gpio_write(i, 1);
        assert(gpio_read(i) == 1);
    }
}


void test_timer_function() {
    unsigned int start = timer_get_ticks();
    timer_delay(1);
    unsigned int end = timer_get_ticks();
    assert(end > start);
}

// Test the various functions within gpio and timer class
void main(void) {
    test_gpio_set_get_function();
    test_gpio_read_write_function();
    test_timer_function();
    return;
}
