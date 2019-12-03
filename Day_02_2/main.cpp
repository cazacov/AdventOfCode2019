#include <iostream>

int main() {

    /* By "disassembling" the opcodes on a sheet of paper it can
     * be shown that the Intcode program calculates the expression
     * 288000 * NOUN + 394702 + VERB
     */

    const int TARGET =  19690720;

    auto noun = (TARGET - 394702) / 288000;
    auto verb = (TARGET - 394702) % 288000;

    std::cout << 100 * noun + verb << std::endl;
}