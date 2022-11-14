//
// Created by Pulak Malhotra on 14/11/22.
//

#include "Display.h"
#include <sstream>
#include <iostream>
#include <ncurses.h>
#include <iomanip>

Display::Display() {
    initscr();
    cbreak();
    noecho();
    start_color();
    keypad(stdscr, TRUE);
    init_pair(1, COLOR_BLACK, COLOR_RED);
    init_pair(2, COLOR_BLACK, COLOR_GREEN);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_WHITE, COLOR_WHITE);
    timeout(0);
    attron(COLOR_PAIR(3));
}

void Display::update(C8 &c8) {
    print_screen(c8);
    print_memory(c8);
    print_register(c8);
}


void Display::print_screen(C8 &cpu) {
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        for (int j = 0; j < SCREEN_HEIGHT; j++) {
            if (cpu.screen[i][j] == 0) {
                attron(COLOR_PAIR(3));
            } else {
                attron(COLOR_PAIR(4));
            }
            mvprintw(j, i, " ");
        }
    }
}

std::string Display::int_to_hex(const uint8_t v) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    ss << std::hex << std::setw(2) << static_cast<int>(v);
    return ss.str();
}

std::string Display::int_to_hex(const uint16_t v) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    ss << std::hex << std::setw(4) << static_cast<int>(v);
    return ss.str();
}

void Display::print_memory(C8 &cpu) {
    int off = 35;
    for (int i = 512; i < 1024; i++) {
        if (i % 0x40 == 0) {
            off += 1;
            move(off, 0);
        }
        if (i == cpu.pc || i == cpu.pc + 1) {
            attron(COLOR_PAIR(1));
        } else if (i == cpu.I) {
            attron(COLOR_PAIR(2));
        } else {
            attron(COLOR_PAIR(3));
        }
        printw(int_to_hex(cpu.memory[i]).c_str());
        printw(" ");
    }
}

void Display::print_register(C8 &cpu) {
    uint8_t i;
    for (i = 0; i < NUM_REGISTERS; i++) {
        move(i, 65);
        auto f = int_to_hex(i) + ":" + int_to_hex(cpu.registers[i]);
        printw(f.c_str());
    }
    auto I = "I :" + int_to_hex(cpu.I);
    mvprintw(++i, 65, I.c_str());

    auto DT = "DT:" + int_to_hex(cpu.DT);
    mvprintw(++i, 65, DT.c_str());

    auto ST = "ST:" + int_to_hex(cpu.ST);
    mvprintw(++i, 65, ST.c_str());

    auto stk = "STK:" + int_to_hex(cpu.stkp);
    mvprintw(++i, 65, stk.c_str());

    auto PC = "PC:" + int_to_hex(cpu.pc);
    mvprintw(++i, 65, PC.c_str());
}

Display::~Display() {
    endwin();
}

int32_t Display::handleInput(C8 &c8) {
    timeout(0);
    auto x = getch();
    if (x == 'q'){
        return x;
    }
    if (x != ERR)
        return c8.pressed_key = (x >= '0' and x <= '9') ? (x - '0') : x - 'a' + 10;
    return c8.pressed_key = ERR;
}
