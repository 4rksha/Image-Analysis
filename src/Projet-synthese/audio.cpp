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
    boom.load("data/projet/audio/1.wav");
    soloud.play(boom);

    return 0;
}

void audio::audio_Quit()
{
    soloud.deinit();
}