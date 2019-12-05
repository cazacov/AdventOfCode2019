//
// Created by vcazacov on 05.12.2019.
//

#ifndef DAY_05_1_COMMAND_H
#define DAY_05_1_COMMAND_H

#define OPCODE_ADD 1
#define OPCODE_MUL 2
#define OPCODE_IN 3
#define OPCODE_OUT 4
#define OPCODE_HALT 99

struct Command {
    int opcode;
    int parameter_values[2] = {0};
    bool is_position_mode[2] = {0};
    int result_addr = {0};
    bool result_to_ram;
    int param_count;
};


#endif //DAY_05_1_COMMAND_H
