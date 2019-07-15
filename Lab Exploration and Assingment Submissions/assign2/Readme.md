## Assignment 2: Implement a clock

Assignment can be found [here](http://cs107e.github.io/assignments/assign2/#extension-set-time)

The timer works as expected.

After one second, the display change to reflect that.

The buttons interact with the system to make it work like a stopwatch.

![Final set-up of timer][final rpi set-up]

## How it works

### After just loading the program onto the pi
Upon loading `./apps/clock.bin` onto the pi, `00.00` will show.

Here, the capped button (left-most) increments the minute by one.

The un-capped button (right-most) increments the second by one.

### Starting the timer

To start the watch, press both buttons down at the same time.

Once done, the clock will start ticking.

To stop it, press the un-capped button once.

The clock should blink, indicating that it has stopped.

Pressing the un-capped button again will continue to timer.

Pressing the capped button will reset the timer. You will then be able to adjust the seconds and minutes again.

## Conclusion

And that's it!

Pretty simple program that took me ages to write because there seems to be some hardware error on my Pi.

What errors?

Certain GPIO pins aren't working properly. (GPIO16 will work when I adjust GPIO6)

That idea evaded me for so long that I kept trying to figure out what went wrong with my code.

Frustrating, but really fun stuff!

[final rpi set-up]: ./final_rpi_set-up.jpg
