//
// Created by Victor on 02.12.2019.
//

#ifndef DAY_02_01_INTCODECOMPUTER_H
#define DAY_02_01_INTCODECOMPUTER_H

#include <vector>
#include "Command.h"

class IntcodeComputer {
private:
    bool trace;
    std::vector<int> input_buf;
    int input_pos = 0;

    std::vector<int> output_buf;
    Command load_next_command();

    int ip;
public:
    IntcodeComputer(bool trace_enabled) : trace(trace_enabled) {};
    std::vector<int> ram;
    void load_program(const char* file_name);
    bool step(bool trace = false);
    void set_input(std::vector<int> input);
    int get_last_output() {
        if (output_buf.empty()) {
            return 0;
        }
        else {
            return *output_buf.rbegin();
        }
    }


};


#endif //DAY_02_01_INTCODECOMPUTER_H
