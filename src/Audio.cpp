// Copyright (c) 2022 Berk Kırtay

#include "Audio.h"

Audio::Audio()
{
    Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4096);
}

Audio::~Audio()
{
    Mix_CloseAudio();
}

void Audio::load(std::string path)
{
    music = Mix_LoadMUS(path.c_str());
}

void Audio::play()
{
    Mix_PlayMusic(music, 1);
}