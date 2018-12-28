#include "audio.hpp"

#include "vector.hpp"

#include <cmath>
#include <iostream>

Audio *Audio::self = NULL;
const int Audio::amp = 28000;
const int Audio::freq = 44100;
const float Audio::notes[]
    = {293.665, 329.628, 349.228, 391.995, 440.000, 493.883, 554.365, 587.330,
       587.33,  659.256, 698.456, 783.99,  880.0,   987.766, 1108.73, 1174.66};

const int Audio::notes_length = 16;

float getAmplitude(const Vector &position, const Vector &v_rel)
{
    const float distance = (position - Vector(0.f, 0.f, 0.f)).length();
    const float v_rel_squared = v_rel.length2();

    const float underlog = (v_rel_squared / 4.f) - 2.f * (distance - 1);
    const float dec = underlog > 0 ? 100.f * (1 + log(underlog)) : 20.f;

    // Decibel to amplitude
    return dec / 200.f;
}

Audio &Audio::getAudio()
{
    if (!self)
    {
        self = new Audio();
    }
    return *self;
}

void Audio::destroyAudio()
{
    if (self)
    {
        delete self;
        self = NULL;
    }
}

void Audio::playSound(int sound, float duration, float amp)
{
    Beep beep;
    beep.freq = notes[sound % notes_length];
    beep.samples = duration * Audio::freq / 1000;
    beep.amp = std::min(amp, 1.f);

    if (amp <= 0.f) return;

    SDL_LockAudio();
    this->beep = beep;
    SDL_UnlockAudio();
    SDL_PauseAudio(0);
}

void Audio::generateSamples(Sint16 *stream, int length)
{
    for (int i = 0; i < length && beep.samples > 0; ++i)
    {
        stream[i]
            = beep.amp * Audio::amp * std::sin(v * 2 * M_PI / Audio::freq);
        v += beep.freq;
        beep.samples -= 1;
    }

    if (beep.samples == 0)
    {
        SDL_PauseAudio(-1);
    }
}

static void audio_callback(void *audio_, Uint8 *stream_, int length_)
{
    Sint16 *stream = (Sint16 *)stream_;
    int length = length_ / 2;
    Audio *audio = (Audio *)audio_;

    audio->generateSamples(stream, length);
}

Audio::Audio() : v(0.f), playing(false)
{
    SDL_AudioSpec desiredSpec;

    desiredSpec.freq = Audio::freq;
    desiredSpec.format = AUDIO_S16SYS;
    desiredSpec.channels = 1;
    desiredSpec.samples = 2048;
    desiredSpec.callback = audio_callback;
    desiredSpec.userdata = this;

    SDL_AudioSpec obtainedSpec;

    // you might want to look for errors here
    SDL_OpenAudio(&desiredSpec, &obtainedSpec);
}

Audio::~Audio()
{
    SDL_CloseAudio();
}
