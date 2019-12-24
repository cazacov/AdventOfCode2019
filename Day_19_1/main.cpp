#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include "IntcodeComputer.h"
#include "Screen.h"
#include "vector"
#include <unordered_map>

using namespace std;

struct Pos {
    int x;
    int y;
    Pos move(int direction) const {
        Pos result {x, y};
        switch (direction) {
            case 0:
                result.y -=1;
                break;
            case 1:
                result.y +=1;
                break;
            case 2:
                result.x -=1;
                break;
            case 3:
                result.x +=1;
                break;
        }
        return result;
    }

    bool operator==(const Pos &other) const{
        return x == other.x && y == other.y;
    }

    bool operator!=(const Pos &other) const{
        return x != other.x || y != other.y;
    }
};

// Required by unordered_map
template<> struct std::hash<Pos> {
    std::size_t operator() (const Pos &node) const {
        return (hash<int>()(node.x) << 4) ^ hash<int>()(node.y);
    }
};

long get_status(IntcodeComputer &computer, int x, int y);

int next_input = 0;
long on_input() {
    return next_input;
}


IntcodeComputer computer;

int main() {
    Screen screen;
    computer.load_program("program.txt");
    std::unordered_map<Pos, bool> map;
    screen.cls();

    int affected_points = 0;

    int top = 0;
    for (int x = 0; x < 50; x++) {
        bool beam_found = false;
        for (int y = 0; y < 50; y++) {
            long result = get_status(computer, x, y);
            if (result == 0) {
                screen.text(x+1,y+1, ".");
                if (beam_found) {
                    break;
                }
                else if (top < y) {
                    top = y;
                }
            }
            else {
                beam_found = true;
                affected_points++;
                screen.text(x+1,y+1, "#");
            }
        }
    }
    cout << endl << affected_points;
}

long get_status(IntcodeComputer &computer, int x, int y) {

    computer.reset();
    computer.set_input(vector<long> { x, y});
    while (!computer.is_halted() && !computer.step(false, on_input)) {
        ;
    }

    if (computer.is_halted()) {
        return -1;
    }
    else {
        return computer.get_last_output();
    }
}

