#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

#include "util.h"

/* utility cast macro */
#define TO_ULONG(X) (unsigned long)X

static inline bool
memory_available_for(unsigned long fsize)
{
  unsigned long available_space = MEMORY_CAP - MEMORY_RESERVED;
  return (fsize <= available_space) ? true : false;
}

int
read_file(const char fname[static 1], memory_t dest_memory[static 1])
{
  FILE* rom = fopen(fname, "rb");
  if (rom == NULL) {
    perror(fname);
    return 1;
  }

  /* assume the fseek() doesn't fail */
  fseek(rom, 0L, SEEK_END);
  long fsize = ftell(rom);
  fseek(rom, 0L, SEEK_SET);
  switch (fsize) {
    case -1:
      perror(fname);
      return 1;
    case 0:
      fprintf(stderr, "mister8: rom file size too small\n");
      return 1;
      break;
    default:
      break;
  }

  if (!memory_available_for(TO_ULONG(fsize))) {
    fprintf(stderr, "mister8: rom file too large to store\n");
    return 1;
  }

  if (fread(
        &(*dest_memory)[PROG_LOAD_ADDRESS], CHAR_BIT, TO_ULONG(fsize), rom) !=
      TO_ULONG(fsize)) {
    perror(fname);
    return 1;
  }

  fclose(rom);

  return 0;
}
