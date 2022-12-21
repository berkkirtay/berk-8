# A CHIP-8 Emulator Implementation with C++ and SDL2
**This repository provides a reference point for emulation 101 and beyond. berk-8 uses general features of CHIP-8 interpreter. The development of berk-8 has an object-oriented approach and the code can be examined in a straightforward way. SDL2 library is used for the development of both graphics and sound. More detailed information and emulator execution examples can be found below.**

## About CHIP-8
[1] CHIP-8 is an interpreted programming language, developed by Joseph Weisbecker made on his 1802 Microprocessor. It was initially used on the COSMAC VIP and Telmac 1800 8-bit microcomputers in the mid-1970s. CHIP-8 programs are run on a CHIP-8 virtual machine. It was made to allow video games to be more easily programmed for these computers. The simplicity of CHIP-8, and its long history and popularity, has ensured that CHIP-8 emulators and programs are still being made to this day.

## Architecture of CHIP-8
**CHIP-8 Processor provides:**
- A 2048 kb main memory with 8-bit word size
- 16 8-bit registers (V)
- 16 16-bit stack registers for recursive subroutine calls
- 16 8-bit key registers for keypad I/O
- I 2 byte register for storing memory addresses
- PC, SP, a draw flag, delay and sound timers
- 36 different 16-bit long instructions

**CHIP-8 Instruction Set Architecture:** 

As it is mentioned earlier, CHIP-8 has 36 different instructions and size of the instructions are 16-bit long.
The instructions are in big-endian format and they cover 2 contiguous memory areas per instruction. 
CHIP-8 has various types of instructions, such as arithmetic and logic operations, simple branching and jumping, 
subroutine calls and returns, polling and waiting, and graphics manipulation.

All the instructions follow the general flow of a state machine approach.
This can be observed in the emulateNextClockCycle function below:
```cpp
void Chip8::emulateNextClockCycle()
{
   //FETCH step:
   fetch();

   // DECODE step:
   decode();

   // EXECUTE step:
   (this->*instructionMap[currentInstruction])();

   // Handle timer and blockers:
   calculateTimersAndBlock();
}
```

**CHIP-8 Display:**

Display is handled with a basic sprite drawing instruction (Dxyn - DRW Vx, Vy, n).
CHIP-8 has some built-in fontset and they are located in the first areas of main memory.
Every font requires 5-bytes memory and new fonts cna be programmed this way.
Whenever a draw instruction is compeleted, draw flag lets the graphics device know that
it can render the display. So, this is how display works on CHIP-8.

**CHIP-8 I/O:**

Available I/O device types are display, keypad and sound.
There are polling and waiting instructions for I/O. For example, 
polling instructions checks the state of keypad and waiting instruction
waits for a key to be pressed from keypad. Sound and display also works 
with polling instructions. The timers also work at a constant rate of 60Hz.

So, we can see that CHIP-8 has a very basic architecutre and does not rely 
on complex interrupts or huge number of various instructions. Size of the memory and
the registers are small and display is handled with a basic sprite drawing instruction.
An emulator for this interpreter can be easily implemented for many other platforms as well.

## Examples from berk-8
- **Space Invaders game**
![Enc1](https://github.com/berkkirtay/berk-8/blob/main/examples/space_invaders_with_berk8.gif)

- **Outlaw game**
![Enc1](https://github.com/berkkirtay/berk-8/blob/main/examples/outlaw_game.png)

- **Testing tool for instruction set of CHIP-8**
![Enc1](https://github.com/berkkirtay/berk-8/blob/main/examples/test_1.png)

## How to compile and run
- g++ -I ./include/ ./src/*.cpp --std=c++17 -ofast -o main.out -lSDL2 -lSDL2_mixer
- ./main.out --ROM {rom_name} 

## Future Plans
- A custom assembler can be implemented for CHIP-8 machine code.
- Instruction set can be expanded by adding Super CHIP-8 instrcutions.
- A custom ROM can be developed.

## References
[1] [https://en.wikipedia.org/wiki/CHIP-8](https://en.wikipedia.org/wiki/CHIP-8)

[2] [http://devernay.free.fr/hacks/chip8/C8TECH10.HTM](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
