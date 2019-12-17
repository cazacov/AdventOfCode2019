#include <iostream>
#include <unordered_map>
#include <algorithm>
#include "IntcodeComputer.h"
#include "Screen.h"
#include "vector"

struct Pos {
    int x;
    int y;
    Pos move(int direction) {
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
};

// Required by unordered_map
template<> struct std::hash<Pos> {
    std::size_t operator() (const Pos &node) const {
        return (hash<int>()(node.x) << 4) ^ hash<int>()(node.y);
    }
};


Screen screen;
IntcodeComputer computer;

int next_direction = 0;
long on_input() {
    return next_direction;
}

Tile try_move(int direction);


void explore(std::unordered_map<Pos, Tile> map, Pos robot) {

    static int moves[4][2] { {0,1}, {0,-1}, {-1,0}, {1,0}};
    for (int dir = 0; dir < 4; dir++) {




        Tile result = try_move(di)

    }
}




int main() {
    computer.load_program("program.txt");


    std::unordered_map<Pos, Tile> map;

    std::vector<int> path;



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
            pos+=i;
            break;
        }
        pos++;
    }

    int columns = 16;
    int len = 890;
    std::string str;

    while (columns < 80) {
        screen.cls();
        for (int i = 0; i < len; i++) {
            int x = i % columns + 1;
            int y = i / columns + 1;
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
                default:
                    screen.draw(x, y, Tile::Robot);
            }
        }
        getchar();
        columns++;
    }


    return 0;
}


Tile try_move(int direction) {

    Tile result;
    next_direction = direction;


    while (!computer.step(false, on_input)) {
        ;
    }
    switch(computer.get_last_output()) {
        case 0:
            result = Tile::Wall;
            break;
        case 1:
            result = Tile::Blank;
            break;
        case 2:
            result = Tile::Oxygen;
            break;
    }
    return result;
}
