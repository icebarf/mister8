#ifndef MISTER8_INSTRUCTIONS_H
#define MISTER8_INSTRUCTIONS_H

#include "chip8.h"
#include "util.h"

#include <string.h>

/* see: https://en.wikipedia.org/wiki/Inline_function#C99
 * rationale behind having 'static inline' functions defined in header file:
 * even though it may cause duplicate code to be generated,
 * this file will only be included in main.c and then each of
 * the inline functions shall only be used/called once
 */

/* clear display */
static inline void
opcode_00e0(display_t* display)
{
  memset(*display, 0, sizeof(*display));
}

/* ret */
static inline void
opcode_00ee(uint16_t* pc, stack_t* stack, uint8_t* stack_counter)
{
  *pc = $pop(stack, stack_counter);
}

/* jump to dest (nnn) */
static inline void
opcode_1nnn(uint16_t* pc, uint16_t dest)
{
  *pc = dest;
}

/* call */
static inline void
opcode_2nnn(uint16_t* pc, stack_t* stack, uint8_t* stack_counter, uint16_t dest)
{
  $push(stack, stack_counter, *pc);
  *pc = dest;
}

/* skip instruction if vx == byte */
static inline void
opcode_3xnn(uint16_t* pc, uint8_t vx, uint8_t byte)
{
  if (vx == byte)
    *pc += 2;
}

/* skip instruction if vx != byte */
static inline void
opcode_4xnn(uint16_t* pc, uint8_t vx, uint8_t byte)
{
  if (vx != byte)
    *pc += 2;
}

/* skip instruction if vx == vy */
static inline void
opcode_5xy0(uint16_t* pc, uint8_t vx, uint8_t vy)
{
  if (vx == vy)
    *pc += 2;
}

/* set register to value */
static inline void
opcode_6xnn(registers_t* registers, uint8_t reg, uint8_t val)
{
  if (reg >= REGISTER_COUNT)
    __builtin_unreachable();
  (*registers)[reg] = val;
}

/* add value to register */
static inline void
opcode_7xnn(registers_t* registers, uint8_t reg, uint8_t val)
{
  if (reg >= REGISTER_COUNT)
    __builtin_unreachable();
  (*registers)[reg] += val;
}

/* store vy in vx */
static inline void
opcode_8xy0(uint8_t* vx, uint8_t vy)
{
  *vx = vy;
}

/* set vx to vx OR vy */
static inline void
opcode_8xy1(uint8_t* vx, uint8_t vy)
{
  *vx = *vx | vy;
}

/* set vx to vx AND vy */
static inline void
opcode_8xy2(uint8_t* vx, uint8_t vy)
{
  *vx = *vx & vy;
}

/* set vx to vx XOR vy */
static inline void
opcode_8xy3(uint8_t* vx, uint8_t vy)
{
  *vx = *vx ^ vy;
}

/* add vx to vy, set vf if carry occurs */
static inline void
opcode_8xy4(uint8_t* vx, uint8_t vy, uint8_t* vf)
{
  *vf = vy > (UINT8_MAX - *vx);
  *vx += vy;
}

/* sub vy from vx, set vf if borrow does not occur */
static inline void
opcode_8xy5(uint8_t* vx, uint8_t vy, uint8_t* vf)
{
  *vf = 1;
  if (*vx < vy)
    *vf = 0;

  *vx -= vy;
}

/* set vx to vy right shift 1, set vf to least significant bit before shift */
static inline void
opcode_8xvy6(uint8_t* vx, uint8_t vy, uint8_t* vf)
{
  *vf = (vy & 0x01);
  *vx = (uint8_t)(vy >> 1);
}

/* sub vx from vy, set vf if borrow does not occur */
static inline void
opcode_8xy7(uint8_t* vx, uint8_t vy, uint8_t* vf)
{
  *vf = 1;
  if (vy < *vx)
    *vf = 0;

  *vx = vy - *vx;
}

/* set vx to vy left shift 1, set vf to most significant bit before shift */
static inline void
opcode_8xvye(uint8_t* vx, uint8_t vy, uint8_t* vf)
{
  *vf = (vy & 0x80);
  *vx = (uint8_t)(vy << 1);
}

/* skip instruction if vx != vy */
static inline void
opcode_9xy0(uint16_t* pc, uint8_t vx, uint8_t vy)
{
  if (vx != vy)
    *pc += 2;
}

/* set index register to address */
static inline void
opcode_annn(uint16_t* index, uint16_t address)
{
  *index = address;
}

/* jump to address + v0 */
static inline void
opcode_bnnn(uint16_t* pc, uint16_t addr, uint8_t v0);

/* draw sprite to screen */
static inline void
opcode_dxyn(memory_t* mem,
            display_t* disp,
            registers_t* registers,
            uint16_t vidx,
            uint8_t vx,
            uint8_t vy,
            uint8_t n)
{
  uint8_t x = (*registers)[vx] & 63;
  uint8_t y = (*registers)[vy] & 31;
  (*registers)[0x0f] = 0;

  for (int row = 0; row < n; row++) {
    uint8_t sprite = (*mem)[vidx + row];
    for (int col = 0; col < 8; col++) {
      if (sprite & (0x80 >> col)) {
        if (((*disp)[(x + col) + ((y + row) * DISPLAY_W)]))
          (*registers)[0x0f] = 1;

        (*disp)[(x + col) + ((y + row) * DISPLAY_W)] ^= 1;
      }
    }
  }
}
#endif
