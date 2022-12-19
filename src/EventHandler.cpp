// Copyright (c) 2022 Berk Kırtay

#include "EventHandler.h"

void EventHandler::handleEvent(uint8_t* keys)
{
    while (SDL_PollEvent(&event) == 1)
    {
        if (event.type == SDL_QUIT)
        {
            exit(EXIT_SUCCESS);
        }
        if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                exit(EXIT_SUCCESS);
            }

            if (keyMap.count(event.key.keysym.sym) != 0)
            {
                keys[keyMap[event.key.keysym.sym]] = KEY_UP;
            }
        }
        else if (event.type == SDL_KEYUP && keyMap.count(event.key.keysym.sym) != 0)
        {
            keys[keyMap[event.key.keysym.sym]] = KEY_DOWN;
        }
    }
}