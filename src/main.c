#include <raylib.h>
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

void
draw_display(display_t* disp)
{
  for (int y = 0; y < DISPLAY_H; y++) {
    for (int x = 0; x < DISPLAY_W; x++) {
      if ((*disp)[x + y * DISPLAY_W])
        DrawRectangle(x * DRAWING_SCALE,
                      y * DRAWING_SCALE,
                      DRAWING_SCALE,
                      DRAWING_SCALE,
                      GOLD);
      else
        DrawRectangle(x * DRAWING_SCALE,
                      y * DRAWING_SCALE,
                      DRAWING_SCALE,
                      DRAWING_SCALE,
                      DARKGREEN);
      ;
    }
  }
}

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

    case 0x3:
      opcode_3xnn(&chip8->program_counter,
                  chip8->registers[nibble(3, instruction)],
                  $low_byte(instruction));
      break;

    case 0x4:
      opcode_4xnn(&chip8->program_counter,
                  chip8->registers[nibble(3, instruction)],
                  $low_byte(instruction));
      break;

    case 0x5:
      opcode_5xy0(&chip8->program_counter,
                  chip8->registers[nibble(3, instruction)],
                  chip8->registers[nibble(2, instruction)]);
      break;

    case 0x6:
      opcode_6xnn(
        &chip8->registers, nibble(3, instruction), $low_byte(instruction));
      break;

    case 0x7:
      opcode_7xnn(
        &chip8->registers, nibble(3, instruction), $low_byte(instruction));
      break;

    case 0x8:
      switch (nibble(1, instruction)) {
        case 0x0:
          opcode_8xy0(&chip8->registers[nibble(3, instruction)],
                      chip8->registers[nibble(2, instruction)]);
          break;

        case 0x1:
          opcode_8xy1(&chip8->registers[nibble(3, instruction)],
                      chip8->registers[nibble(2, instruction)]);
          break;

        case 0x2:
          opcode_8xy2(&chip8->registers[nibble(3, instruction)],
                      chip8->registers[nibble(2, instruction)]);
          break;

        case 0x3:
          opcode_8xy3(&chip8->registers[nibble(3, instruction)],
                      chip8->registers[nibble(2, instruction)]);
          break;

        case 0x4:
          opcode_8xy4(&chip8->registers[nibble(3, instruction)],
                      chip8->registers[nibble(2, instruction)],
                      &chip8->registers[0xf]);
          break;

        case 0x5:
          opcode_8xy5(&chip8->registers[nibble(3, instruction)],
                      chip8->registers[nibble(2, instruction)],
                      &chip8->registers[0xf]);
          break;

        case 0x6:
          opcode_8xy6(&chip8->registers[nibble(3, instruction)],
                      chip8->registers[nibble(2, instruction)],
                      &chip8->registers[0xf]);
          break;

        case 0x7:
          opcode_8xy7(&chip8->registers[nibble(3, instruction)],
                      chip8->registers[nibble(2, instruction)],
                      &chip8->registers[0xf]);
          break;

        case 0xe:
          opcode_8xye(&chip8->registers[nibble(3, instruction)],
                      chip8->registers[nibble(2, instruction)],
                      &chip8->registers[0xf]);
          break;
      }
      break;

    case 0x9:
      opcode_9xy0(&chip8->program_counter,
                  chip8->registers[nibble(3, instruction)],
                  chip8->registers[nibble(2, instruction)]);
      break;

    case 0xa:
      opcode_annn(&chip8->index_register, $high_nib_low_byte(instruction));
      break;

    case 0xb:
      opcode_bnnn(&chip8->program_counter,
                  $high_nib_low_byte(instruction),
                  chip8->registers[0x0]);
      break;

    case 0xc:
      opcode_cxnn(&chip8->registers[nibble(3, instruction)],
                  $low_byte(instruction));
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

    case 0xe:
      switch ($low_byte(instruction)) {
        case 0x9e:
          opcode_ex9e(&chip8->program_counter,
                      chip8->registers[nibble(3, instruction)]);
          break;

        case 0xa1:
          opcode_exa1(&chip8->program_counter,
                      chip8->registers[nibble(3, instruction)]);
          break;
      }
      break;

    case 0xf:
      switch ($low_byte(instruction)) {
        case 0x07:
          opcode_fx07(chip8->delay_timer,
                      &chip8->registers[nibble(3, instruction)]);
          break;
        case 0x0a:
          opcode_fx0a(&chip8->program_counter,
                      &chip8->registers[nibble(3, instruction)]);
          break;

        case 0x15:
          opcode_fx15(&chip8->delay_timer,
                      chip8->registers[nibble(3, instruction)]);
          break;

        case 0x18:
          opcode_fx18(&chip8->sound_timer,
                      chip8->registers[nibble(3, instruction)]);
          break;

        case 0x1e:
          opcode_fx1e(&chip8->index_register,
                      chip8->registers[nibble(3, instruction)]);
          break;

        case 0x29:
          opcode_fx29(&chip8->index_register,
                      chip8->registers[nibble(3, instruction)]);
          break;

        case 0x33:
          opcode_fx33(&chip8->memory,
                      chip8->index_register,
                      chip8->registers[nibble(3, instruction)]);
          break;

        case 0x55:
          opcode_fx55(&chip8->memory,
                      &chip8->registers,
                      &chip8->index_register,
                      nibble(3, instruction));
          break;

        case 0x65:
          opcode_fx65(&chip8->memory,
                      &chip8->registers,
                      &chip8->index_register,
                      nibble(3, instruction));
          break;
      }
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

  InitWindow(
    DISPLAY_W * DRAWING_SCALE, DISPLAY_H * DRAWING_SCALE, "mister8 - alpha");
  SetTargetFPS(60);

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

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(DARKGREEN);
    draw_display(&chip8.display);
    EndDrawing();

    uint16_t instruction = fetch(&chip8.memory, &chip8.program_counter);
    decode(&chip8, instruction);
  }

  CloseWindow();
}
