#ifndef MISTER8_INSTRUCTIONS_H
#define MISTER8_INSTRUCTIONS_H

#include "util.h"

inline void
opcode_00e0(display_t* display);

inline void
opcode_00ee(uint16_t* pc, stack_t* stack, uint8_t* stack_counter);

inline void
opcode_1nnn(uint16_t* pc, uint16_t dest);

inline void
opcode_2nnn(uint16_t* pc,
            stack_t* stack,
            uint8_t* stack_counter,
            uint16_t dest);

inline void
opcode_6xnn(registers_t* registers, uint8_t reg, uint8_t val);

inline void
opcode_7xnn(registers_t* registers, uint8_t reg, uint8_t val);

inline void
opcode_annn(uint16_t* index, uint16_t address);

inline void
opcode_dxyn(memory_t* mem,
            display_t* disp,
            registers_t* registers,
            uint8_t vx,
            uint8_t vy,
            uint8_t vidx,
            uint8_t n);

#endif
