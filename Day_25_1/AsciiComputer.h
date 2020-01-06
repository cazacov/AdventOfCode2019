//
// Created by Victor on 06.01.2020.
//

#ifndef DAY_25_1_ASCIICOMPUTER_H
#define DAY_25_1_ASCIICOMPUTER_H

#include "IntcodeComputer.h"

enum Direction {
    North,
    East,
    South,
    West
};



class AsciiComputer {
private:
    IntcodeComputer computer;
    std::string out_buf;
public:
    AsciiComputer();
    std::string run();
    std::string move(Direction direction);
    std::string take(std::string item);
    std::string drop(std::string item);
    std::vector<std::string> list();
};
#endif //DAY_25_1_ASCIICOMPUTER_H
