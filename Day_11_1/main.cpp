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

void render_identifier(std::unordered_map<Panel, int> &hull);

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

    hull[current_panel] = COLOR_WHITE;

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

    render_identifier(hull);

    std::cout << painted_panels.size();
    return 0;
}

void render_identifier(std::unordered_map<Panel, int> &hull) {

    // find corner panels

    Panel upper_left { INT32_MAX, INT32_MIN};
    Panel right_down { INT32_MIN, INT32_MAX};

    for (const auto &pair : hull) {
        int x = pair.first.x;
        int y = pair.first.y;

        if (x < upper_left.x) {
            upper_left.x = x;
        }
        if (x > right_down.x) {
            right_down.x = x;
        }
        if (y > upper_left.y) {
            upper_left.y = y;
        }
        if (y < right_down.y) {
            right_down.y = y;
        }
    }

    for (int y = upper_left.y; y >= right_down.y; y--) {
        for (int x = upper_left.x; x <= right_down.x; x++) {
            Panel p {x,y};
            if (hull.count(p)) {
                if (hull[p] == COLOR_WHITE) {
                    std::cout << "#";
                }
                else {
                    std::cout << " ";
                }
            }
            else {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }
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