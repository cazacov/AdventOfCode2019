#include <iostream>

int calculate(int param1, int param2);

int main() {
    const int TARGET =  19690720;
    for (auto verb = 0; verb < 100; verb++) {
        for (auto noun = 0; noun < 100; noun++) {
            auto result = calculate(noun, verb);
            if (result == TARGET) {
                std::cout << 100* noun + verb << std::endl;
                exit(1);
            }
            if (result > TARGET) {  // function is monotonically increasing
                break;
            }
        }
    }
    return 0;
}

// "Disassembled" on a sheet of paper by carefully reading the program opcodes
int calculate(int param1, int param2) {

    const int VAR5 = 1;
    const int VAR6 = 2;
    const int VAR9 = 3;
    const int VAR10 = 4;
    const int VAR13 = 5;

    return ((((((((VAR13 * param1 + VAR6) * VAR6
            + VAR5 + VAR10) * VAR6
            + VAR13 + VAR9) * VAR10
            + VAR5 + VAR10) * VAR6 * VAR6
            + VAR5) * VAR9
            + VAR6 + VAR6) * VAR9 * VAR13
            + VAR10 + VAR13)*VAR13
            + VAR10)*VAR10
            + VAR9 + param2 + VAR9;
}