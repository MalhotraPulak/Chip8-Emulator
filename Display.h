//
// Created by Pulak Malhotra on 14/11/22.
//

#ifndef CHIP8_DISPLAY_H
#define CHIP8_DISPLAY_H


#include "C8.h"

class Display {
    static void print_register(C8 &c8);

    static void print_memory(C8 &c8);

    static void print_screen(C8 &c8);

    static std::string int_to_hex(uint8_t v);

    static std::string int_to_hex(uint16_t v);

public:
    Display();

    ~Display();

    void update(C8 &c8);

    static int32_t handleInput(C8 &c8);
};


#endif //CHIP8_DISPLAY_H
