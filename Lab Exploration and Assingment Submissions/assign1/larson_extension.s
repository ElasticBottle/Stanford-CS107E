.equ DELAY, 0x00F000
.equ MOVE_RIGHT, 0x4
.equ MOVE_LEFT, 0x4
.equ RIGHT_MOST_BULB, 0x8000000
.equ LEFT_MOST_BULB, 0x100000
.equ GPIO27, 0x200000
.equ TRAILING_BULB_1_ON, 0x2
.equ TRAILING_BULB_2_ON, 0x1
.equ TRAIL_1_DELAY, 0x3
.equ TRAIL_2_DELAY, 0x6

// Registers needed to create output numbers for GPIO Pins
mov r1, #1
mov r2, #1

// configure GPIO 20 - 23 for output
setOutput$:
lsl r1, r1, #3
add r2, r2, r1
cmp r1, #GPIO27
blt setOutput$

// Storing output settings into FSEL2 to configure GPIO pin 20 - 23
ldr r0, FSEL2
str r2, [r0]

// set bit 20. LED attached to GPIO pin 20 blinks first 
mov r1, #(1<<20)

// Indicates flash going right
mov r3, #MOVE_RIGHT

// Set trailing fade lights to be none at the start
mov r4, #0
mov r5, #0

loop:

// set GPIO PIN high
ldr r0, SET0
str r1, [r0] 

// delay
mov r2, #DELAY
wait1$:
    subs r2, #1
    beq wait1Fin$

    // Pulse Wave Modulation of trailing bulbs 1 and 2
    mov r6, #TRAIL_1_DELAY
    tst r3, #TRAILING_BULB_1_ON
    bne turnOffTrail1$
    beq turnOnTrail1$
    
    bulb2$:
    mov r6, #TRAIL_2_DELAY
    tst r3, #TRAILING_BULB_2_ON
    bne turnOffTrail2$
    beq turnOnTrail2$
    
    // Turns the 1st trailing bulb on or off. Has a duty cycle of about 0.5
    turnOnTrail1$:
        ldr r0, SET0
        str r4, [r0]
        subs r6, #1
        bne turnOnTrail1$
        eor r3, #TRAILING_BULB_1_ON
        b bulb2$
    turnOffTrail1$:
        ldr r0, CLR0
        str r4, [r0]
        
        eor r3, #TRAILING_BULB_1_ON
        b bulb2$

    // Turns the 2nd trailing bulb on or off. Has a duty cycle of about 0.2
    turnOnTrail2$:
        ldr r0, SET0
        str r5, [r0]
        
        eor r3, #TRAILING_BULB_2_ON
        b wait1$
    turnOffTrail2$:
        ldr r0, CLR0
        str r5, [r0]
        subs r6, #1
        bne turnOffTrail2$
        eor r3, #TRAILING_BULB_2_ON
        b wait1$

wait1Fin$:
// set GPIO PIN low
ldr r0, CLR0
str r1, [r0] 
str r4, [r0]
str r5, [r0]


// Iterate on to the next pin if not at the right most bulb
tst r3, #MOVE_RIGHT
bne goRight$

// Right shifts the bit to blink bulb on left of current bulb on next loop
goLeft$:
    // Setting trailing bulbs for fade
    mov r5, r4
    mov r4, r1

    // Setting the main bulb
    lsr r1, r1, #1

    // Ensuring that the main bulb is not the 2nd trailing bulb. (Happens when the penultimate bulb is heading away from the nearest end)
    cmp r5, r1
    bne notEqualLeft$
    mov r5, #0

    notEqualLeft$:
    // Preventing main bulb to go past the left most bulb
    cmp r1, #LEFT_MOST_BULB
    bne loop
    orr r3, #MOVE_RIGHT
    b loop

// left shifts the bit to blink bulb on the right of current bulb on next loop
goRight$:
    // Setting trailing bulbs for fade
    mov r5, r4
    mov r4, r1
    
    // Setting the main bulb
    lsl r1, r1, #1
    
    // Ensuring that the main bulb is not the 2nd trailing bulb. (Happens when the penultimate bulb is heading away from the nearest end)
    cmp r1, r5
    bne notEqualRight$
    mov r5, #0

    notEqualRight$:
    // Preventing main bulb to go past the right most bulb
    cmp r1, #RIGHT_MOST_BULB
    bne loop
    eor r3, #MOVE_LEFT
    b loop

FSEL0: .word 0x20200000
FSEL1: .word 0x20200004
FSEL2: .word 0x20200008
SET0:  .word 0x2020001C
SET1:  .word 0x20200020
CLR0:  .word 0x20200028
CLR1:  .word 0x2020002C


