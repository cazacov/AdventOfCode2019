//
// Created by Victor on 02.12.2019.
//

#include <fstream>
#include <sstream>
#include "IntcodeComputer.h"

#define OPCODE_ADD 1
#define OPCODE_MUL 2
#define OPCODE_HALT 99

void IntcodeComputer::load_program(const char *file_name) {

    std::vector<int> result;
    ram.clear();
    ip = 0;

    std::ifstream  data(file_name);
    std::string line;
    while(std::getline(data,line))
    {
        line.erase(line.find_last_not_of("0123456789,") + 1);
        std::stringstream lineStream(line);
        std::string opcode ;
        while(std::getline(lineStream, opcode ,','))
        {
            ram.push_back(stoi(opcode));
        }
    }
}

bool IntcodeComputer::execute() {

    int op1_addr, op2_addr, op3_addr;

    switch (ram[ip]) {
        case OPCODE_HALT:
            return false;
        case OPCODE_ADD:
            op1_addr = ram[ip+1];
            op2_addr = ram[ip+2];
            op3_addr = ram[ip+3];
            do_add(op1_addr, op2_addr, op3_addr);
            ip+=4;
            return true;
        case OPCODE_MUL:
            op1_addr = ram[ip+1];
            op2_addr = ram[ip+2];
            op3_addr = ram[ip+3];
            do_mul(op1_addr, op2_addr, op3_addr);
            ip+=4;
            return true;
        default:
            return false;
    }
}

void IntcodeComputer::do_add(int op1_addr, int op2_addr, int result_addr) {
    ram[result_addr] = ram[op1_addr] + ram[op2_addr];
}

void IntcodeComputer::do_mul(int op1_addr, int op2_addr, int result_addr) {
    ram[result_addr] = ram[op1_addr] * ram[op2_addr];
}
