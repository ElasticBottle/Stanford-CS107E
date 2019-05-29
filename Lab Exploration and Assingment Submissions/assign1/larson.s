.equ DELAY, 0x9F0000
.equ MOVE_RIGHT, 0
.equ MOVE_LEFT, 1
.equ RIGHT_MOST_BULB, 0x800000
.equ LEFT_MOST_BULB, 0x100000

// Registers needed to create output numbers for GPIO Pins
mov r1, #1
mov r2, #1

// configure GPIO 20 - 23 for output
setOutput$:
lsl r1, r1, #3
add r2, r2, r1
cmp r2, #0x100
blt setOutput$

// Storing output settings into FSEL2 to configure GPIO pin 20 - 23
ldr r0, FSEL2
str r2, [r0]

// set bit 20. LED attached to GPIO pin 20 blinks first 
mov r1, #(1<<20)

// Indicates flash going right
mov r3, #MOVE_RIGHT

loop:

// set GPIO PIN high
ldr r0, SET0
str r1, [r0] 

// delay
mov r2, #DELAY
wait1:
    subs r2, #1
    bne wait1

// set GPIO PIN low
ldr r0, CLR0
str r1, [r0] 

// Iterate to on the next pin if not at the right most
cmp r3, #MOVE_RIGHT
beq goRight$

// Right shifts the bit to blink bulb on left of current bulb on next loop
goLeft$:
    lsr r1, r1, #1
    cmp r1, #LEFT_MOST_BULB
    bne loop
    mov r3, #MOVE_RIGHT
    b loop

// left shifts the bit to blink bulb on the right of current bul on next loop
goRight$:
    lsl r1, r1, #1
    cmp r1, #RIGHT_MOST_BULB
    bne loop
    mov r3, #MOVE_LEFT
    b loop

FSEL0: .word 0x20200000
FSEL1: .word 0x20200004
FSEL2: .word 0x20200008
SET0:  .word 0x2020001C
SET1:  .word 0x20200020
CLR0:  .word 0x20200028
CLR1:  .word 0x2020002C
GPIO23: .word 0x200

