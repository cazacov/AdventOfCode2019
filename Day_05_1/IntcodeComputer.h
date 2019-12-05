//
// Created by Victor on 02.12.2019.
//

#ifndef DAY_02_01_INTCODECOMPUTER_H
#define DAY_02_01_INTCODECOMPUTER_H

#include <vector>

class IntcodeComputer {
private:
    int ip;
public:
    std::vector<int> ram;
    void load_program(const char* file_name);
    bool execute(bool trace = false);
    void do_add(int op1_addr, int op2_addr, int result_addr);
    void do_mul(int op1_addr, int op2_addr, int result_addr);
};


#endif //DAY_02_01_INTCODECOMPUTER_H
