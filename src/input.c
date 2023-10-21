#include "input.h"

#include <raylib.h>
#include <string.h>

enum
{
  KEY_COUNT = 16,
  KEY_HELD = 1,
  KEY_RELEASED = 0
};

/* these variables must be written-to by update_keys,
 * and read-from by  get_key.*/
static uint8_t keys[KEY_COUNT] = { 0 };
static uint8_t keys_last[KEY_COUNT] = { 0 };

void
update_keys(void)
{
  memcpy(keys_last, keys, KEY_COUNT);

  int kbd_keys[] = {
    [0x1] = KEY_ONE, [0x2] = KEY_TWO, [0x3] = KEY_THREE, [0xc] = KEY_FOUR,
    [0x4] = KEY_Q,   [0x5] = KEY_W,   [0x6] = KEY_E,     [0xd] = KEY_R,
    [0x7] = KEY_A,   [0x8] = KEY_S,   [0x9] = KEY_D,     [0xe] = KEY_F,
    [0xa] = KEY_Z,   [0x0] = KEY_X,   [0xb] = KEY_C,     [0xf] = KEY_V
  };
  for (unsigned int i = 0; i < sizeof(kbd_keys) / sizeof(kbd_keys[0]); i++)
    keys[i] = IsKeyDown(kbd_keys[i]);
}

bool
is_key_released(const int key)
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
