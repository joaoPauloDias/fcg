#pragma once
struct Sound;

class AudioManager {
  private:
    AudioManager();

  public:
    static void init();
    static void destroy();

    static Sound* makeSound(const char* fname, bool loop = false, float volume = 1, bool stream = false);
    static void destroySound(Sound *s);

    static void setSoundVolume(Sound *s, float volume);

    static void playSound(Sound *s);
    static void stopSound(Sound *s);

    static bool isSoundPlaying(Sound *s);
};

