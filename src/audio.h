#ifndef MISTER8_AUDIO_H
#define MISTER8_AUDIO_H

#include <raylib.h>

Sound
load_beep(void);

void
play_beep(Sound beep);

void
unload_beep(Sound beep);

#endif // MISTER8_AUDIO_H
