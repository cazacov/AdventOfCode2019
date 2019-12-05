#include <iostream>
#include "IntcodeComputer.h"

int main() {

    IntcodeComputer ship_computer(true);
    ship_computer.load_program("program.txt");

    ship_computer.set_input({1});

    while (ship_computer.step(true)) {
        ;
    }

    std::cout << ship_computer.get_last_output() << std::endl;
    return 0;
}