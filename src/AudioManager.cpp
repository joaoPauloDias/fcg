#include "AudioManager.h"

#include <cstdlib>
#include <cstdio>

// There must be exactly one MINIAUDIO_IMPLEMENTATION define in your entire project, and it
// must be on a source file (.c, .cpp, ...), NOT a header file (.h, .hpp, ...).
// After defining MINIAUDIO_IMPLEMENTATION, include miniaudio.h and you're good to go.
#define MINIAUDIO_IMPLEMENTATION
#define strcasecmp strcmp // Hack to make miniaudio compile on windows
#include <miniaudio.h>

static ma_engine engine;

struct Sound {
  ma_sound sound;
};

Sound* AudioManager::makeSound(const char* fname, bool loop, float volume, bool stream) {
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

void AudioManager::destroySound(Sound* s) {
  ma_sound_uninit(&s->sound);
  free(s);
}

void AudioManager::init() {
  ma_result result;
  result = ma_engine_init(NULL, &engine);
  if (result != MA_SUCCESS) {
    printf("Failed to initialize audio engine.");
    exit(1);
  }
}

void AudioManager::destroy() {
  ma_engine_uninit(&engine);
}

void AudioManager::setSoundVolume(Sound *s, float volume) {
  ma_sound_set_volume(&s->sound, volume);
}

void AudioManager::playSound(Sound *s) {
  ma_sound_start(&s->sound);
}

void AudioManager::stopSound(Sound *s) {
  ma_sound_stop(&s->sound);
}

bool AudioManager::isSoundPlaying(Sound* s) {
  return ma_sound_is_playing(&s->sound);
}