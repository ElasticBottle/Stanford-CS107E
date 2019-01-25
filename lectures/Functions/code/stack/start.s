.global _start
_start:
    mov sp, #0x8000000  // set initial stack pointer
    mov fp, #0          // init frame pointer
    bl _cstart          // call _cstart
hang: b hang        	// after _cstart returns, go into infinite loop
