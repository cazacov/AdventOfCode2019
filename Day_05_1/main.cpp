#include <iostream>
#include "IntcodeComputer.h"

int main() {

    IntcodeComputer ship_computer;
    ship_computer.load_program("program.txt");

    ship_computer.ram[1] = 12;
    ship_computer.ram[2] = 2;

    while (ship_computer.execute(true)) {
        ;
    }

    std::cout << ship_computer.ram[0] << std::endl;
    return 0;
}