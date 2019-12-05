#include <utility>

//
// Created by Victor on 02.12.2019.
//

#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
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

bool IntcodeComputer::step(bool trace) {

    int op1_addr, op2_addr, op3_addr;

    switch (ram[ip]) {
        case OPCODE_HALT:
            if (trace) {
                std::cout << "[" << std::setfill('0') << std::setw(3) << ip << "]\tHALT" << std::endl;
            }
            return false;
        case OPCODE_ADD:
            op1_addr = ram[ip+1];
            op2_addr = ram[ip+2];
            op3_addr = ram[ip+3];
            if (trace) {
                std::cout << "[" << std::setfill('0') << std::setw(4) << ip << "]\tADD ["
                        << std::setfill('0') << std::setw(3) << op1_addr << "] ["
                        << std::setfill('0') << std::setw(3) << op2_addr << "] => ["
                        << std::setfill('0') << std::setw(3) << op3_addr << "]";
                std::cout << "\t"
                          << std::setfill('0') << std::setw(3) << ram[op1_addr] << " + "
                          << std::setfill('0') << std::setw(3) << ram[op2_addr] << " = ";
            }

            do_add(op1_addr, op2_addr, op3_addr);

            if (trace) {
                std::cout << std::setfill('0') << std::setw(3) << ram[op3_addr] << std::endl;
            }
            ip+=4;
            return true;
        case OPCODE_MUL:
            op1_addr = ram[ip+1];
            op2_addr = ram[ip+2];
            op3_addr = ram[ip+3];
            if (trace) {
                std::cout << "[" << std::setfill('0') << std::setw(4) << ip << "]\tMUL ["
                          << std::setfill('0') << std::setw(3) << op1_addr << "] ["
                          << std::setfill('0') << std::setw(3) << op2_addr << "] => ["
                          << std::setfill('0') << std::setw(3) << op3_addr << "]";
                std::cout << "\t"
                          << std::setfill('0') << std::setw(3) << ram[op1_addr] << " * "
                          << std::setfill('0') << std::setw(3) << ram[op2_addr] << " = ";
            }

            do_mul(op1_addr, op2_addr, op3_addr);

            if (trace) {
                std::cout << std::setfill('0') << std::setw(3) << ram[op3_addr] << std::endl;
            }
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

void IntcodeComputer::set_input(std::vector<int> input) {
    input_buf = std::move(input);
}
