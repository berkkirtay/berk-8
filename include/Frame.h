// Copyright (c) 2022 Berk Kırtay

#pragma once
#include <iostream>
#include <memory>
#include "Audio.h"
#include "Screen.h"
#include "Chip8.h"
#include "EventHandler.h"
#include <SDL2/SDL.h>

class Frame
{
private:
    std::unique_ptr<Audio> audio;
    std::unique_ptr<Screen> screen;
    std::unique_ptr<Chip8> chip8;
    std::unique_ptr<EventHandler> eventHandler;

public:
    void initializeComponents();
    void load(const char* ROMPath);
    void mainLoop();
};