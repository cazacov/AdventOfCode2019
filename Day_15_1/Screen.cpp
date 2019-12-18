#include <utility>

#include "Screen.h"

void Screen::print_at(int column, int row, std::string str) {
    if (column > 0 && row > 0) {
        printf("\033[%d;%dH%s", row, column, str.c_str());
        fflush(stdout);
    }
}

void Screen::draw(int x, int y, Tile tile) {
    switch (tile) {
        case Blank:
            print_at(x, y, ".");
            break;
        case Wall:
            print_at(x, y, "#");
            break;
        case Oxygen:
            print_at(x, y, "*");
            break;
        case Robot:
            print_at(x, y, "@");
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
