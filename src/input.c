#include "input.h"

#include <raylib.h>
#include <string.h>

enum
{
  KEY_COUNT = 16,
  KEY_HELD = 1,
  KEY_RELEASED = 0
};

/* these variables must be written-to by update_keys, and
 * read-from by get_key.
 * 1 means down (pressed), 0 means up (released)
 */
static uint8_t keys[KEY_COUNT] = { 0 };
static uint8_t keys_last[KEY_COUNT] = { 0 };

void
update_keys(void)
{
  memcpy(keys_last, keys, KEY_COUNT);
  /* 1 2 3 C */
  if (IsKeyDown(KEY_ONE))
    keys[0x1] = KEY_HELD;
  else
    keys[0x1] = KEY_RELEASED;
  if (IsKeyDown(KEY_TWO))
    keys[0x2] = KEY_HELD;
  else
    keys[0x2] = KEY_RELEASED;
  if (IsKeyDown(KEY_THREE))
    keys[0x3] = KEY_HELD;
  else
    keys[0x3] = KEY_RELEASED;
  if (IsKeyDown(KEY_FOUR))
    keys[0xC] = KEY_HELD;
  else
    keys[0xC] = KEY_RELEASED;

  /* 4 5 6 D */
  if (IsKeyDown(KEY_Q))
    keys[0x4] = KEY_HELD;
  else
    keys[0x4] = KEY_RELEASED;
  if (IsKeyDown(KEY_W))
    keys[0x5] = KEY_HELD;
  else
    keys[0x5] = KEY_RELEASED;
  if (IsKeyDown(KEY_E))
    keys[0x6] = KEY_HELD;
  else
    keys[0x6] = KEY_RELEASED;
  if (IsKeyDown(KEY_R))
    keys[0xD] = KEY_HELD;
  else
    keys[0xD] = KEY_RELEASED;

  /* 7 8 9 E */
  if (IsKeyDown(KEY_A))
    keys[0x7] = KEY_HELD;
  else
    keys[0x7] = KEY_RELEASED;
  if (IsKeyDown(KEY_S))
    keys[0x8] = KEY_HELD;
  else
    keys[0x8] = KEY_RELEASED;
  if (IsKeyDown(KEY_D))
    keys[0x9] = KEY_HELD;
  else
    keys[0x9] = KEY_RELEASED;
  if (IsKeyDown(KEY_F))
    keys[0xE] = KEY_HELD;
  else
    keys[0xE] = KEY_RELEASED;

  /* A 0 B F */
  if (IsKeyDown(KEY_Z))
    keys[0xA] = KEY_HELD;
  else
    keys[0xA] = KEY_RELEASED;
  if (IsKeyDown(KEY_X))
    keys[0x0] = KEY_HELD;
  else
    keys[0x0] = KEY_RELEASED;
  if (IsKeyDown(KEY_C))
    keys[0xB] = KEY_HELD;
  else
    keys[0xB] = KEY_RELEASED;
  if (IsKeyDown(KEY_V))
    keys[0xF] = KEY_HELD;
  else
    keys[0xF] = KEY_RELEASED;
}

bool
is_key_released(int key)
{
  if (keys[key] == KEY_RELEASED)
    return true;
  return false;
}

uint8_t
any_pressed_key(void)
{
  for (uint8_t i = 0; i < KEY_COUNT; i++) {
    if (keys[i])
      return i;
  }
  return INVALID_KEY;
}

uint8_t
get_keypress(void)
{
  for (uint8_t i = 0; i < 16; i++) {
    if (keys_last[i] == KEY_HELD && keys[i] == KEY_RELEASED)
      return i;
  }
  return INVALID_KEY;
}
