# A CHIP-8 Interpreter Emulator Implementation with C++ and SDL2
**I create this repository to construct a reference point for emulation 101 and beyond.
berk-8 uses general features of chip-8 interpreter. Additionally, I used SDL2 libary for
both graphics and sound.**

## About CHIP-8
[1] CHIP-8 is an interpreted programming language, developed by Joseph Weisbecker made on his 1802 Microprocessor. It was initially used on the COSMAC VIP and Telmac 1800 8-bit microcomputers in the mid-1970s. CHIP-8 programs are run on a CHIP-8 virtual machine. It was made to allow video games to be more easily programmed for these computers. The simplicity of CHIP-8, and its long history and popularity, has ensured that CHIP-8 emulators and programs are still being made to this day.

## Architecture of CHIP-8
todo

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
