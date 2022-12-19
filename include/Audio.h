// Copyright (c) 2022 Berk Kırtay

#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>

class Audio
{
private:
    Mix_Music* music = nullptr;

public:
    Audio();
    ~Audio();
    void load(std::string path);
    void play();
};