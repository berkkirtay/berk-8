// Copyright (c) 2022 Berk Kırtay

#include "Chip8.h"

void Chip8::initialize()
{
   PC = 0x200;
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
      memory[i] = fontSet[i];
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

void Chip8::emulateNextCycle()
{
   //FETCH step:
   fetch();

   // DECODE step:
   decode();

   // EXECUTE step:
   (this->*instructionMap[opcode])();

   // Handle timer and blockers:
   calculateTimersAndBlock();
}

/*
* @fetch
* Fetching currentInstruction is done by combining
* two contiguous memmory locations. Since CHIP-8
* stores instructions that way, we increment PC by 2.
*/
void Chip8::fetch()
{
   currentInstruction = memory[PC] << 8 | memory[PC + 1];
   PC += 2;
}

/*
* @decode
* Provides a basic decoding operation on
* instruction and sets Vx and Vy fields that
* will be used in the execute step.
*/
void Chip8::decode()
{
   opcode = static_cast<InstructionSet::InstructionFamily>(currentInstruction & 0xF000);
   Vx = (currentInstruction & 0x0F00) >> 8;
   Vy = (currentInstruction & 0x00F0) >> 4;
}

/*
 * @calculateTimersAndBlock function handles
 * timing and delaying in every instruction cycle.
 */
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

/*
* The rest of the functions handle execute
* step by realizing the necessary events for
* the current instruction:
*/
void Chip8::x0000()
{
   switch (currentInstruction & 0x0FFF)
   {
      // 00E0 - CLS
      case 0x00E0:
         memset(graphics, 0, sizeof(graphics));
         draw = true;
         break;
         // 00EE - RET
      case 0x00EE:
         SP--;
         PC = stack[SP];
         break;
         /* 0nnn - SYS addr:
          * 0nnn -> Calls machine code routine (RCA 1802 for COSMAC VIP)
          * at address NNN. Not necessary for most ROMs.
          */
      default:
         PC = memory[currentInstruction & 0x0FFF];
         break;
   }
}

/*
 * 1nnn - JP addr
 */
void Chip8::x1000()
{
   PC = currentInstruction & 0x0FFF;
}

/*
 * 2nnn - CALL addr
 */
void Chip8::x2000()
{
   stack[SP] = PC;
   SP++;
   PC = currentInstruction & 0x0FFF;
}

/*
 * 3xkk - SE Vx, kk
 */
void Chip8::x3000()
{
   if ((currentInstruction & 0x00FF) == registers[Vx])
   {
      PC += 2;
   }
}

/*
 * 4xkk - SNE Vx, kk
 */
void Chip8::x4000()
{
   if ((currentInstruction & 0x00FF) != registers[Vx])
   {
      PC += 2;
   }
}

/*
 * 5xy0 - SE Vx, Vy
 */
void Chip8::x5000()
{
   if (registers[Vx] == registers[Vy])
   {
      PC += 2;
   }
}

/*
 * 6xkk - LD Vx, kk
 */
void Chip8::x6000()
{
   registers[Vx] = currentInstruction & 0x00FF;
}

/*
 * 7xkk - ADD Vx, kk
 */
void Chip8::x7000()
{
   registers[Vx] += currentInstruction & 0x00FF;
}

/*
 * 0x8000 Arithmetic Instructions
 */
void Chip8::x8000()
{
   switch (currentInstruction & 0x000F)
   {
      //  8xy0 - LD Vx, Vy
      case 0x0000:
         registers[Vx] = registers[Vy];
         break;
         // 8xy1 - OR Vx, Vy
      case 0x0001:
         registers[Vx] |= registers[Vy];
         break;
         // 8xy2 - AND Vx, Vy
      case 0x0002:
         registers[Vx] &= registers[Vy];
         break;
         // 8xy3 - XOR Vx, Vy
      case 0x0003:
         registers[Vx] ^= registers[Vy];
         break;
         // 8xy4 - ADD Vx, Vy
      case 0x0004:
         {
            uint16_t sum = registers[Vx] + registers[Vy];
            if (sum > 255)
            {
               registers[0xF] = 1;
            }
            else
            {
               registers[0xF] = 0;
            }
            registers[Vx] = sum & 0x00FF;
            break;
         }
         // 8xy5 - SUB Vx, Vy
      case 0x0005:
         registers[Vx] -= registers[Vy];
         if (registers[Vx] > registers[Vy])
         {
            registers[0xF] = 1;
         }
         else
         {
            registers[0xF] = 0;
         }
         break;
         // 8xy6 - SHR Vx {, Vy}
      case 0x0006:
         registers[Vx] >>= 1;
         registers[0xF] = registers[Vx] & 0x01;
         break;
         // 8xy7 - SUBN Vx, Vy
      case 0x0007:
         registers[Vx] = registers[Vy] - registers[Vx];
         if (registers[Vx] < registers[Vy])
         {
            registers[0xF] = 1;
         }
         else
         {
            registers[0xF] = 0;
         }
         break;
         // 8xyE - SHL Vx {, Vy}
      case 0x000E:
         registers[Vx] <<= 1;
         registers[0xF] = registers[Vx] >> 7;
         break;
      default:
         std::cerr << "Error at instruction: " <<
            std::hex << currentInstruction << std::endl;
         break;
   }
}

/*
* 9xy0 - SNE Vx, Vy
*/
void Chip8::x9000()
{
   if (registers[Vx] != registers[Vy])
   {
      PC += 2;
   }
}

/*
* Annn - LD I, addr
*/
void Chip8::xA000()
{
   I = currentInstruction & 0x0FFF;
}

/*
* Bnnn - JP V0, addr
*/
void Chip8::xB000()
{
   PC = (currentInstruction & 0x0FFF) + registers[0];
}

/*
* Cxkk - RND Vx, byte
*/
void Chip8::xC000()
{
   registers[Vx] = (currentInstruction & 0x00FF) & (rand() % 255);
}

/*
* Dxyn - DRW Vx, Vy, n
* Sprite drawing instruction:
*/
void Chip8::xD000()
{
   uint8_t n = currentInstruction & 0x000F;
   registers[0xF] = 0;

   for (auto row = 0; row < n; row++)
   {
      // Processing sprites starting from I to I + height of:
      uint8_t sprite = memory[I + row];

      for (auto col = 0; col < 8; col++)
      {
         // Acquiring the pixel positions:
         uint16_t point_y = ((registers[Vy] + row) % 32) * 64;
         uint16_t point_x = ((registers[Vx] + col) % 64);

         // Acquiring the pixel value at the column and checking
         // if the pixel is chosen to be drawn:
         uint8_t spritePixel = sprite & (0x80 >> col);
         if (spritePixel != 0)
         {
            // If collision happens at sprite, we set VF to 1:
            if (graphics[point_y + point_x])
            {
               registers[0xF] = 1;
            }
            // Flipping the value of pixel, so if it is 
            // already drawn, it will be turned off:
            graphics[point_y + point_x] = ~graphics[point_y + point_x];
         }
      }
   }
   draw = true;
}

/*
* Key press branching instructions:
*/
void Chip8::xE000()
{
   switch (currentInstruction & 0x00FF)
   {
      // Ex9E - SKP Vx
      case 0x009E:
         if (keys[registers[Vx]] != 0)
         {
            PC += 2;
         }
         break;
         // ExA1 - SKNP Vx
      case 0x00A1:
         if (keys[registers[Vx]] == 0)
         {
            PC += 2;
         }
         break;
      default:
         std::cerr << "Error at instruction: " <<
            std::hex << currentInstruction << std::endl;
         break;
   }
}

void Chip8::xF000()
{
   switch (currentInstruction & 0x00FF)
   {
      // Fx07 - LD Vx, DT
      case 0x0007:
         registers[Vx] = delayTimer;
         break;

         /*
         * Fx0A - LD Vx, K
         * Polling Operation. All instructions
         * are halted until next key event.
         */
      case 0x000A:
         {
            bool isKeyPressed = false;
            for (int i = 0; i < 16; i++)
            {
               if (keys[i] != 0)
               {
                  registers[Vx] = i + 1;
                  isKeyPressed = true;
                  break;
               }
            }
            if (isKeyPressed == false)
            {
               PC -= 2;
            }
            break;
         }

         // Fx15 - LD DT, Vx
      case 0x0015:
         delayTimer = registers[Vx];
         break;
         // Fx18 - LD ST, Vx
      case 0x0018:
         soundTimer = registers[Vx];
         break;
         // Fx1E - ADD I, Vx
      case 0x001E:
         I += registers[Vx];
         break;
         // Fx29 - LD F, Vx
      case 0x0029:
         I = registers[Vx] * 0x5;
         break;
         // Fx33 - LD B, Vx
      case 0x033:
         memory[I] = registers[Vx] / 100;
         memory[I + 1] = (registers[Vx] / 10) % 10;
         memory[I + 2] = registers[Vx] % 10;
         break;
         // Fx55 - LD [I], Vx
      case 0x055:
         for (int i = 0; i <= Vx; i++)
         {
            memory[I + i] = registers[i];
         }
         break;
         // Fx65 - LD Vx, [I]
      case 0x065:
         for (int i = 0; i <= Vx; i++)
         {
            registers[i] = memory[I + i];
         }
         break;
      default:
         std::cerr << "Error at instruction: " <<
            std::hex << currentInstruction << std::endl;
         break;
   }
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