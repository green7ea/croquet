#ifndef AUDIO_H
#define AUDIO_H

#include "SDL_audio.h"

class Vector;

float getAmplitude(const Vector &position, const Vector &v_rel);

class Audio
{
public:
    static Audio &getAudio();
    static void destroyAudio();

    void playSound(int sound, float duration, float amp);
    void generateSamples(Sint16 *stream, int length);

private:
    Audio();
    ~Audio();

    static const int amp;
    static const int freq;
    static const float notes[];
    static const int notes_length;

    struct Beep
    {
        float freq;
        int samples;
        float amp;
    };

    Beep beep;
    float v;
    bool playing;

    static Audio *self;
};

#endif // AUDIO_H
