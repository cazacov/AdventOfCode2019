#include <iostream>
#include <map>
#include <algorithm>
#include <zconf.h>
#include "IntcodeComputer.h"
#include "Screen.h"

int last_ball_x = 0;
int last_paddle_x = 0;


long on_input() {
    usleep(2000);
    if (last_ball_x < last_paddle_x) {
        return -1;
    } else if (last_ball_x > last_paddle_x) {
        return 1;
    }
    else {
        return 0;
    }
}

int main() {

    Screen screen;

    IntcodeComputer computer;
    computer.load_program("program.txt");

    computer.ram[0] = 2; // play for free

    int out[3];
    int out_pos = 0;
    while (!computer.is_halted()) {
        if (computer.step(false, on_input)) {
            out[out_pos++] = (int)computer.get_last_output();
            if (out_pos == 3) {
                if (out[0] == -1 && out[1] == 0) {  // show new score
                    screen.text(1, 20, std::to_string(out[2]));
                } else {
                    screen.draw(out[0], out[1], (Tile) out[2]);
                }
                if (out[2] == Tile::Ball) {
                    last_ball_x = out[0];
                }
                if (out[2] == Tile::Paddle) {
                    last_paddle_x = out[0];
                }
                out_pos = 0;
            }
        }
    }
    return 0;
}

