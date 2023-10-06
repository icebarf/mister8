#include "instructions.h"
#include "chip8.h"

#include <string.h>

/* clear display */
inline void
opcode_00e0(display_t* display)
{
  memset(*display, 0, sizeof(*display));
}

/* ret */
inline void
opcode_00ee(uint16_t* pc, stack_t* stack, uint8_t* stack_counter)
{
  *pc = $pop(stack, stack_counter);
}

/* jump to dest (nnn) */
inline void
opcode_1nnn(uint16_t* pc, uint16_t dest)
{
  *pc = dest;
}

/* call */
inline void
opcode_2nnn(uint16_t* pc, stack_t* stack, uint8_t* stack_counter, uint16_t dest)
{
  $push(stack, stack_counter, *pc);
  *pc = dest;
}

/* set register to value */
inline void
opcode_6xnn(registers_t* registers, uint8_t reg, uint8_t val)
{
  if (reg >= REGISTER_COUNT)
    __builtin_unreachable();
  (*registers)[reg] = val;
}

/* add value to register */
inline void
opcode_7xnn(registers_t* registers, uint8_t reg, uint8_t val)
{
  if (reg >= REGISTER_COUNT)
    __builtin_unreachable();
  (*registers)[reg] += val;
}

/* set index register to address */
inline void
opcode_annn(uint16_t* index, uint16_t address)
{
  *index = address;
}

/* draw sprite to screen */
inline void
opcode_dxyn(memory_t* mem,
            display_t* disp,
            registers_t* registers,
            uint8_t vx,
            uint8_t vy,
            uint8_t vidx,
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
