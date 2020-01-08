//
// Created by Victor on 06.01.2020.
//

#include "AsciiComputer.h"

AsciiComputer::AsciiComputer() {
    computer.load_program("program.txt");
}

std::string AsciiComputer::run(std::string input) {
    const std::string command_str = "Command?\x0A";

    out_buf = "";

    if (!input.empty()) {
        std::vector<int64_t> data(input.begin(), input.end());
        computer.set_input(data);
        computer.add_to_input(10);  // CR
    }

    while (!computer.is_halted()) {
        bool has_output = computer.step(false, nullptr,0);
        if (has_output) {
            out_buf += char(computer.get_last_output());
        }
        if (out_buf.length() > command_str.length()) {
            if (std::equal(command_str.rbegin(), command_str.rend(), out_buf.rbegin())) {
                return out_buf.substr(0, out_buf.length() - command_str.length());
            }
        }
    }
    return "Computer halted";
}
