#include <iostream>
#include <map>
#include <algorithm>
#include "IntcodeComputer.h"

IntcodeComputer computer(false);

const int PHASE_COUNT = 5;
const int AMPLIFIER_COUNT = 5;

std::vector<std::map<int, int>> cache(PHASE_COUNT);

int calculate_output(int phase, int input) {

    if (!cache[phase].count(input)) {
        computer.reset();
        std::vector<int> inp { phase, input};
        computer.set_input(inp);

        while (computer.step()) {
            ;
        }
        cache[phase][input] =  computer.get_last_output();
    }
    return cache[phase][input];
}


int main() {
    computer.load_program("program.txt");

    std::vector<int> phases(AMPLIFIER_COUNT);
    for (int i = 0; i < AMPLIFIER_COUNT; i++) {
        phases[i] = i;
    }

    int max_thrust = 0;

    do {
        for (int i = 0; i < PHASE_COUNT; i++) {
            std::cout << phases[i] << " ";
        }
        int signal = 0;
        for (int i = 0; i < AMPLIFIER_COUNT; i++) {
            signal = calculate_output(phases[i], signal);
        }

        std::cout << "=> " << signal;
        if (signal > max_thrust) {
            max_thrust = signal;
            std::cout << "\t MAX!" << std::endl;
        }
        else {
            std::cout << std::endl;
        }

        // go to next permutation of phases

        // find the first pair is not in the reverse order
        int pos = AMPLIFIER_COUNT - 2;
        while (phases[pos] > phases[pos+1] && pos >= 0) {
            pos--;
        }
        if (pos < 0) {
            break;
        }

        // find first from that is greater than element in position pos
        int pos2 = AMPLIFIER_COUNT-1;
        while (phases[pos2] < phases[pos]) {
            pos2--;
        }

        std::swap(phases[pos], phases[pos2]);

        // sort remaining elements
        std::sort(phases.begin()+pos+1, phases.end());
    } while (true);

    std::cout << "Max thrust: " << max_thrust << std::endl;
    return 0;
}