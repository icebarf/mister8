#include <limits.h>
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>

#include "util.h"

/* utility cast macro */
#define $to_ulong(X) (unsigned long)X

static inline bool
memory_available_for(unsigned long fsize)
{
  unsigned long available_space =
    MEMORY_CAP - BYTES_RESERVED_LOW - BYTES_RESERVED_HIGH;
  return (fsize <= available_space) ? true : false;
}

int
read_file(const char fname[static 1], memory_t dest_memory[static 1])
{
  FILE* rom = fopen(fname, "rb");
  if (rom == NULL) {
    perror(fname);
    return -1;
  }

  /* assume the fseek calls don't fail */
  fseek(rom, 0L, SEEK_END);
  long fsize = ftell(rom);
  fseek(rom, 0L, SEEK_SET);
  switch (fsize) {
    case -1:
      perror(fname);
      return -1;
    case 0:
      fprintf(stderr, "mister8: rom file size too small\n");
      return -1;
      break;
    default:
      break;
  }

  if (!memory_available_for($to_ulong(fsize))) {
    fprintf(stderr, "mister8: rom file too large to store\n");
    return -1;
  }

  size_t read_bytes =
    fread(&(*dest_memory)[PROG_LOAD_ADDRESS], 1, $to_ulong(fsize), rom);
  if (read_bytes != $to_ulong(fsize)) {
    perror(fname);
    return -1;
  }

  fclose(rom);

  /* we know that the read_bytes value is none larger than 0xe000 bytes, or
   * 3584 bytes and is non-negative */
  return (int)read_bytes;
}

void
dump_display(display_t display[static 1])
{
  fprintf(
    stdout,
    "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n");
  for (int y = 0; y < 32; y++) {
    for (int x = 0; x < 64; x++) {
      fprintf(stdout, "%c", (*display)[x + y * DISPLAY_W] == 1 ? '0' : '-');
    }
    fprintf(stdout, "\n");
  }
  fprintf(
    stdout,
    "\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
}

void
dump_memory(const int bytes, const memory_t* mem)
{
  fprintf(stdout,
          "---------------------------MEMORY-------------------------------\n");

  for (int x = 0; x < bytes; x += 6) {
    fprintf(stdout,
            "%3x-%3x\t\t%2x %2x %2x %2x %2x %2x\n",
            PROG_LOAD_ADDRESS + x,
            PROG_LOAD_ADDRESS + x + 5,
            (*mem)[PROG_LOAD_ADDRESS + x],
            (*mem)[PROG_LOAD_ADDRESS + x + 1],
            (*mem)[PROG_LOAD_ADDRESS + x + 2],
            (*mem)[PROG_LOAD_ADDRESS + x + 3],
            (*mem)[PROG_LOAD_ADDRESS + x + 4],
            (*mem)[PROG_LOAD_ADDRESS + x + 5]);
  }
}

/* 0xABCD
 * pos = 1: D
 * pos = 2: C
 * pos = 3: B
 * pos = 4: A
 */
uint8_t
nibble(const uint8_t pos, const uint16_t number)
{
  switch (pos) {
    case 1:
      return number & 0x000f;
    case 2:
      return (number & 0x00f0) >> 4;
    case 3:
      return (number & 0x0f00) >> 8;
    case 4:
      return (uint8_t)((number & 0xf000) >> 12);
    default:
      __builtin_unreachable();
      break;
  }
}

uint8_t
get_random_uint8(void)
{
  return (uint8_t)GetRandomValue(0, UINT8_MAX);
}
