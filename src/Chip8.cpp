// Copyright (c) 2022 Berk Kırtay

#include "Chip8.h"

void Chip8::initialize()
{
   PC = 0x200;
   opcode = 0;
   I = 0;
   SP = 0;
   delayTimer = 0;
   soundTimer = 0;
   timePoint = std::chrono::steady_clock::now();
   draw = false;
   memset(graphics, 0, sizeof(graphics));
   memset(stack, 0, sizeof(stack));
   memset(keys, 0, sizeof(keys));
   memset(memory, 0, sizeof(memory));
   memset(registers, 0, sizeof(registers));

   for (int i = 0; i < 80; ++i)
   {
      memory[i + 0x50] = fontset[i];
   }
   srand(time(NULL));
}

int32_t Chip8::loadROM(const char* ROMPath)
{
   std::ifstream FILE(ROMPath, std::ios::binary | std::ios::ate);
   if (FILE)
   {
      std::streampos size = FILE.tellg();
      char* buffer = new char[size];
      FILE.seekg(0, std::ios::beg);
      FILE.read(buffer, size);
      FILE.close();

      for (long i = 0; i < static_cast<size_t>(size); ++i)
      {
         memory[0x200 + i] = buffer[i];
      }
      delete[] buffer;
      return static_cast<int32_t>(size);
   }
   else
   {
      return -1;
   }
}



void Chip8::emulateNextClockCycle()
{
   //FETCH step:
   fetch();

   // DECODE step:
   decode();

   // TODO: EXECUTE step:


   // Handle timer and blockers:
   calculateTimersAndBlock();
}

/*
* Fetching opcode is done by combining two contiguous memmory locations.
* Since CHIP-8 stores instructions in that way, we increment PC by 2.
*/
void Chip8::fetch()
{
   opcode = memory[PC] << 8 | memory[PC + 1];
   PC += 2;
}

void Chip8::decode()
{
   currentInstruction = static_cast<InstructionSet::InstructionFamily>(opcode & 0xF000);
   Vx = (opcode & 0x0F00) >> 8;
   Vy = (opcode & 0x00F0) >> 4;
}
void Chip8::calculateTimersAndBlock()
{
   /*
    * 60 Hz = 	0.0167 seconds = 16.7 ms
    */
   using namespace std::chrono;
   auto timer = static_cast<int8_t>(
      duration_cast<milliseconds>(steady_clock::now() - timePoint).count());

   if (timer >= 16.7)
   {
      timePoint = std::chrono::steady_clock::now();
      if (delayTimer > 0)
      {
         delayTimer--;
      }

      if (soundTimer > 0)
      {
         soundTimer--;
      }
   }
   /*
    * 600 Hz = 0.00167 seconds = 1670 microseconds (1.67 ms)
    */
   std::this_thread::sleep_for(std::chrono::microseconds(1670));
}

uint32_t* Chip8::getGraphics()
{
   return graphics;
}

uint8_t* Chip8::getKeys()
{
   return keys;
}

uint8_t Chip8::getSoundTimer()
{
   return soundTimer;
}

bool Chip8::getDraw()
{
   return draw;
}

void Chip8::setDraw(bool d)
{
   draw = d;
}