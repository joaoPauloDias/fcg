#pragma once
struct Sound;

class SoundPlayer {
  private:
    SoundPlayer();

  public:
    static void init();

    static Sound* initSound(const char* fname, bool loop = false, float volume = 1, bool stream = false);

    static void setVolume(Sound *s, float volume);

    static void playSound(Sound *s);

    static bool isPlaying(Sound *s);
};

