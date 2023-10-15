#include "input.h"

#include <raylib.h>

uint8_t
get_key_pressed(void)
{
  int key = GetKeyPressed();
  switch (key) {
    /* 1 2 3 C */
    case KEY_ONE:
      return 0x1;
    case KEY_TWO:
      return 0x2;
    case KEY_THREE:
      return 0x3;
    case KEY_FOUR:
      return 0xC;

    /* 4 5 6 D */
    case KEY_Q:
      return 0x4;
    case KEY_W:
      return 0x5;
    case KEY_E:
      return 0x6;
    case KEY_R:
      return 0xD;

    /* 7 8 9 E */
    case KEY_A:
      return 0x7;
    case KEY_S:
      return 0x8;
    case KEY_D:
      return 0x9;
    case KEY_F:
      return 0xE;

    /* A 0 B F */
    case KEY_Z:
      return 0xA;
    case KEY_X:
      return 0x0;
    case KEY_C:
      return 0xB;
    case KEY_V:
      return 0xF;

      /* non chip8 input key */
    default:
      return INVALID_KEY;
  }
}

uint8_t
is_key_released(int key)
{
  if (IsKeyReleased(key))
    return true;
  return false;
}
