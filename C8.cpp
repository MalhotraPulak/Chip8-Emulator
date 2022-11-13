//
// Created by Pulak Malhotra on 13/11/22.
//

#include "C8.h"
#include<random>
#include <ncurses.h>

int C8::execute_instruction() {
    assert (pc % 2 == 0);
    const uint16_t instr = (((uint16_t) memory[pc]) << 8) + memory[pc + 1];
    pc += 2;
    // cout << std::hex << instr << endl;
    const auto first = first_nibble(instr);
    const auto fourth = fourth_nibble(instr);
    const auto x = second_nibble(instr);
    const auto y = third_nibble(instr);
    const auto n = fourth;
    const auto kk = instr & 0x00FF;
    const auto nnn = instr & 0x0FFF;
    if (first == 0x0 and kk == 0xE0) {
        clear_screen();
    } else if (first == 0x0 and kk == 0xEE) {
        assert(stkp >= 0x0 and stkp <= STACK_SIZE);
        pc = stack[stkp];
        stkp -= 1;
    } else if (first == 0x0) {
    } else if (first == 0x1) {
        pc = instr & nnn;
    } else if (first == 0x2) {
        stkp += 1;
        stack[stkp] = pc;
        pc = instr & nnn;
        assert(stkp >= 0x0 and stkp <= STACK_SIZE);
    } else if (first == 0x3) {
        if (registers[x] == (instr & 0x00FF)) {
            pc += 2;
        }
    } else if (first == 0x4) {
        if (registers[x] != (instr & 0x00FF)) {
            pc += 2;
        }
    } else if (first == 0x5 and fourth == 0x0) {
        if (registers[x] == registers[y]) {
            pc += 2;
        }
    } else if (first == 0x6) {
        registers[x] = instr & 0x00FF;
    } else if (first == 0x7) {
        registers[x] += instr & 0x00FF;
    } else if (first == 0x8 and fourth == 0x0) {
        registers[x] = registers[y];
    } else if (first == 0x8 and fourth == 0x1) {
        registers[x] |= registers[y];
    } else if (first == 0x8 and fourth == 0x2) {
        registers[x] &= registers[y];
    } else if (first == 0x8 and fourth == 0x3) {
        registers[x] ^= registers[y];
    } else if (first == 0x8 and fourth == 0x4) {
        // add
        int16_t rx = registers[x];
        int16_t ry = registers[y];
        auto sum = rx + ry;
        if (rx + ry > UINT8_MAX) {
            registers[0xF] = 1;
        } else {
            registers[0xF] = 0;
        }
        registers[x] = sum & 0xFFFF;
    } else if (first == 0x8 and fourth == 0x5) {
        if (registers[x] > registers[y]) {
            registers[0xF] = 1;
        } else {
            registers[0xF] = 0;
        }
        registers[x] = registers[x] - registers[y]; // todo check
    } else if (first == 0x8 and fourth == 0x6) {
        registers[0xF] = registers[x] & 0x0001;
        registers[x] = registers[x] >> 1;
    } else if (first == 0x8 and fourth == 0xE) {
        registers[0xF] = registers[x] & 0x8000;
        registers[x] = registers[x] << 1;
    } else if (first == 0x8 and fourth == 0x7) {
        if (registers[y] > registers[x]) {
            registers[0xF] = 1;
        } else {
            registers[0xF] = 0;
        }
        registers[x] = registers[y] - registers[x]; // todo check
    } else if (first == 0x9 and fourth == 0x0) {
        if (registers[x] != registers[y]) {
            pc += 2;
        }
    } else if (first == 0xA) {
        I = instr & 0x0FFF;
    } else if (first == 0xB) {
        uint16_t reg = registers[x];
        pc = (uint16_t) (instr & 0x0FFF) + reg;
    } else if (first == 0xC) {
        registers[x] = (uint8_t) (rand() % 256) & kk;
    } else if (first == 0xD) {
        registers[0xF] = sprite_apply(x, y, n);
    } else if (first == 0xE and kk == 0x9E) {
        if (is_key_pressed(registers[x])) {
            pc += 2;
        }
    } else if (first == 0xE and kk == 0xA1) {
        if (!is_key_pressed(registers[x])) {
            pc += 2;
        }
    } else if (first == 0xF and kk == 0x07) {
        registers[x] = DT;
    } else if (first == 0xF and kk == 0x0A) {
        registers[x] = wait_for_key_press();
    } else if (first == 0xF and kk == 0x15) {
        DT = registers[x];
    } else if (first == 0xF and kk == 0x18) {
        ST = registers[x];
    } else if (first == 0xF and kk == 0x1E) {
        I += registers[x];
    } else if (first == 0xF and kk == 0x29) {
        I = registers[x] * 5;
    } else if (first == 0xF and kk == 0x33) {
        auto val = registers[x];
        for (int i = 0; i < 3; i += 1) {
            memory[I + 2 - i] = val % 10;
            val /= 10;
        }
    } else if (first == 0xF and kk == 0x55) {
        for (int i = 0; i <= x; i++) {
            memory[I + i] = registers[i];
        }
    } else if (first == 0xF and kk == 0x65) {
        for (int i = 0; i <= x; i++) {
            registers[i] = memory[I + i];
        }
    } else {
        assert(false);
    }
    return instr;
}

void C8::fill_some_data() {
}

C8::C8() {
    load_sprites();
    fill_some_data();
}

uint16_t C8::fourth_nibble(uint16_t val) {
    return val & 0x000F;
}


uint16_t C8::third_nibble(uint16_t val) {
    return (val & 0x00F0) >> 4;
}

uint16_t C8::second_nibble(uint16_t val) {
    return (val & 0x0F00) >> 8;
}

uint16_t C8::first_nibble(uint16_t val) {
    return (val & 0xF000) >> 12;
}

void C8::clear_screen() {
    for (auto &row: screen) {
        for (auto &pixel: row) {
            pixel = 0;
        }
    }
}

bool C8::is_key_pressed(uint8_t key) const {
    if (pressed_key == ERR) {
        return wait_for_key_press() == key;
    }
    return pressed_key == key;
}

uint8_t C8::wait_for_key_press() const {
    timeout(-1);
    auto x = getch();
    timeout(0);
    return (x >= '0' and x <= '9') ? x - '0' : x - 'a' + 10;
}

bool C8::sprite_apply(uint8_t x, uint8_t y, uint8_t n) {
    auto x_start = registers[x];
    auto y_start = registers[y];
    auto collision = false;
    for (int i = 0; i < n; i++) {
        auto val = memory[I + i];
        for (int j = 8; j > 0; j--) {
            auto x_d = (x_start + j) % SCREEN_WIDTH;
            auto y_d = (y_start + i) % SCREEN_HEIGHT;
            auto bit = val & 1;
            val >>= 1;
            if (val == 1 and screen[x_d][y_d] == 1) {
                collision = true;
            }
            screen[x_d][y_d] = screen[x_d][y_d] ^ bit;
        }
    }

    return collision;
}

void C8::init_rom(const std::vector<unsigned char> &buffer) {
    pc = PROGRAM_START;
    for (int i = 0; i < buffer.size(); i++) {
        memory[pc + i] = (int8_t) buffer[i];
    }
    pc = PROGRAM_START;
}

void C8::load_sprites() {
    std::copy(sprites, sprites + FONTSET_SIZE, memory.begin());
}

void C8::decrement() {
    DT = std::max(DT - 1, 0);
    ST = std::max(ST - 1, 0);
}