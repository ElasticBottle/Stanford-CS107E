# Assignment 3: YEAH!
First, make sure you read the spec thoroughly! It's worth your time to make sure you've understood what's going there as much as possible. Also make sure you read through the `.h` files as they desrcibe in detail what we're expecting from each function.



## Strings module
Keep an eye out for what your functions should be returning, and how they should behave in the case of an error.

### Notes on functions
- `void *memcpy(void *dst, const void *src, size_t n)`:
    - Copy n bytes, regardless of content.
    - `void *` is a generic pointer.
- `size_t strlen(const char *s)`:
    - Return the length of string (excluding the null terminator!).
    - `const char *` means you can't change the value that the pointer points to. Makes sense because we just need to get the length so no need to change anything.
- `int strcmp(const char *s1, const char *s2)`:
    - Note that you can compare `char`s directly! (treat them like 1-byte integers and subtract to get a neg/pos/zero result).
- `unsigned int strtonum(const char *str, const char **endptr)`:
    - Only needs to handle positive numbers, but you do need to handle hex numbers.
    - **What is this scary `**endptr` for?**
        - `**endptr` is a double pointer that we can use to let the caller know which `char` the conversion stopped at.
        - That `char` is going to be either the **first** invalid `char` in `*str` **or** the null terminator if all characters were valid. 
        - Whoever called this function kindly gave us a pointer to {a pointer to a `char`} which lives somewhere with them (the caller).
        - So now *we* can dereference the double pointer in order to put {a pointer to the `char` where the conversion stopped} in a place where the caller knows to look.
        - Essentially, these double pointer shennanigans allow us to "return" a second thing in addition to the `unsigned int`.
- `size_t strlcat(char *dst, const char *src, size_t maxsize)`
    - `maxsize` accounts for null terminator 
    - Some examples:
        1. `maxsize = 8`  
           `src = "89"`  
           `dst = "1234567"`  
           This input should return `9`, and `dst` should now be pointing to a string that reads "1234567" followed by a null terminator.  
        2. `maxsize = 15`  
           `src = "89"`  
           `dst = "1234567"`  
           This input should return `9`, and `dst` should now be pointing to a string that reads "123456789" followed by a null terminator.  
    - Note that return values are the same regardless of whether the string is chopped off or not.
    - May seem like an odd little function, but it's the perfect go-to for bounded concatenation which is just what `snprintf` needs...
- `int unsigned_to_base(char *buf, size_t bufsize, unsigned int val, int base, int min_width)`
    - `min_width` is the minimum number of characters (excluding null terminator) your string should be (pad with zeros if too short). `bufsize` is how much space you have to write to (including null terminator).
        - Apply padding based on `min_width` first, *then* use `bufsize` to chop off the end of the string if needed
        - Some examples:
            1. `unsigned_to_base(buf, 20, 128, 16, 6)`  
            128 -> "80" in hex  
            write "000080\0"  (i.e. "000080" followed by null terminator)  
            return 6  
            2. `unsigned_to_base(buf, 20, 128, 4, 6)`  
                write "000\0"  
                return 6  
            3. `unsigned_to_base(buf, 20, 128, 16, 0)`  
                write "80\0"  
                return 2  
        - It is somewhat easier if you process the number in reverse (i.e. trailing digit first and leading digit last)
            -  You can write the digits (including zero padding) into a separate buffer, from R to L, and then transfer whatever fits into `buf`. (use `strlcat` in this case!)
            -  or write L to R and reverse later



## `printf/snprintf/vsnprintf` module
- Variadic functions
    - Allow you to write functions in C that can take a variable number of arguments.
    - Use `va_list` to retrieve arguments, pass `va_list` to `vsnprintf`.
    - Syntax: `va_arg(ap, type)` (What would be the type for printf?).
- Don't forget to `va_end()`
- Keep your code organized
- Use your hard work from `string.h`
- `printf` and `snprintf` should be calling `vsnprintf`
- Convenient to use a temporary buffer to assemble each format argument, and copy from temp to output (alternative of trying to write directly to output is messier, esp. for handling truncation)



## Common Pitfalls
- Make sure you have NULL termination -- valid C strings always end with `'\0'`
    - anything mentioning `size` is referring to the buffer size: it includes the null terminator so make space for it / keep it in mind!
    - anything mentioning `length` or `width` is referring to the length of the string, i.e. the number of characters NOT including the null terminator.
- Note the difference between GDB and actual HW
    - GDB will usually have zeroed out uninitialized memory whereas the Pi will have gibberish (random characters). That means if you forget your null terminators, GDB will probably still print out your strings just fine, but the Pi will spit out a lot of nonsense until it happens to reach a `'\0'` somewhere down the line.



## Tips for staying productive
- Test your string library thoroughly
    - Hint: you *might* be using your string library in your `printf` module, so make sure it works properly before moving on.
- Take teeny tiny epsilon-steps, test every step!
    - If you make a small change to a working system and it stops working, you know exactly where to start your search for bugs!



### If you're in a pickle...
- Omit field width on integers; rest of functionality can be tested without it



### Debugging advice
In the spec, there are some instructions on how to use our `printf` implementation to help debug *your* `printf` implementation. It involves temporarily renaming your functions (so that there aren't two functions with the same name). However, you have to be vigilant and make sure you don't get the two mixed up (e.g. if my_printf calls library vsnprintf, you can mistakenly believe your code is working when it really isn't being tested).
