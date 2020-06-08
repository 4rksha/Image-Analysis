#ifndef _AUDIO_H
#define _AUDIO_H

#include "libs/soloud20200207/include/soloud.h"
#include "libs/soloud20200207/include/soloud_wav.h"

class audio
{
private:
    SoLoud::Soloud soloud;
    SoLoud::Wav boom;
public:
    audio();
    ~audio();
    int audio_Init();
    void audio_Quit();
};




#endif