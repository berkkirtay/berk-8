// Copyright (c) 2022 Berk Kırtay

#include "Frame.h"

void Frame::initializeComponents()
{
    chip8 = std::make_unique<Chip8>();
    audio = std::make_unique<Audio>();
    screen = std::make_unique<Screen>();
    eventHandler = std::make_unique<EventHandler>();

    audio->load("sound93.wav");
    chip8->initialize();
}

void Frame::load(const char* ROMPath)
{
    auto programSize = chip8->loadROM(ROMPath);
    if (programSize == -1)
    {
        std::cout << "Error: The given path cannot be read!" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "ROM loaded, Program size: " << programSize << " bytes." << std::endl;
}

void Frame::mainLoop()
{
    while (true)
    {
        if (chip8->getSoundTimer() == 1)
        {
            audio->play();
        }

        // Graphics output from chip8:
        if (chip8->getDraw() == true)
        {
            chip8->setDraw(false);
            screen->render(chip8->getGraphics());
        }

        // Keyboard interrupt handler:
        eventHandler->handleEvent(chip8->getKeys());
        chip8->emulateNextClockCycle();
    }
}