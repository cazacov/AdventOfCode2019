//
// Created by Victor on 06.01.2020.
//

#include "AsciiComputer.h"

AsciiComputer::AsciiComputer() {
    computer.load_program("program.txt");
}

std::string AsciiComputer::run() {
    const std::string command_str = "Command?";

    out_buf = "";

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
}
