#ifndef MISTER8_INPUT_H
#define MISTER8_INPUT_H

#include <stdint.h>

enum
{
  INVALID_KEY = UINT8_MAX,
};

uint8_t
get_key_pressed(void);

uint8_t
is_key_released(int key);

#endif
