#include <utility>

//
// Created by Victor on 02.12.2019.
//

#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "IntcodeComputer.h"
#include "Command.h"


void IntcodeComputer::load_program(const char *file_name) {

    std::vector<int> result;
    ram.clear();
    ip = 0;

    std::ifstream  data(file_name);
    std::string line;
    while(std::getline(data,line))
    {
        int comment_idx = line.find_last_not_of("0123456789,-");
        if (comment_idx > 0) {
            line.erase(comment_idx + 1);
        }
        std::stringstream lineStream(line);
        std::string opcode ;
        while(std::getline(lineStream, opcode ,','))
        {
            ram.push_back(stoi(opcode));
        }
    }
    ram_snapshot = ram;
}

bool IntcodeComputer::step() {

    if (trace) {
        std::cout << std::setfill('0') << std::setw(4) << ip << "\t";
    }

    Command next_command = load_next_command();

    int result = 0;
    std::string cmd;
    if (trace) {
        for (int i = 0; i < next_command.param_count; i++) {
            if (next_command.is_position_mode[i]) {
                std::cout << "[" << std::setw(8) << next_command.parameter_values[i] << "]\t";
            } else {
                std::cout << " " << std::setw(8) << next_command.parameter_values[i] << " \t";
            }
        }
    }

    switch(next_command.opcode) {
        case OPCODE_HALT:
            if (trace) {
                std::cout << "HALT" << std::endl;
            }
            return false;
        case OPCODE_ADD:
            result = next_command.parameter_values[0] + next_command.parameter_values[1];
            cmd = "ADD";
            break;
        case OPCODE_MUL:
            result = next_command.parameter_values[0] * next_command.parameter_values[1];
            cmd = "MUL";
            break;
        case OPCODE_IN:
            result = input_buf[input_pos++];
            cmd = "IN";
            break;
        case OPCODE_OUT:
            cmd = "OUT";
            result = next_command.parameter_values[0];
            break;
        case OPCODE_JUMP_IF_TRUE:
            cmd = "JMP True";
            if ( next_command.parameter_values[0] != 0) {
                result = next_command.parameter_values[1];
            }
            else {
                next_command.destination = ResultDestination::NONE;
            }
            break;
        case OPCODE_JUMP_IF_FALSE:
            cmd = "JMP False";
            if ( next_command.parameter_values[0] == 0) {
                result = next_command.parameter_values[1];
            }
            else {
                next_command.destination = ResultDestination::NONE;
            }
            break;
        case OPCODE_LESS_THAN:
            result = (next_command.parameter_values[0] < next_command.parameter_values[1]) ? 1 : 0;
            cmd = "LESS";
            break;
        case OPCODE_EQUALS:
            result = (next_command.parameter_values[0] == next_command.parameter_values[1]) ? 1 : 0;
            cmd = "EQU";
            break;

    }

    if (trace) {
        std::cout << cmd << "\t";
    }

    if (next_command.destination == ResultDestination::RAM) {
        ram[next_command.result_addr] = result;
        if (trace) {
            std::cout << "[" << std::setw(4) << next_command.result_addr << "] (" << std::setw(8) << result << ")"
                      << std::endl;
        }
    }
    if (next_command.destination == ResultDestination::OUT) {
        output_buf.push_back(result);
        if (trace) {
            std::cout << " OUTPUT: " << result << std::endl;
        }
    }
    else if (next_command.destination == ResultDestination::IP) {
        if (trace) {
            std::cout << " GOTO: " << result << std::endl;
        }
        ip = result;
    }
    return true;
}

void IntcodeComputer::set_input(std::vector<int> input) {
    input_buf = std::move(input);
}

Command IntcodeComputer::load_next_command() {

    int flags = ram[ip] / 100;

    Command result;

    for (int i = 0; i < 2; i++) {
        result.is_position_mode[i] = !(flags % 10);
        flags/=10;
    }

    result.opcode = ram[ip] % 100;
    result.destination = ResultDestination::RAM;
    result.param_count = 0;

    int command_length = 0;

    switch (result.opcode) {
        case OPCODE_HALT:
            command_length = 1;
            break;
        case OPCODE_ADD:
            result.param_count = 2;
            command_length = 4;
            break;
        case OPCODE_MUL:
            result.param_count = 2;
            command_length = 4;
            break;
        case OPCODE_IN:
            result.param_count = 0;
            command_length = 2;
            break;
        case OPCODE_OUT:
            result.param_count = 1;
            result.destination = ResultDestination::OUT;
            command_length = 2;
            break;
        case OPCODE_JUMP_IF_TRUE:
            result.param_count = 2;
            result.destination = ResultDestination::IP;
            command_length = 3;
            break;
        case OPCODE_JUMP_IF_FALSE:
            result.param_count = 2;
            result.destination = ResultDestination::IP;
            command_length = 3;
            break;
        case OPCODE_LESS_THAN:
            result.param_count = 3;
            result.destination = ResultDestination::RAM;
            command_length = 4;
            break;
        case OPCODE_EQUALS:
            result.param_count = 3;
            result.destination = ResultDestination::RAM;
            command_length = 4;
            break;
    }

    for (int i = 0; i < result.param_count; i++) {
        if (result.is_position_mode[i]) {
            result.parameter_values[i] = ram[ram[ip+1+i]];
        }
        else {
            result.parameter_values[i] = ram[ip+1+i];
        }
    }
    if (result.destination == ResultDestination::RAM) {
        result.result_addr = ram[ip + command_length -1];
    }

    ip+= command_length;
    return result;
}

void IntcodeComputer::reset() {
    ram = ram_snapshot;
    ip = 0;
    input_pos = 0;
    input_buf.clear();
    output_buf.clear();
}
