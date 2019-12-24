#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include "IntcodeComputer.h"
#include "Screen.h"
#include "vector"
#include <unordered_map>
#include <iterator>
#include <sstream>

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
                result.x +=1;
                break;
            case 2:
                result.y +=1;
                break;
            case 3:
                result.x -=1;
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

    int x=1;
    int y=1;
    Pos robot;
    screen.cls();
    while (!computer.is_halted()) {
        while (!computer.step(false, on_input)) {
            ;
        }
        char ch = computer.get_last_output();
        Pos pos {x, y};
        if (ch == '#') {
            map[pos] = 1;
        }
        if (ch == '^') {
            robot = pos;
        }
        if (ch != 10) {
            screen.text(x, y, std::string(1,ch));
        }
        if (ch == 10) {
            y++;
            x=1;
        }
        else {
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

    screen.text(1,40, " ");
    std::cout << std::endl << "Sum of the alignment parameters: " << sum << std::endl;

    std::vector<std::string> path;
    int dir = 0;
    Pos robot_pos = robot;
    do {
        int steps = 0;
        auto forward = robot_pos.move(dir);

        while (map.count(forward)) {
//            screen.text(forward.x, forward.y, "o");
          robot_pos = forward;
            steps++;
            forward = robot_pos.move(dir);
        }
        if (steps) {
            path.push_back(std::to_string(steps));
        }

        auto left = robot_pos.move((dir+3) % 4);
        if (map.count(left)) {
            path.push_back("L");
            dir = (dir+3) % 4;
            continue;
        }
        auto right = robot_pos.move((dir+1) % 4);
        if (map.count(right)) {
            path.push_back("R");
            dir = (dir+1) % 4;
            continue;
        }
        break;
    } while(true);

    std::stringstream  s;
    copy(path.begin(),path.end(), std::ostream_iterator<std::string>(s, ","));

    std::cout << "Path: " << s.str() << std::endl;
    return 0;

}

