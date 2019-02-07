---
released: true
permalink: /assignments/assign4/
title: 'Assignment 4: Backtrace and Malloc'
toc: true
---
{% comment %}
Task list to copy/paste when creating PR for this assign:

__Before releasing assign4:__
- [ ] Review writeup/starter code (instructor)
- [ ] Consider changes for clarity of spec/ease of grading (TA)
- [ ] Followup on any github issue from previous

__To prep for assign4:__
- [ ] 

{% endcomment %}

*Written by Julie Zelenski*

{% include duedate.html n=4 %}

## Goals
For this week's assignment, you'll again be playing the role of
library implementer to add a debugging aid (`backtrace.c`)
and a heap allocator (`malloc.c`) to your growing
collection of foundational routines. 

After you finish this assignment, you will:

- be able to dissect the runtime stack in an executing program,

- further understand C data structures and memory layout,

- have leveled up your skills wrangling pointers in C, and

- appreciate the complexity and tradeoffs in implementing a heap allocator.

This assignment (and every assignment to follow) will also provide much opportunity for you to reap the benefits of the modules you worked so hard on last week. All hail `printf`!

## Get started
Navigate to your copy of the `cs107e.github.io` courseware repository and do a `git pull` to update to the latest files.

As usual, we already created your `assign4-basic` branch. Navigate to your `assignments` directory and checkout the new branch.

```
$ cd cs107e_home/assignments
$ git fetch origin
$ git checkout assign4-basic
```

Read the `Makefile` for information on how to reuse the modules you have written for previous assignments. 

If reusing your code, verify that your project has up-to-date versions of
your modules `gpio.c`, `timer.c`, `strings.c`, and `printf.c`.  The `git merge` command can be used to incorporate changes from another branch. For example, if you fixed issues in `gpio.c` or `timer.c` as part of an assignment 2 regrade, use `git merge assign2-basic` to merge those changes into your current branch.  If you pushed changes to `assign3-basic`
after your assign4-basic branch was created, use `git merge assign3-basic` to merge those additions now. If your files are out of date and you don't fall
into one of these categories, ask a staff member for help. 

The starter project contains the files `backtrace.c` and
`malloc.c` and the application programs `apps/heap.c` and `tests/test_backtrace_malloc.c`. You will edit `backtrace.c` and `malloc.c` to  implement the required functions. You will also add many tests to `tests/test_backtrace_malloc.c`.  The program `apps/heap.c` is used unchanged as a sample application.

## Basic section

### Backtrace module
The `backtrace` module provides functions to gather a backtrace and print it.

A backtrace is a list of the frames currently on the stack. The backtrace begins at the executing function, traces through the sequence of calls that led to this function, and ends at `_cstart`.

Consider a program whose `main` function calls `print_triangle`, which then calls `printf`. A backtrace initiated during `printf` would look something like this: 
```
#0 0x8250 at printf+60
#1 0x80d0 at print_triangle+128
#2 0x8118 at main+24
#3 0x8160 at _cstart+48
```

The [backtrace.h](https://github.com/cs107e/cs107e.github.io/blob/master/cs107e/include/backtrace.h) header file declares and documents these three functions:

- `int backtrace(frame_t f[], int max_frames)`
-  `void print_frames(frame_t f[], int n)`
- `void print_backtrace(void)`

The two `print_xxx` functions are given to you pre-written. Your task is to implement the `backtrace` function. This function fills the array of frames with information harvested from the stack memory. `backtrace` is a small but mighty function. Completing it will demonstrate your understanding of the stack layout and show off your finest pointer skills.

Before writing code for the `backtrace` function, we recommend that you first spend time poking around in gdb and exploring the runtime state of the program, examining stack memory, stepping through function calls, and digging around to extract the stack frame information, and so on. Exercise 1b of [Lab 3](/labs/lab3/#1b) and exercise 4 of [Lab 4](/labs/lab4/#4-stack) may be worth a second look.

#### An example backtrace


A backtrace is stored as an array of structs, one `frame_t` for each function on the stack. Review
[backtrace.h](https://github.com/cs107e/cs107e.github.io/blob/master/cs107e/include/backtrace.h) 
to see the type definition of the struct `frame_t` and read the comments which explain the intent of the fields `resume_addr` and `resume_offset`.

The backtrace function starts at the innermost stack frame and traces to the outermost, harvesting the information for each stack frame and storing into a `frame_t`.

The current value of the `fp` register is the "anchor" you need to start examining the stack. You must drop down to assembly to access a register. The GCC `__asm__` keyword allows you to embed assembly into 
your C code. The code sample below demonstrates using embedded assembly to store the value of the `fp` register into a variable named `cur_fp`:

   ```
   void *cur_fp;
   __asm__("mov %0, fp" : "=r" (cur_fp));
   ```

(If you're 
   curious to learn more, check out the manual: [gcc support for inline assembly](https://gcc.gnu.org/onlinedocs/gcc/Using-Assembly-Language-with-C.html).)

We are using the `code/simple` program from lab3/lab4 as our example below. Have the
   [Lab 4 stack diagram](/labs/lab4/images/stack_abs.html) handy when working through the following thought questions.

   - The executing function is `abs()`. What is the value of the `fp` register? Relative to the `fp`, where in the stack can you read the `resume_addr` for the caller of `abs()`?

- How can you work from the `fp` for `abs()` to access the saved `fp` of its caller, the `diff` function?  How does the saved `fp` grant you access to the stack frame for `diff`? How do you then go from `diff`'s frame to the frame of its caller, the `main` function? What is the indication that you have hit the outermost stack frame? This is where backtrace stops.

- The `resume_offset` is calculated by subtracting the address of the first instruction of the caller from the resume address in the caller. What information in the stack frame for `diff` helps you locate its first instruction? (Hint: may help to re-visit question 4 on the 
[lab4 check-in](/labs/lab4/checkin/) 
for some foreshadowing about this calculation.)

The final task in harvesting the backtrace is to get the function name. Symbol names are usually not present in a program binary file as functions are referred to by address, not name. However, there is a compiler option to retain function names in the binary to support developers (such as yourself)
writing debugging tools. If you compile with the flag `-mpoke-function-name`,
gcc embeds each function's name into the text section, at the location in memory preceding the function's first instruction.
Below is an excerpt from the gcc documentation
about this option.

{% include callout.html type="danger" %}
The quote below is taken from the gcc manual, but we have corrected a few mis-identified details. Please use this version rather than refer back to the original.

</div>

   > `-mpoke-function-name`
   >
   > Write the name of each function into the text section, directly
   > preceding the function prologue. The generated code is similar to
   > this:
   >
   > ```
   > t0
   >     .ascii "arm_poke_function_name", 0
   >     .align
   > t1
   >     .word 0xff000000 + (t1 - t0)
   > arm_poke_function_name
   >     mov     r12, sp
   >     push    {fp, r12, lr, pc}
   >     sub     fp, r12, #4
   > ```
   >
   > When performing a stack backtrace, code can inspect the value of
   > `pc` stored at `fp + 0`. If the trace function then looks at location
   > `pc - 16` and the top 8 bits are set, then we know that there is a
   > function name embedded immediately preceding this location and has
   > length `((pc[-4]) & ~0xff000000)`.

   How cool! Look at the assign4 `Makefile` and see that we've added
   `-mpoke-function-name` to the `CFLAGS`.

If the word preceding a function's first instruction in memory has `0xff` in its upper byte, this indicates that the function has its name embedded. The name's length is stored in the lower three bytes of the word. Use that length to back up in memory to the start of the string. Neat! If you don't find the magic `0xff` in the right place, this indicates the function name is not available, in which case, use `"???"` as the name.

If we recompile our example program with the
`-mpoke-function-name` flag, we get this [memory diagram compiled with embedded function names](images/diagram_with_poke.html). The text section now includes the function names along the function instructions. Use the memory diagram as a guide, work out at what location you can access the string name for the `main` function.

Each embedded function name is null-terminated and the terminator is
counted in the length. The use of 
`.align` causes the length of the name to be rounded up to a 
multiple of 4; additional null characters are used as padding. 
A function named `binky` will have its name embedded 
as `binky\0\0\0` and length of 8. Given that the name is
stored with its null terminator, you can access the string in-place and directly
store its address into the name field of a `frame_t` without making a copy of it.

Put this all together and you have a function to harvest a backtrace from the current stack contents. It requires surprisingly little code, but it is a dense passage, heavy on the pointer arithmetic and typecasts. Plan out your steps carefully, take it slow, and test as you go. Validating your backtrace against `gdb`'s `backtrace` command can be a useful debugging technique.

### Malloc module
Global and local variables have been sufficient for our program needs thus far, but supporting dynamic allocation will enable more sophisticated applications. Dynamic allocation offers the programmer explicit and 
direct control over sizing and lifetime of storage. With this
power comes with the responsibility to properly allocate and
deallocate that memory. Using dynamic memory correctly and safely is a
challenge for the client; implementing a heap allocator correctly
and efficiently is a challenge for the implementer.

The [malloc.h](https://github.com/cs107e/cs107e.github.io/blob/master/cs107e/include/malloc.h) module declares three functions:

   - `void *malloc(size_t nbytes)`
   - `void free(void *ptr)`
   - `void *realloc(void *ptr, size_t new_size)`

These are the allocator operations of the standard C library,

The starter code provides a trivial sample allocator 
implementation in `malloc.c`. This
"bump allocator" is simple and correct, but fairly naive. 
It is provided only to serve as a reference point 
from which your own
heap allocator will be able to show substantial improvements.
 
The job of the heap allocator is to service allocation requests and manage the pool of available space. The critical requirement for a heap allocator is to correctly service any combination of well-formed requests. It is also desirable that it be space-efficient (recycling freed memory, compactly storing blocks) and fast (quickly servicing requests). For the assignment, your primary goal will be to achieve correctness, with a lesser priority on performance.

#### Step-by-step

1. **Study starter code.**

    Start by reviewing the given code for the bump allocator in the file `malloc.c`.

    Make sure you understand what each line in the bump allocator is
doing. Your allocator will be significantly more complicated, so it is best to start from a solid foundation. Here are some self-test questions to work through to verify your understanding:

    - Where is the heap located in memory? How big can it grow?
    - Why does the code cast pointers to `char *` before doing pointer arithmetic?
    - If you request a 5-byte allocation, how much space is actually reserved by the bump allocator?
    - Every allocation is required to be 8-byte aligned (i.e. payload must start on an address which is a multiple of 8). How does the bump allocator achieve this requirement?
    - Why is the bump allocator unable to recycle a previous allocation that has been freed?
    - Read the man page ( `man realloc`) to learn about the realloc operation. Review the realloc implementation in the bump allocator and confirm your understanding of how and why this code works.

2. **Re-work `malloc()` to add a block header to each allocated block.**
    
    Your heap allocator will use a header on each block that tracks the block size and status (in-use or free).  Review these [block header diagrams](block_headers/) to understand the basic layout of a heap using block headers.

    The existing bump allocator services a `malloc` request by adding a new block onto the end of the heap. Continue with the same strategy for now, but augment the operation to add a block header on the block.

3. **Implement `free()` to update status in block header.**

    `free()` should access the block's header and mark it as free. 
    This doesn't yet recycle a free block, but marking it as free is the
    first step toward that.

4. **Implement `heap_dump()` to output debugging information.**

    `heap_dump()` walks over the blocks in your heap and prints out whatever debugging information you would find helpful. Yay `printf`, where have you been all my life?!  For each block, it might report the size and status from the block header. If you choose to also print the payload of the block, we recommend abbreviating, perhaps displaying only the first 16 bytes. If you instead print out each and every byte, the output can be overwhelming when working with larger heaps. 

    Use the `trace_heap()` test in `tests/test_backtrace_malloc.c` to try out `heap_dump`. Inspecting the dumped output allows you to validate that the contents of your heap are as you expect. Your `heap_dump` routine will not be graded, it is yours to use as you see fit, but the more help it provides to you, the better off you will be.

    You can sprinkle calls to `heap_dump()` wherever needed in your tests to get a visual dump of your heap contents for debugging. 

5. **Upgrade `malloc()` to search freed blocks and reuse/split.**
   
    Now change `malloc` to search for an existing free block of appropriate size to recycle.

    To advance through the blocks in the heap, read the size from the
    block's header and use the size in conjunction with pointer arithmetic to
    move from one block header to the next. Searching the heap for free
    blocks in this way is traversing an _implicit free list_.

    Ideally, you would choose a free block of approximately the right size for the request. However, searching the entire heap to find that best fit block can be time-consuming. A faster approach is to go with _first fit_, i.e. grab the first block found that is large enough. If this block satisfies the request with enough excess leftover to form a second block, split off that remainder into its own free block.

    If no recyclable block is found, then just tack a new block on the heap end as before.

6. **Upgrade `free()` to coalesce adjacent free blocks.**
   
    Reusing single freed blocks is a good improvement, but it doesn't entirely solve the general recycling need. What happens if the client repeatedly allocates 8-byte blocks until the heap is full, frees all of them, then tries to allocate a 32-byte block? The heap contains way more than 32 bytes of free space, but it has been fragmented it into individual 8-byte blocks. We need a way to join those smaller blocks to service larger requests.

    Modify `free()` so that it examines the neighbor block after the newly freed
    block. If that forward neighbor is also free, then _coalesce_ the two
    blocks into one big free block. Then look at the forward neighbor of
    that bigger block and see if it's free in turn. Continue
    forward-coalescing until no further coalescing is possible.

7. **Upgrade `realloc()` to operate in-place when possible.**

    The final feature is to make a smarter `realloc`. The version of `realloc` given in the starter code always copies the payload data from the original block to a new block. It would be much more efficient if we could instead resize the block in place. This won't always be possible, but if the neighboring space is free and can be tacked on to the current block, we can save ourselves a lot of effort.  Modify `realloc()` to seize the opportunity to resize the block in-place and only move the payload data when it must.


__Congratulations!__ You have summited Heap Allocator Mountain ⛰ and earned your Pointer Wrangling Merit Badge 🏆 Time to celebrate with a nice cup of tea ☕️, a moment of mediation 🧘‍♀️, or a decadent nap in the sun ☀️.

#### Malloc specification

All pointers returned by `malloc()` are required be aligned to 8 bytes, the size of the largest primitive type on our system. A convenient way to adhere to the alignment rule is
to round up all requested sizes to a total block size (payload
plus header) that is a multiple of 8, and lay out blocks end to end starting from an address that is a multiple of 8. Our provided `memmap` aligns the `__bss_end__` symbol to an 8-byte boundary.

Read `man malloc` to learn the required handling for error conditions, such as the response if `malloc()` cannot fulfill a request. Four valid, but odd, edge cases to consider are `malloc(0)`, `realloc(NULL, size)`,  `realloc(ptr, 0)` and `free(NULL)`. Read the man page to learn what the expected behavior is for each and follow along with the specification.

If the client request is invalid, such as an attempt to `realloc()` a
random pointer that was not obtained from `malloc()` or `free`ing an already freed pointer, the behavior of the allocator is undefined. We will not test such invalid calls.


 <a name="testing"></a>
## Testing

The `apps/heap.c` program contains sample client code taken from lab that uses `backtrace` and `malloc`. Use this program unchanged as a basic sanity test of your modules.

The `tests/test_backtrace_malloc.c` program is yours to fill with your comprehensive suite of test cases. The starter code provides some example tests to get you started. You will need to add many more tests to throughly exercise your heap allocator in a wide variety of situations. Remember that part of your assignment grade is awarded based on the quality and breadth of your testing.

In CS106B, you learned about a variety of dynamic data structures (linked lists, binary search trees, hash tables) and used them in the implementation of classic abstract data types (vector, stack, queue, dictionary, set, graph). Given your shiny new heap allocator, you can now bring all that wonderful data structure goodness to your Pi.  Porting some of that code to your Pi would make an excellent and rigorous test case for your allocator and in the process, you would gain a library of useful ADTs -- win-win!

## Extension: Mini-Valgrind
The extension for this assignment is to combine your backtrace and malloc modules to add a "Mini-Valgrind" set of memory debugging features to your heap allocator.  

What happens if a client allocates a block of memory and then mistakenly writes outside of the bounds?  The erroneous write not only stomps on the neighboring payload data, it is almost certain to destroy the critical heap housekeeping that is stored adjacent to the payload data. Many sad consequences will follow. 

What happens if a client allocates a block, but 
forgets to free it when done? This creates a _memory leak_,
where the block is parked there forever, unable to be recycled.

Memory debugging tools such as [Valgrind](http://valgrind.org) are invaluable weapons in the battle against these insidious and all-too-common memory errors and leaks. For the extension, you are to implement a "Mini-Valgrind" that adds memory error and leak detection to your heap allocator.

#### Red zones and leak detection

One technique for detecting memory over/underruns is to surround each payload with a pair of _red zones_. When servicing a malloc request, oversize the space by an extra 8 bytes. Place the actual payload in the middle with one 4-byte red zone before it and another 4-byte red zone after it. (Take care to keep the payload aligned on the 8-byte boundary). Assign the red zone a distinctive value. Our implementation uses a repeated `0x7e`, though you are free to use [any distinctive non-offensive value you like](https://en.wikipedia.org/wiki/Hexspeak). When the client later frees that block, check the red zones and squawk loudly if the value has been perturbed.

Use your backtrace module to tag each allocated block with the context it was allocated from. Modify your block header structure to add an array of three `frame_t` and fill it with a snapshot of the backtrace at the time of allocation. 

Modify `free` to verify that the red zones are intact for the block currently being freed. If not, print an error message about the block in error along with its mini-backtrace that identifies where the block was allocated from.

```
=============================================
 **********  Mini-Valgrind Alert  ********** 
=============================================
Attempt to free address 0x0000a978 that has damaged red zone(s): [457e7e7e] [7e7e7e7e]
Block of size 5 bytes, allocated by
#0 0x8d4c at malloc+192
#1 0x8650 at test_heap_redzones+52
#2 0x86bc at main+56
```

Format your error message as shown in the sample above. The values for the address, damaged red zone, block size, etc. will differ based on the test case, but you should otherwise exactly match the wording and format so as to be compatible with our automated checker. 

Mini-Valgrind also tracks heap statistics, such as the count of calls to `malloc` and `free` and the aggregate total number of bytes requested. Implement the function `memory_report` to print a summary of the program's total heap usage and list all remaining memory leaks. The mini-backtrace stored in the block header is used to identify the context for each leaked block.

```
=============================================
         Mini-Valgrind Memory Report         
=============================================

malloc/free: 42 allocs, 40 frees, 5752 bytes allocated

8 bytes are lost, allocated by
#0 0x8d0c at malloc+196
#1 0x8674 at main+28
#2 0x86ec at _cstart+48

100 bytes are lost, allocated by
#0 0x8d0c at malloc+196
#1 0x868c at main+52
#2 0x86ec at _cstart+48

Lost 108 total bytes in 2 blocks. 
```
The format of your memory report should exactly match the wording and format of the sample report shown above.

As a final detail, work out where you can call `memory_report()` so that Mini-Valgrind can provide leak detection to any program, without modifying the application's
`main` or other application files? Hint: think about how the green LED 
turns on when your test program runs to successful completion.

## Submit and automated checks
Submit the finished version of your assignment by making a git "pull request". The deliverables for `assign4-basic` are:

  - Implementation of the `backtrace.c` and `malloc.c` modules
  - Your comprehensive tests for all backtrace and malloc functions in `tests/test_backtrace_malloc.c`

The automated checks make sure that we can run your C
code and test and grade it properly, including swapping your tests for
ours.

CI verifies that:

- `apps/heap.c` is unchanged

- `make` completes successfully

- `make test` completes successfully using your `tests/test_backtrace_malloc.c`

- `make test` completes successfully using the original `tests/test_backtrace_malloc.c` from
  the starter code (we will swap this in before running it)

If the automated test fails on your final submission, we will
automatically deduct 1 point from your basic grade.  Also remember
that we will deduct half a point for any compiler warnings that are
generated when compiling your code.

## Heap alligators
At home, my chatter about "heap allocators" was mistaken for "heap alligators" by my young kids, who were alarmed that I would ask my students to wrestle with such fearsome beasts. 

A family trip to Gatorland taught us that a heap __of__ alligators can actually be quite cute and the adorable drawing from Jane Lange (talented CS107e alum and former section leader) demonstrates that a tamed heap alligator makes a lovely pet. May you, too, tame your heap alligator and triumph over the perils of dynamic allocation.

[<img title="Heap of Alligators" src="images/heap_of_alligators.jpg" width="50%" style="display:inline;">](images/heap_of_alligators.jpg)[<img title="Heap Alligator" src="images/heapalligator.png" width="50%" style="float:right;">](images/heapalligator.png)



