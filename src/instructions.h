#ifndef MISTER8_INSTRUCTIONS_H
#define MISTER8_INSTRUCTIONS_H

#include "chip8.h"
#include "input.h"
#include "util.h"

#include <stdbool.h>
#include <stdlib.h>
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
opcode_00ee(uint16_t* pc, const stack_t* stack, uint8_t* stack_counter)
{
  *pc = $pop(stack, stack_counter);
}

/* jump to dest (nnn) */
static inline void
opcode_1nnn(uint16_t* pc, const uint16_t dest)
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
opcode_3xnn(uint16_t* pc, const uint8_t vx, const uint8_t byte)
{
  if (vx == byte)
    *pc += 2;
}

/* skip instruction if vx != byte */
static inline void
opcode_4xnn(uint16_t* pc, const uint8_t vx, const uint8_t byte)
{
  if (vx != byte)
    *pc += 2;
}

/* skip instruction if vx == vy */
static inline void
opcode_5xy0(uint16_t* pc, const uint8_t vx, const uint8_t vy)
{
  if (vx == vy)
    *pc += 2;
}

/* set register to value */
static inline void
opcode_6xnn(registers_t* registers, const uint8_t reg, const uint8_t val)
{
  if (reg >= REGISTER_COUNT)
    __builtin_unreachable();
  (*registers)[reg] = val;
}

/* add value to register */
static inline void
opcode_7xnn(registers_t* registers, const uint8_t reg, const uint8_t val)
{
  if (reg >= REGISTER_COUNT)
    __builtin_unreachable();
  (*registers)[reg] += val;
}

/* store vy in vx */
static inline void
opcode_8xy0(uint8_t* vx, const uint8_t vy)
{
  *vx = vy;
}

/* set vx to vx OR vy */
static inline void
opcode_8xy1(uint8_t* vx, const uint8_t vy)
{
  *vx = *vx | vy;
}

/* set vx to vx AND vy */
static inline void
opcode_8xy2(uint8_t* vx, const uint8_t vy)
{
  *vx = *vx & vy;
}

/* set vx to vx XOR vy */
static inline void
opcode_8xy3(uint8_t* vx, const uint8_t vy)
{
  *vx = *vx ^ vy;
}

/* add vx to vy, set vf if carry occurs */
static inline void
opcode_8xy4(uint8_t* vx, const uint8_t vy, uint8_t* vf)
{
  uint8_t flag =
    vy > (UINT8_MAX - *vx); // sum of individual components should not be
                            // larger than the MAX value supported. Thus,
                            // one individual component being larger than
                            // MAX - other component, implies to us that
                            // there will be a carry or an overflow.
  *vx += vy;
  *vf = flag;
}

/* vx = vx - vy , set vf if borrow does not occur */
static inline void
opcode_8xy5(uint8_t* vx, const uint8_t vy, uint8_t* vf)
{
  uint8_t flag = 1;
  if (*vx < vy) // simple elementary school math
    flag = 0;

  *vx -= vy;
  *vf = flag;
}

/* set vx to vy right shift 1, set vf to least significant bit before shift */
static inline void
opcode_8xy6(uint8_t* vx, const uint8_t vy, uint8_t* vf)
{
  uint8_t flag = (vy & 0x01);
  *vx = (uint8_t)(vy >> 1);
  *vf = flag;
}

/* sub vx from vy, set vf if borrow does not occur */
static inline void
opcode_8xy7(uint8_t* vx, const uint8_t vy, uint8_t* vf)
{
  uint8_t flag = 1;
  if (vy < *vx)
    flag = 0;

  *vx = vy - *vx;
  *vf = flag;
}

/* set vx to vy left shift 1, set vf to most significant bit before shift */
static inline void
opcode_8xye(uint8_t* vx, const uint8_t vy, uint8_t* vf)
{
  uint8_t flag = (vy & 0x80) >> 7;
  *vx = (uint8_t)(vy << 1);
  *vf = flag;
}

/* skip instruction if vx != vy */
static inline void
opcode_9xy0(uint16_t* pc, const uint8_t vx, const uint8_t vy)
{
  if (vx != vy)
    *pc += 2;
}

/* set index register to address */
static inline void
opcode_annn(uint16_t* index, const uint16_t address)
{
  *index = address;
}

/* jump to address + v0 */
static inline void
opcode_bnnn(uint16_t* pc, const uint16_t addr, const uint8_t v0)
{
  *pc = addr + v0;
}

/* set vx to random number masked with byte
 * we're just gonna use C's shitty rand() prng
 */
static inline void
opcode_cxnn(uint8_t* vx, const uint8_t byte)
{
  *vx = get_random_uint8() & byte;
}

/* draw sprite to screen */
static inline void
opcode_dxyn(memory_t* mem,
            display_t* disp,
            registers_t* registers,
            const uint16_t vidx,
            const uint8_t vx,
            const uint8_t vy,
            const uint8_t n,
            bool* modified)
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

      if (col + y > DISPLAY_H)
        break;
    }

    if (row + x > DISPLAY_W)
      break;
  }
  *modified = true;
}

/* skip instruction if hex value of key in vx is pressed (down) */
static inline void
opcode_ex9e(uint16_t* pc, const uint8_t key)
{
  if (!is_key_released(key))
    *pc += 2;
}

/* skip instruction if hex value of key in vx is not pressed (up) */
static inline void
opcode_exa1(uint16_t* pc, const uint8_t key)
{
  if (is_key_released(key))
    *pc += 2;
}

/* store delay timer in vx */
static inline void
opcode_fx07(const uint8_t delay_timer, uint8_t* vx)
{
  *vx = delay_timer;
}

/* wait for key press and store the result in vx */
static inline void
opcode_fx0a(uint16_t* pc, uint8_t* vx)
{
  *pc -= 2;
  uint8_t key = get_keypress();
  if (key != INVALID_KEY) {
    *pc += 2;
    *vx = key;
  }
}

/* set delay timer to vx */
static inline void
opcode_fx15(uint8_t* delay_timer, const uint8_t vx)
{
  *delay_timer = vx;
}

/* set sound timer to vx */
static inline void
opcode_fx18(uint8_t* sound_timer, const uint8_t vx)
{
  *sound_timer = vx;
}

/* vx = vx + index register */
static inline void
opcode_fx1e(uint16_t* index, const uint8_t vx)
{
  *index += vx;
}

/* load font sprite address in index register */
static inline void
opcode_fx29(uint16_t* index, const uint8_t vx)
{
  /* mister8 implementation loads font at address 0x0000.
   * each font sprite is 5 bytes, so each sprite is at an adress
   * which is a multiple of 5 */
  *index = 5 * (vx & 0x0f); // original cosmic vip behavior
                            // consider character as lower
                            // nibble of value in vx.
}

/* store BCD representation of value in vx at I, I+1, I+2 */
static inline void
opcode_fx33(memory_t* mem, const uint16_t index, const uint8_t vx)
{
  (*mem)[index] = vx / 100;             // Hundreds : 234 / 100 = 1
  (*mem)[index + 1] = ((vx / 10) % 10); // Tens     : (234/10) = 23, 23 % 10 = 3
  (*mem)[index + 2] = vx % 10;          // Ones     : 234 % 10  = 4
}

/* store values from v0 to vX (inclusive) at mem[index] to mem[index + X].
 * Index is set to index + X + 1*/
static inline void
opcode_fx55(memory_t* mem, registers_t* reg, uint16_t* index, uint8_t x)
{
  x++;
  memcpy(&((*mem)[*index]), &(*reg)[0], x);
  *index += x;
}

/* store values from mem[index] to mem[index + X] at v0 to vX (inclusive)
 * Index is set to index + X + 1*/
static inline void
opcode_fx65(memory_t* mem, registers_t* reg, uint16_t* index, uint8_t x)
{
  x++;
  memcpy(&(*reg)[0], &((*mem)[*index]), x);
  *index += x;
}

#endif
