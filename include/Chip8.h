#pragma once
#include <iostream>
#include <memory>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>
#include <time.h>
#include <unistd.h>
#include <random>
#include <string.h>
#include <thread>
// Copyright (c) 2022 Berk Kırtay

#include <chrono>
#include <functional>
#include <unordered_map>
#include "InstructionSet.h"

/*
 * @opcode: 0xNNNN, 0xnVxVyn
 * @registers take index values from Vx and Vy.
 */
class Chip8
{
private:
    uint8_t memory[0x1000];
    uint16_t opcode;
    uint8_t registers[0x10];
    uint16_t stack[0x10];
    uint8_t keys[0x10];
    uint32_t graphics[64 * 32];
    uint16_t I;
    uint16_t PC;
    uint16_t SP;
    bool draw;
    uint8_t delayTimer;
    uint8_t soundTimer;
    std::chrono::steady_clock::time_point timePoint;
    uint8_t Vx;
    uint8_t Vy;
    InstructionSet::InstructionFamily currentInstruction;
    uint8_t fontset[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

private:
    void fetch();
    void decode();
    /*
     * calculateTimersAndBlock function handles
     * timing and delaying in every instruction cycle.
     */
    void calculateTimersAndBlock();

public:
    void initialize();
    int32_t loadROM(const char* ROMPath);
    void emulateNextClockCycle();
    uint32_t* getGraphics();
    uint8_t* getKeys();
    uint8_t getSoundTimer();
    bool getDraw();
    void setDraw(bool d);
};