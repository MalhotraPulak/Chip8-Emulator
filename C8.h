//
// Created by Pulak Malhotra on 13/11/22.
//

#ifndef CHIP8_C8_H
#define CHIP8_C8_H

#include <iostream>
#include <array>
#include <ncurses.h>
#include <thread>

// todo look how to handle constants
const int MEMORY_SIZE = 4096;
const int PROGRAM_START = 0x200;
const int NUM_REGISTERS = 16;
const int SCREEN_WIDTH = 64;
const int SCREEN_HEIGHT = 32;
const int FONTSET_SIZE = 16 * 5;
const int STACK_SIZE = 16;

const uint8_t sprites[FONTSET_SIZE] =
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

class C8 {
    uint16_t fetch_instruction();

    void load_sprites();

    void clear_screen();

    static uint16_t first_nibble(uint16_t val);

    static uint16_t second_nibble(uint16_t val);

    static uint16_t third_nibble(uint16_t val);

    static uint16_t fourth_nibble(uint16_t val);

    bool is_key_pressed(uint8_t key) const;

    static uint8_t wait_for_key_press();

    bool sprite_apply(uint8_t x, uint8_t y, uint8_t n);

    void load_rom(const std::vector<unsigned char> &buffer);

    static void delayTimer(C8 *cpu);

    std::thread timerThread;
public:
    std::atomic<bool> finish;
    std::array<uint8_t, NUM_REGISTERS>
            registers{};
    std::array<uint16_t, STACK_SIZE> stack{};
    uint16_t I = 0x0;
    std::atomic<uint8_t> DT = 0x0; // delay
    std::atomic<uint8_t> ST = 0x0; // sound
    uint16_t pc = 0x200;
    uint8_t stkp = 0x0;
    std::array<std::array<uint8_t, SCREEN_HEIGHT>, SCREEN_WIDTH> screen{};
    std::array<uint8_t, MEMORY_SIZE> memory{};
    int32_t pressed_key{ERR};

    explicit C8(const std::vector<unsigned char> &buffer);

    int execute_instruction();

    void decrement();

    ~C8();

};


#endif //CHIP8_C8_H
