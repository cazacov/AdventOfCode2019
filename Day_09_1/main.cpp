#include <iostream>
#include <map>
#include <algorithm>
#include "IntcodeComputer.h"

int main() {

    IntcodeComputer computer;
    computer.load_program("program.txt");
    computer.add_to_input(2);

    do  {
        bool has_out = computer.step(false);
        if (has_out) {
            std::cout << computer.get_last_output() << std::endl;
        }
    } while (!computer.is_halted());

    return 0;
}