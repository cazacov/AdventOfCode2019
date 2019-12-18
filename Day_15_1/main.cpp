#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <numeric>
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

Tile try_move(int direction);


int find_shortest_way(const std::unordered_map<Pos, Tile> &map, Pos from, Pos to);
int time_to_fill_all(const std::unordered_map<Pos, Tile> &map, Pos from);

void explore(std::unordered_map<Pos, Tile> &map, Pos robot, Screen &screen) {
    for (int dir = 0; dir < 4; dir++) {
        auto newPos = robot.move(dir);
        if (map.count(newPos)) {    // We already know what's in that field
            continue;
        }
        Tile result = try_move(dir);
        map[newPos] = result;
        screen.draw(newPos.x, newPos.y, result);
        if (result != Tile::Wall) {
            explore(map, newPos, screen);
            int back_dir = dir ^ 0x01;
            try_move(back_dir);
        }
    }
}

IntcodeComputer computer;

int main() {
    Screen screen;
    computer.load_program("program.txt");

    Pos robot {22,22};

    std::unordered_map<Pos, Tile> map;
    explore(map, Pos {22,22}, screen);
    screen.text(robot.x, robot.y, "D");

    Pos oxygen;
    for (const auto &pair: map) {
        if (pair.second == Tile::Oxygen) {
            oxygen = pair.first;
        }
    }

    int path_length = find_shortest_way(map, robot, oxygen);
    screen.text(40,1, std::to_string(path_length));

    screen.text(40,2, std::to_string(time_to_fill_all(map,oxygen)));

    return 0;
}

int find_shortest_way(const std::unordered_map<Pos, Tile> &map, Pos from, Pos to) {


    // Dijkstra's algorithm
    // https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm


    std::unordered_map<Pos, int> unvisited_set;

    for (const auto &pair : map) {
        if (pair.second != Tile::Wall) {
            unvisited_set[pair.first]  = INT32_MAX;
        }
    }
    unvisited_set[from] = 0;

    auto current_node = from;
    while (current_node != to) {
        int new_distance = unvisited_set[current_node] + 1;
        for (int dir = 0; dir < 4; dir++) {
            auto neighbour = current_node.move(dir);
            if (!unvisited_set.count(neighbour)) {
                continue;
            }
            if (unvisited_set[neighbour] > new_distance) {
                unvisited_set[neighbour] = new_distance;
            }
        }
        unvisited_set.erase(current_node);

        int min_distance = INT32_MAX;
        for (const auto &pair : unvisited_set) {
            if (pair.second < min_distance) {
                current_node = pair.first;
                min_distance = pair.second;
            }
        }
    }
    return unvisited_set[current_node];
}


Tile try_move(int direction) {

    Tile result = Tile::Unknown;
    next_input = direction + 1;

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

int time_to_fill_all(const std::unordered_map<Pos, Tile> &map, Pos from) {


    // Dijkstra's algorithm
    // https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm


    std::unordered_map<Pos, int> unvisited_set;

    int max_distance = 0;

    for (const auto &pair : map) {
        if (pair.second != Tile::Wall) {
            unvisited_set[pair.first]  = INT32_MAX;
        }
    }
    unvisited_set[from] = 0;

    auto current_node = from;
    while (!unvisited_set.empty()) {
        int new_distance = unvisited_set[current_node] + 1;
        for (int dir = 0; dir < 4; dir++) {
            auto neighbour = current_node.move(dir);
            if (!unvisited_set.count(neighbour)) {
                continue;
            }
            if (unvisited_set[neighbour] > new_distance) {
                unvisited_set[neighbour] = new_distance;
            }
        }
        if (unvisited_set[current_node] > max_distance) {
            max_distance = unvisited_set[current_node];
        }
        unvisited_set.erase(current_node);

        if (unvisited_set.empty()) {
            break;
        }

        int min_distance = INT32_MAX;
        for (const auto &pair : unvisited_set) {
            if (pair.second < min_distance) {
                current_node = pair.first;
                min_distance = pair.second;
            }
        }
    }
    return max_distance;
}
