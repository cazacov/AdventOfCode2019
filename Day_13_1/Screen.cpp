#include <utility>

#include "Screen.h"

void Screen::print_at(int x, int y, std::string str) {
    printf("\033[%d;%dH%s", y, x, str.c_str());
    fflush(stdout);
}

void Screen::draw(int x, int y, Tile tile) {
    switch (tile) {
        case Blank:
            print_at(x*2, y, "  ");
            break;
        case Wall:
            print_at(x*2, y, "##");
            break;
        case Block:
            print_at(x*2, y, "[]");
            break;
        case Paddle:
            print_at(x*2, y, "==");
            break;
        case Ball:
            print_at(x*2, y, "()");
            break;
    }
}

void Screen::text(int x, int y, std::string text) {
    print_at(x, y, std::move(text));
}

void Screen::cls() {
    printf("\033[2J");
    fflush(stdout);
}
