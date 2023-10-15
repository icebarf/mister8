#include <stdio.h>
#include <stdlib.h>

#include "chip8.h"
#include "instructions.h"
#include "util.h"

// clang-format off
#define font \
{ \
0xF0, 0x90, 0x90, 0x90, 0xF0,\
0x20, 0x60, 0x20, 0x20, 0x70,\
0xF0, 0x10, 0xF0, 0x80, 0xF0,\
0xF0, 0x10, 0xF0, 0x10, 0xF0,\
0x90, 0x90, 0xF0, 0x10, 0x10,\
0xF0, 0x80, 0xF0, 0x10, 0xF0,\
0xF0, 0x80, 0xF0, 0x90, 0xF0,\
0xF0, 0x10, 0x20, 0x40, 0x40,\
0xF0, 0x90, 0xF0, 0x90, 0xF0,\
0xF0, 0x90, 0xF0, 0x10, 0xF0,\
0xF0, 0x90, 0xF0, 0x90, 0x90,\
0xE0, 0x90, 0xE0, 0x90, 0xE0,\
0xF0, 0x80, 0x80, 0x80, 0xF0,\
0xE0, 0x90, 0x90, 0x90, 0xE0,\
0xF0, 0x80, 0xF0, 0x80, 0xF0,\
0xF0, 0x80, 0xF0, 0x80, 0x80 \
}
// clang-format on

inline uint16_t
fetch(memory_t* memory, uint16_t* pc)
{
  uint16_t inst = (uint16_t)((*memory)[*pc] << 8 | (*memory)[*pc + 1]);
  *pc += 2;
  return inst;
}

void
decode(struct system* chip8, uint16_t instruction)
{
  switch (nibble(4, instruction)) {
    case 0x0:
      switch ($low_byte(instruction)) {
        case 0xe0:
          opcode_00e0(&chip8->display);
          break;

        case 0xee:
          opcode_00ee(
            &chip8->program_counter, &chip8->stack, &chip8->stack_counter);
          break;
      }
      break;

    case 0x1:
      opcode_1nnn(&chip8->program_counter, $high_nib_low_byte(instruction));
      break;

    case 0x2:
      opcode_2nnn(&chip8->program_counter,
                  &chip8->stack,
                  &chip8->stack_counter,
                  $high_nib_low_byte(instruction));
      break;

    case 0x6:
      opcode_6xnn(
        &chip8->registers, nibble(3, instruction), $low_byte(instruction));
      break;

    case 0x7:
      opcode_7xnn(
        &chip8->registers, nibble(3, instruction), $low_byte(instruction));
      break;

    case 0xa:
      opcode_annn(&chip8->index_register, $high_nib_low_byte(instruction));
      break;

    case 0xd:
      opcode_dxyn(&chip8->memory,
                  &chip8->display,
                  &chip8->registers,
                  chip8->index_register,
                  nibble(3, instruction),
                  nibble(2, instruction),
                  nibble(1, instruction));
      break;
    default:
      fprintf(stderr,
              "Unrecognised instruction: 0x%4x\nProgram Counter for "
              "Instruction: 0x%4x\n",
              instruction,
              chip8->program_counter - 2);
      exit(1);
  }
}

int
main(int argc, char** argv)
{
  if (argc != 2) {
    fprintf(stdout, "./mister8 romfile\n");
    return 0;
  }

  struct system chip8 = {
    .display = { 0 },
    .memory = font,
    .stack = { 0 },
    .registers = { 0 },
    .index_register = 0,
    .program_counter = PROG_LOAD_ADDRESS,
    .delay_timer = 0,
    .sound_timer = 0,
    .stack_counter = 0,
  };

  int read_bytes = read_file(argv[1], &chip8.memory);
  if (read_bytes <= 0)
    return 1;
  fprintf(stdout, "File: %s\nBytes Read: %i\n", argv[1], read_bytes);
  dump_memory(read_bytes, &chip8.memory);

  while (1) {
    uint16_t instruction = fetch(&chip8.memory, &chip8.program_counter);
    decode(&chip8, instruction);
    dump_display(&chip8.display);
  }
}
