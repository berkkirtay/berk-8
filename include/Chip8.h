// Copyright (c) 2022 Berk Kırtay

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
#include <chrono>
#include <unordered_map>
#include "InstructionSet.h"

/*
 * @opcode: 0xNNNN, 0xnVxVyn
 * @registers take index values from Vx and Vy operands.
 */
class Chip8
{
private:
    uint8_t memory[0x1000];
    uint16_t currentInstruction;
    InstructionSet::InstructionFamily opcode;
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
    uint8_t fontSet[80] =
    {
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

    /*
     * Instruction Handler Functions:
     */
    using Handler = void (Chip8::*)();
    std::unordered_map<InstructionSet::InstructionFamily, Handler>
        instructionMap =
    {
            {InstructionSet::x0000, &Chip8::x0000},
            {InstructionSet::x1000, &Chip8::x1000},
            {InstructionSet::x2000, &Chip8::x2000},
            {InstructionSet::x3000, &Chip8::x3000},
            {InstructionSet::x4000, &Chip8::x4000},
            {InstructionSet::x5000, &Chip8::x5000},
            {InstructionSet::x6000, &Chip8::x6000},
            {InstructionSet::x7000, &Chip8::x7000},
            {InstructionSet::x8000, &Chip8::x8000},
            {InstructionSet::x9000, &Chip8::x9000},
            {InstructionSet::xA000, &Chip8::xA000},
            {InstructionSet::xB000, &Chip8::xB000},
            {InstructionSet::xC000, &Chip8::xC000},
            {InstructionSet::xD000, &Chip8::xD000},
            {InstructionSet::xE000, &Chip8::xE000},
            {InstructionSet::xF000, &Chip8::xF000}
    };

private:
    void fetch();
    void decode();
    void calculateTimersAndBlock();
    void x0000();
    void x1000();
    void x2000();
    void x3000();
    void x4000();
    void x5000();
    void x6000();
    void x7000();
    void x8000();
    void x9000();
    void xA000();
    void xB000();
    void xC000();
    void xD000();
    void xE000();
    void xF000();

public:
    void initialize();
    int32_t loadROM(const char* ROMPath);
    void emulateNextCycle();
    uint32_t* getGraphics();
    uint8_t* getKeys();
    uint8_t getSoundTimer();
    bool getDraw();
    void setDraw(bool d);
};