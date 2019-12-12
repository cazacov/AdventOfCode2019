#include <iostream>
#include <map>
#include <algorithm>
#include <unordered_map>
#include "IntcodeComputer.h"

struct Panel {
    int x;
    int y;
    bool operator==(const Panel &other) const{
        return x == other.x && y == other.y;
    }
};
// Required by unordered_map
template<> struct std::hash<Panel> {
    std::size_t operator() (const Panel &node) const {
        return (hash<int>()(node.x) << 4) ^ hash<int>()(node.y);
    }
};

void robot_move(Panel &panel, int direction);

const int COLOR_BLACK = 0;
const int COLOR_WHITE = 1;

Panel current_panel {0,0};
std::unordered_map<Panel, int> hull;

long on_input() {
    return hull[current_panel];
}

int main() {

    IntcodeComputer computer;
    std::unordered_map<Panel, int> painted_panels;
    int direction = 0;

    computer.load_program("program.txt");
    int out[2];
    int out_pos = 0;
    while (!computer.is_halted()) {
        if (computer.step(false, on_input)) {
            out[out_pos++] = (int)computer.get_last_output();
            if (out_pos == 2) {
                // process robot command
                hull[current_panel] = out[0];
                painted_panels[current_panel]+=1;
                if (out[1] == 0) {
                    direction -= 1;
                } else {
                    direction += 1;
                }
                direction = (direction + 4) % 4;
                robot_move(current_panel, direction);
                out_pos = 0;
            }
        }
    }
    std::cout << painted_panels.size();
    return 0;
}

void robot_move(Panel &panel, int direction) {

    switch (direction) {
        case 0: // UP
            panel.y++;
            break;
        case 1: // Right
            panel.x++;
            break;
        case 2: // DOWN
            panel.y--;
            break;
        case 3: // LEFT
            panel.x--;
            break;
    }

}