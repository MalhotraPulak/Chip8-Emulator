# Chip8-Emulator
![image](https://user-images.githubusercontent.com/56169176/201547967-84cc3941-e0c3-4e73-8e1d-2842d7871cd4.png) <br/>
An interpreter for [Chip8](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#Fx29) written in C++. <br/>

## Features
- Supports all standard CHIP8 instructions except beep sound
- Handles user input mapped to keys `0 - 9` and `a - f`
- Displays the state of RAM
- Displays the state of each register and PC as execution happens
- Press `q` to quit the emulator


## Compile and make a debug build
```bash
mkdir cmake-build-debug
cd cmake-build-debug
cmake -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - Unix Makefiles" ../
cd ..
cmake --build cmake-build-debug --target Chip8 -- -j 9
```

## Run
```bash
cmake-build-debug/Chip8 <path/to/rom>
```
Roms for Chip8 can be found [here](https://github.com/kripod/chip8-roms)

