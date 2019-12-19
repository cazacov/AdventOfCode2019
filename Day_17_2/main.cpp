#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include "IntcodeComputer.h"
#include "Screen.h"
#include "vector"
#include <unordered_map>

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

int next_input = 0;
long on_input() {
    return next_input;
}


IntcodeComputer computer;

int main() {
    Screen screen;
    computer.load_program("program.txt");

    std::unordered_map<Pos, int> map;

    int x=0;
    int y=0;
    while (!computer.is_halted()) {
        while (!computer.step(false, on_input)) {
            ;
        }
        char ch = computer.get_last_output();
        if (ch == '#') {
            Pos pos {x, y};
            map[pos] = 1;
        }
        if (ch == 10) {
            std::cout << std::endl;
            y++;
            x=0;
        }
        else {
            std::cout << ch;
            x++;
        }
    }
    int rows = y+1;
    int cols = x+1;

    int sum = 0;

    for (const auto &pair: map) {
        auto &pos = pair.first;
        if (pos.x == 0 || pos.y == 0 || pos.x == cols-1 || pos.y == rows -1) {
            continue;
        }
        bool is_intersection = true;
        for (int dir = 0; dir < 4; dir++) {
            auto neighbour = pos.move(dir);
            if (!map.count(neighbour)) {
                is_intersection = false;
                break;
            }
        }
        if (is_intersection) {
            sum+= pos.x * pos.y;
        }
    }

    std::cout << std::endl << sum << std::endl;
}

