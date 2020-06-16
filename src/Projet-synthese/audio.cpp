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
    boom.load("data/projet/audio/pas.wav");
    pluie.setLooping(1);
    pluie.load("data/projet/audio/pluie.wav");
    vase.load("data/projet/audio/vase.wav");
    saut.setLooping(1);
    saut.load("data/projet/audio/saut.wav");
    cri.load("data/projet/audio/cri.wav");
    return 0;
}

SoLoud::Wav & audio::getWav()
{
    return boom;
}
SoLoud::Wav & audio::getVase()
{
    return vase;
}
SoLoud::Wav & audio::getSaut()
{
    return saut;
}
SoLoud::Wav & audio::getPluie()
{
    return pluie;
}
SoLoud::Wav & audio::getCri()
{
    return cri;
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