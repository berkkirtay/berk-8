// Copyright (c) 2022 Berk Kırtay

#pragma once
#include <SDL2/SDL.h>

class Screen
{
private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;

public:
    Screen();
    /*
     * render
     * Renders the screen by reading the
     * @graphics array from CHIP-8.
     */
    void render(uint32_t* graphics);
};