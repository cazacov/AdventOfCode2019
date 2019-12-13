#include <iostream>
#include <map>
#include <algorithm>
#include "IntcodeComputer.h"
#include "Screen.h"

long on_input() {
    return 0;
}

int main() {

    Screen screen;

    int block_tiles = 0;

    IntcodeComputer computer;
    computer.load_program("program.txt");
    int out[3];
    int out_pos = 0;
    while (!computer.is_halted()) {
        if (computer.step(false, on_input)) {
            out[out_pos++] = (int)computer.get_last_output();
            if (out_pos == 3) {
                screen.draw(out[0], out[1], (Tile)out[2]);
                if (out[2] == Tile::Block) {
                    block_tiles++;
                }
                out_pos = 0;
            }
        }
    }
    screen.text(1, 20, std::to_string(block_tiles));
    return 0;
}

