#include "input.h"

#include <raylib.h>

enum
{
  KEY_COUNT = 16
};

/* must be written-to by store_key_pressed, and
 * read-from by is_key_pressed.
 * 1 means down (pressed), 0 means up (released)
 */
static uint8_t keys[KEY_COUNT] = { 0 };

void
store_key_pressed(void)
{
  /* 1 2 3 C */
  if (IsKeyDown(KEY_ONE))
    keys[0x1] = 1;
  else
    keys[0x1] = 0;
  if (IsKeyDown(KEY_TWO))
    keys[0x2] = 1;
  else
    keys[0x2] = 0;
  if (IsKeyDown(KEY_THREE))
    keys[0x3] = 1;
  else
    keys[0x3] = 0;
  if (IsKeyDown(KEY_FOUR))
    keys[0xC] = 1;
  else
    keys[0xC] = 0;

  /* 4 5 6 D */
  if (IsKeyDown(KEY_Q))
    keys[0x4] = 1;
  else
    keys[0x4] = 0;
  if (IsKeyDown(KEY_W))
    keys[0x5] = 1;
  else
    keys[0x5] = 0;
  if (IsKeyDown(KEY_E))
    keys[0x6] = 1;
  else
    keys[0x6] = 0;
  if (IsKeyDown(KEY_R))
    keys[0xD] = 1;
  else
    keys[0xD] = 0;

  /* 7 8 9 E */
  if (IsKeyDown(KEY_A))
    keys[0x7] = 1;
  else
    keys[0x7] = 0;
  if (IsKeyDown(KEY_S))
    keys[0x8] = 1;
  else
    keys[0x8] = 0;
  if (IsKeyDown(KEY_D))
    keys[0x9] = 1;
  else
    keys[0x9] = 0;
  if (IsKeyDown(KEY_F))
    keys[0xE] = 1;
  else
    keys[0xE] = 0;

  /* A 0 B F */
  if (IsKeyDown(KEY_Z))
    keys[0xA] = 1;
  else
    keys[0xA] = 0;
  if (IsKeyDown(KEY_X))
    keys[0x0] = 1;
  else
    keys[0x0] = 0;
  if (IsKeyDown(KEY_C))
    keys[0xB] = 1;
  else
    keys[0xB] = 0;
  if (IsKeyDown(KEY_V))
    keys[0xF] = 1;
  else
    keys[0xF] = 0;
}

bool
is_key_pressed(int key)
{
  if (keys[key])
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
