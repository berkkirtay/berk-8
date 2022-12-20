// Copyright (c) 2022 Berk Kırtay

#include "Screen.h"

Screen::Screen()
{
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("berk-8",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1280, 640, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_RenderSetScale(renderer, 1, 1);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_UNKNOWN,
        SDL_TEXTUREACCESS_STREAMING,
        64, 32);


    SDL_SetTextureColorMod(texture, 255, 255, 255);
}

void Screen::render(uint32_t* graphics)
{
    /*
     * SDL lets us to render the screen in one pass
     * with the following functions:
     */
    SDL_RenderClear(renderer);
    SDL_UpdateTexture(texture, nullptr, graphics, 64 * sizeof(uint32_t));
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}