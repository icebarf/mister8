#ifndef MISTER8_CHIP8_H
#define MISTER8_CHIP8_H

#include <stdbool.h>
#include <stdint.h>

enum Constants
{
  MEMORY_CAP = 4096,
  DISPLAY_W = 64,
  DISPLAY_H = 32,
  STACK_SIZE = 48,
  STACK_COUNTER_MAX = 47,
  REGISTER_COUNT = 16,
  BYTES_RESERVED_LOW = 512,
  PROG_LOAD_ADDRESS = 0x200,
  DRAWING_SCALE = 8,
  INSTRUCTIONS_PER_FRAME = 16, // ipf - instructions to execute per frame.
  TIMER_DECREMENT_VALUE = 1, // decrement timer by this value per framce. Should
                             // be 1 if trying to maintain 60 frames per second.
};

#ifndef MISTER8_DONT_WANT_SYSTEM
struct system
{
  uint8_t display[DISPLAY_W * DISPLAY_H];
  uint8_t memory[MEMORY_CAP];
  uint16_t stack[STACK_SIZE];
  uint8_t registers[REGISTER_COUNT];
  uint16_t index_register;
  uint16_t program_counter;
  uint8_t delay_timer;
  uint8_t sound_timer;
  uint8_t stack_counter;
  bool display_modified;
};
#endif

#endif
