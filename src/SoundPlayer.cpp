#include "SoundPlayer.h"

#include <cstdlib>
#include <cstdio>

// There must be exactly one MINIAUDIO_IMPLEMENTATION define in your entire project, and it
// must be on a source file (.c, .cpp, ...), NOT a header file (.h, .hpp, ...).
// After defining MINIAUDIO_IMPLEMENTATION, include miniaudio.h and you're good to go.
#define MINIAUDIO_IMPLEMENTATION
#define strcasecmp strcmp // Hack to make miniaudio compile on windows
#include <miniaudio/miniaudio.h>

// FONTE: https://github.com/mackron/miniaudio

static ma_engine engine;

struct Sound {
  ma_sound sound;
};

Sound* SoundPlayer::initSound(const char* fname, bool loop, float volume, bool stream) {
  Sound* s = (Sound*)malloc(sizeof(Sound));
  if (stream) {
    ma_sound_init_from_file(&engine, fname, MA_SOUND_FLAG_STREAM, NULL, NULL, &s->sound);
  } else {
    ma_sound_init_from_file(&engine, fname, 0, NULL, NULL, &s->sound);
  }
  ma_sound_set_looping(&s->sound, loop);
  ma_sound_set_volume(&s->sound, volume);
  return s;
}


void SoundPlayer::init() {
  ma_result result;
  result = ma_engine_init(NULL, &engine);
  if (result != MA_SUCCESS) {
    printf("Failed to initialize audio engine.");
    exit(1);
  }
}

void SoundPlayer::setVolume(Sound *s, float volume) {
  ma_sound_set_volume(&s->sound, volume);
}

void SoundPlayer::playSound(Sound *s) {
  ma_sound_start(&s->sound);
}

bool SoundPlayer::isPlaying(Sound* s) {
  return ma_sound_is_playing(&s->sound);
}