#include "audio.h"

#include <stdio.h>
#include <stdlib.h>

Sound
load_beep(void)
{
  Sound beep = LoadSound("beep.wav");
  if (IsSoundReady(beep) == false) {
    fprintf(stderr, "Beep sound could not be loaded\n");
    exit(1);
  }
  return beep;
}

void
play_beep(Sound beep)
{
  PlaySound(beep);
}

void
unload_beep(Sound beep)
{
  UnloadSound(beep);
}
