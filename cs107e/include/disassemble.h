#ifndef DISASSEMBLE_H
#define DISASSEMBLE_H

/*
 * Function to decode an ARM instruction into a string,
 * used in the non-standard %pI extension to printf.
 *
 * Utility functions to get embedded function names from
 * text section if compiled -mpoke-function-name
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 * Sun Feb 10 09:46:11 PST 2019
 */


#include <stddef.h>
#include <stdint.h>

void insn_to_str(char *buf, size_t bufsize, uint32_t *addr);
unsigned int *find_symbol(const char *name);
const char *name_for_addr(unsigned int *addr);

#endif