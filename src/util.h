#ifndef MISTER8_UTIL_H
#define MISTER8_UTIL_H

#define MISTER8_DONT_WANT_SYSTEM
#include "chip8.h"

/* we want the type information to be retained for the memory parameter */
typedef uint8_t(memory_t)[DISPLAY_W * DISPLAY_H];

int
read_file(const char fname[static 1], memory_t dest_memory[static 1]);

#endif
