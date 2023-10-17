#ifndef MISTER8_INPUT_H
#define MISTER8_INPUT_H

#include <stdbool.h>
#include <stdint.h>

enum
{
  INVALID_KEY = UINT8_MAX,
};

void
update_keys(void);

bool
is_key_released(int key);

uint8_t
any_pressed_key(void);

uint8_t
get_keypress(void);
#endif
