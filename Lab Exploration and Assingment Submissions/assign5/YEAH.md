---
---

## Spec/approach

- Note how each layer of keyboard is isolated from concerns above and below:
    - `read_scancode` is the only place to fuss with clock/data gpio, bit order, errors/timeouts (but has no concern with all the rest)
    - `read_sequence` gathers sequence of good scancodes and translates to physical key action, only concern how to recognize 1-2-3 byte sequences
    - `read_event` translates physical key to logical (uses ps2_key array), also handles modifiers
    - `read_next` top-level to grab typed character
- Limited global variables are ok (necessary in fact)
    - Keyboard driver stores clock/data gpio, modifiers state
    - Shell store print function, command table (given in starter code)
        + May want to review the function pointer syntax
    - Mark static to not pollute global namespace
- Hint: shell_evaluate can borrow tokenize code from lab4
    - Be sure to cite!
- Shell commands
    - Function pointers: review syntax, how to use
    - peek/poke can use `strtonum`              
            
## Pitfalls
- One scancode's worth of bits needs to be read without breaks. Attempt to printf in between bits will likely miss some

## Productivity/process
- Understand layers of abstraction
    - shell -> `keyboard_read_next` -> `keyboard_read_event` -> `keyboard_read_sequence` -> `keyboard_read_scancode`
    - start from the bottom up!
    
- `keyboard_read_scancode`
    - use code from lab/review [part 4 of lab5](http://cs107e.github.io/labs/lab5/#4-implement-read-scancode)
    
- `keyboard_read_sequence`
    - read multiple scancodes if necessary (up to 3)
    - special cases (recall from lab 5 section 4: fix keyboard_read_sequence to recognize when a scancode is not a standalone action, but part of a larger sequence, such as the two scancodes sent together for a key up action)
       - key releases (breakcode `0xf0` called before key scancode)
       - extended key 
    - if error, read again starting from start 
    - return `key_action_t` struct
        ```
        typedef struct {
            enum { KEY_PRESS, KEY_RELEASE } what;
            unsigned char keycode;
        } key_action_t;
        ```
- `keyboard_read_event`
    - Returns a `key_event_t` struct
        ```
        typedef struct {
            enum { KEY_PRESS, KEY_RELEASE } what;
            unsigned char keycode;
        } key_action_t;
        ```
    + struct includes the key that was pressed or released and modifier flags (CTRL, ALT, SHIFT, etc.)
    - [ps2.h](https://github.com/cs107e/cs107e.github.io/blob/master/cs107e/include/ps2.h) may be helpful 
    - ps2_keys[] is an array that maps scancodes to `ps2_key_t` structs
        ```
        typedef struct {
            unsigned char ch;
            unsigned char other_ch;
        } ps2_key_t;
        ```
    - Ex. `ps2_keys[0x1C]->ch = 'a'`
    - remember to update modifiers (control, alt, shift, caps, num, scroll)
    - consider using static variables to store global modifiers
    
- `keyboard_read_next`
    - reads key typed on the keyboard and returns ASCII value
    - calls `keyboard_read_event`, gets a `key_event_t` struct:
        ```
        typedef struct {
            key_action_t action;                
            ps2_key_t key;                     
            keyboard_modifiers_t modifiers;     
        } key_event_t;
        ```
    - remember, we don't want non-printable ASCII values such as backspace
        - specifically, return these ASCII values:
            - `<= 0x7f` (printable chars)
            - `>= 0x90` (modifiers Ex. arrows)
 
 - Recap: return types of some important functions and example return values
    - `keyboard_read_scancode` returns 
        ```
        char scanocde Ex. 0x1C
        ```
    - `keyboard_read_sequence` returns 
    	```
        key_action_t {
    		what = PRESSED
    		keycode = 0x1C
    	}
        ```
   	- `keybaord_read_event` returns 
    	```
       	key_event_t {
    	    key_action_t action = (thing returned from keyboard_read_sequence)
    	    ps2_key_t key = ps2_keys.keycode
    	    modifiers = 0b00000
    	 }
    	```
	- we recommend making a helper function to keep track of the modifiers (Ex. get_modifier to track the global modifier and the current key press)
  
  - `shell`
    - Smooth sailing from here - pat yourselves on the back you're over the hump (keyboard) now :clap: :clap: :clap: 
    - make sure to `shell_printf` what you are typing
    - use command_t struct to store shell commands in a table 
    	- function pointer: ```int (*fn) (int argc, const char argv[])```
    	- just run ```cmds->fn(argc, argv)``` where cmds is the commands table
    		- argc = # of arguments
    		- argv = array of arguments

## :boom: Useful .h files :boom: 
- [keyboard.h](https://github.com/cs107e/cs107e.github.io/blob/master/cs107e/include/keyboard.h)  
- [ps2.h](https://github.com/cs107e/cs107e.github.io/blob/master/cs107e/include/ps2.h)  
- [shell.h](https://github.com/cs107e/cs107e.github.io/blob/master/cs107e/include/shell.h)  
- [shell_commands.h](https://github.com/cs107e/cs107e.github.io/blob/master/cs107e/include/shell_commands.h)  

## Triage when in a jam
- try reference implementation of the keyboard (using the test application from lab 5) if you're unsure how to handle cases

## Debugging
- Don't debug scancodes with printf as that will cause you to miss falling clock edges and screw up the timing

## Extension
