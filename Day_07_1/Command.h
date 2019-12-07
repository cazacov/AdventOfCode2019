//
// Created by vcazacov on 05.12.2019.
//

#ifndef DAY_05_1_COMMAND_H
#define DAY_05_1_COMMAND_H

#define OPCODE_ADD 1
#define OPCODE_MUL 2
#define OPCODE_IN 3
#define OPCODE_OUT 4
#define OPCODE_JUMP_IF_TRUE 5
#define OPCODE_JUMP_IF_FALSE 6
#define OPCODE_LESS_THAN 7
#define OPCODE_EQUALS 8
#define OPCODE_HALT 99

enum ResultDestination { RAM, OUT, IP, NONE };


struct Command {
    int opcode;
    int parameter_values[2] = {0};
    bool is_position_mode[2] = {0};
    int result_addr = {0};
    ResultDestination destination;
    int param_count;
};


#endif //DAY_05_1_COMMAND_H
