#ifndef MISTER8_CHIP8_H
#define MISTER8_CHIP8_H

#include <stdint.h>

enum Constants
{
  MEMORY_CAP = 4096,
  DISPLAY_W = 64,
  DISPLAY_H = 32,
  STACK_SIZE = 48,
  REGISTER_COUNT = 16,
  MEMORY_RESERVED = 0x200,
  PROG_LOAD_ADDRESS = 0x200,
};

#ifndef MISTER8_DONT_WANT_SYSTEM
struct __attribute__((packed)) system
{
  uint8_t display[DISPLAY_W * DISPLAY_H];
  uint8_t memory[MEMORY_CAP];
  uint16_t stack[STACK_SIZE];
  uint8_t registers[REGISTER_COUNT];
  uint16_t index_register;
  uint16_t program_counter;
  uint8_t delay_timer;
  uint8_t sound_timer;
};
#endif

#endif
