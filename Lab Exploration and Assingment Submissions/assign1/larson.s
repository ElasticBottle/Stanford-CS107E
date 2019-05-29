.equ DELAY, 0x3F0000

// configure GPIO 20,21,22,23 for output

mov r1, #1 // Initialising r1 to hold value 1
setOutput$:
    ldr r0, FSEL2
    str r1, [r0]
    lsl r1, #3
    cmp r1, GPIOOUTPUT23
    bne setOutput$

mov r1, #(1<<20)

loop: 

// set GPIO 20 high
ldr r0, SET0
str r1, [r0] 

// delay
mov r2, #DELAY
wait1:
    subs r2, #1
    bne wait1

// set GPIO 20 low
ldr r0, CLR0
str r1, [r0] 

// delay
mov r2, #DELAY
wait2:
    subs r2, #1
    bne wait2

b loop

FSEL0: .word 0x20200000
FSEL1: .word 0x20200004
FSEL2: .word 0x20200008
GPIOOUTPUT23: .word 0b1000000000
SET0:  .word 0x2020001C
SET1:  .word 0x20200020
CLR0:  .word 0x20200028
CLR1:  .word 0x2020002C
