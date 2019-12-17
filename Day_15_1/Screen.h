//
// Created by victor on 13.12.19.
//

#ifndef DAY_13_1_SCREEN_H
#define DAY_13_1_SCREEN_H

#include <string>

enum Tile {
    Blank,
    Wall,
    Oxygen,
    Robot
};


class Screen {
private:
    void print_at(int x, int y, std::string str);
public:
    void cls();
    void draw(int x, int y, Tile tile);
    Screen() { this->cls(); }
    void text(int x, int y, std::string text);
};


#endif //DAY_13_1_SCREEN_H
