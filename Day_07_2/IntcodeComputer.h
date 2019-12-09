//
// Created by Victor on 02.12.2019.
//

#ifndef DAY_02_01_INTCODECOMPUTER_H
#define DAY_02_01_INTCODECOMPUTER_H

enum ExecutionResult {
    Halt,
    Output,
    Running
};

#include <vector>
#include "Command.h"

class IntcodeComputer {
private:
    std::vector<int> input_buf;
    int input_pos = 0;
    std::vector<int> output_buf;
    Command load_next_command();
    int ip;
    std::vector<int> ram_snapshot;
    bool i_am_halted;
public:
    IntcodeComputer() {};
    std::vector<int> ram;
    void load_program(const char* file_name);
    bool step(bool trace);
    void set_input(std::vector<int> input);
    void add_to_input(int value) { input_buf.push_back(value);}
    int get_last_output() {
        if (output_buf.empty()) {
            return 0;
        }
        else {
            return *output_buf.rbegin();
        }
    }
    void reset();
    bool is_halted() { return i_am_halted;}
};


#endif //DAY_02_01_INTCODECOMPUTER_H
