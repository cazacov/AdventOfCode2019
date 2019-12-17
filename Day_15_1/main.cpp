#include <iostream>
#include <unordered_map>
#include <algorithm>
#include "IntcodeComputer.h"
#include "Screen.h"
#include "vector"

struct Pos {
    int x;
    int y;
};

Screen screen;
IntcodeComputer computer;

Pos robot;

int next_direction = 0;
long on_input() {
    return next_direction;
}


int main() {
    computer.load_program("program.txt");
    std::vector<long> s {21201,1,1519,-2, 109,-3,2106,0, 0};
    int pos = 500;
    while (pos < computer.ram.size()-10) {
        int i;
        for (i = 0; i < s.size(); i++) {
            if (computer.ram[pos+i] != s[i]) {
                break;
            }
        }
        if (i == s.size()) {
            std::cout << (pos + s.size()) << std::endl;
            break;
        }
        pos++;
    }

    int columns = 16;
    int len = 880;
    std::string str;

    while (columns < 80) {
        screen.cls();
        for (int i = 0; i < len; i++) {
            int x = i % columns;
            int y = i / columns;
            switch(computer.ram[pos + i]) {
                case 0:
                    screen.draw(x, y, Tile::Blank);
                    break;
                case 1:
                    screen.draw(x, y, Tile::Wall);
                    break;
                case 2:
                    screen.draw(x, y, Tile::Oxygen);
                    break;
            }
        }
        std::cin >> str;
        columns++;
    }


    return 0;
}

/*
Tile try_move(int direction) {

static int moves[4][2] { {0,1}, {0,-1}, {-1,0}, {1,0}};

    Tile result;
    next_direction = direction;
    if (computer.step(false, on_input)) {
        switch(computer.get_last_output()) {
            case 0:
                result = Tile::Wall;
                break;
            case 1:
        }

    }
}
*/
