#ifndef MISTER8_INPUT_H
#define MISTER8_INPUT_H

#include <stdbool.h>
#include <stdint.h>

enum
{
  INVALID_KEY = UINT8_MAX,
};

uint8_t
get_key_pressed(void);

bool
is_key_pressed(int key);

#endif
