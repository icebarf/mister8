#ifndef MISTER8_INPUT_H
#define MISTER8_INPUT_H

#include <stdbool.h>
#include <stdint.h>

enum
{
  INVALID_KEY = UINT8_MAX,
};

void
store_key_pressed(void);

bool
is_key_pressed(int key);

uint8_t
any_pressed_key(void);

#endif
