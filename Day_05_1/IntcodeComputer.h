//
// Created by Victor on 02.12.2019.
//

#ifndef DAY_02_01_INTCODECOMPUTER_H
#define DAY_02_01_INTCODECOMPUTER_H

#include <vector>

class IntcodeComputer {
private:
    std::vector<int> input_buf;
    std::vector<int> output_buf;

    int ip;
    void do_add(int op1_addr, int op2_addr, int result_addr);
    void do_mul(int op1_addr, int op2_addr, int result_addr);
public:
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
