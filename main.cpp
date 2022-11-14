#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include "C8.h"
#include "Display.h"

auto read_binary_file(char *path) {
    std::ifstream input(path, std::ios::binary);
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});
    return buffer;
}

int main(int argc, char **argv) {
    if (argc <= 1) {
        std::cout << "Usage: Chip8 <rom path>\n";
        return 1;
    }

    C8 c8(read_binary_file(argv[1]));
    Display display;

    while (true) {
        display.update(c8);
        c8.execute_instruction();
        auto key = Display::handleInput(c8);
        if (key == 'q'){
            break;
        }

        using namespace std;
        std::this_thread::sleep_for(1ms);
    }

    return 0;
}