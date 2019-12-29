//
// Created by Victor on 02.12.2019.
//

#ifndef DAY_02_01_INTCODECOMPUTER_H
#define DAY_02_01_INTCODECOMPUTER_H

#define RAM_SIZE 1000000

enum ExecutionResult {
    Halt,
    Output,
    Running
};

#include <vector>
#include <functional>
#include "Command.h"

class IntcodeComputer {
private:
    std::vector<long> input_buf;
    int input_pos = 0;
    std::vector<long> output_buf;
    Command load_next_command();
    int ip;
    int base;
    std::vector<long> ram_snapshot;
    bool i_am_halted;
public:
    IntcodeComputer() {reset();};
    std::vector<long> ram;
    void load_program(const char* file_name);
    bool step(bool trace, std::function<long(int)> input_required, int param);
    void set_input(std::vector<long> input);
    void add_to_input(long value) { input_buf.push_back(value);}
    long get_last_output() {
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
