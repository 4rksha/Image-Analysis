#include "audio.h"
#include "libs/soloud20200207/include/soloud.h"
#include "libs/soloud20200207/include/soloud_wav.h"

audio::audio()
{
}

audio::~audio()
{
}

int audio::audio_Init()
{
    soloud.init(); 
    boom.setLooping(1);
    boom.load("data/projet/audio/2.wav");

    return 0;
}

SoLoud::Wav & audio::getWav()
{
    return boom;
}

void audio::play_audio(SoLoud::Wav & b)
{
    soloud.play(b);   
}

void audio::pause_audio(SoLoud::Wav & b)
{
    b.stop();
}

void audio::audio_Quit()
{
    soloud.deinit();
}