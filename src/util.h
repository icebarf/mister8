#ifndef MISTER8_UTIL_H
#define MISTER8_UTIL_H

#define MISTER8_DONT_WANT_SYSTEM
#include "chip8.h"

/* we want the type information to be retained for a few parameters
 * typedef is an easy way to not make declarations look messy*/
typedef uint8_t(memory_t)[MEMORY_CAP];
typedef uint8_t(display_t)[DISPLAY_W * DISPLAY_H];
typedef uint16_t(stack_t)[STACK_SIZE];
typedef uint8_t(registers_t)[REGISTER_COUNT];

int
read_file(const char fname[static 1], memory_t dest_memory[static 1]);

/* opcode utils */

/* 0xABCD
 * pos = 1: D
 * pos = 2: C
 * pos = 3: B
 * pos = 4: A
 */
uint8_t
nibble(uint8_t pos, uint16_t number);

/* 0xABCD
 * low_byte: 0xCD
 * high_byte: 0xAB
 */
#define $low_byte(X) (X & 0x00ff)

#define $high_byte(X) ((X & 0xff00) >> 8)

/* 0xABCD
 * high byte's first nibble and low byte combined
 */
#define $high_nib_low_byte(X) (X & 0x0fff)

/* stack utils */
#define $pop(stack_p, stack_cntr_p) (*stack)[(*stack_cntr_p)--]

#define $push(stack_p, stack_cntr_p, val) ((*stack)[++(*stack_cntr_p)] = val)

#endif
