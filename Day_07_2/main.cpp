#include <iostream>
#include <map>
#include <algorithm>
#include "IntcodeComputer.h"

IntcodeComputer computer();

const int PHASE_COUNT = 5;
const int AMPLIFIER_COUNT = 5;


int main() {

    IntcodeComputer computers[AMPLIFIER_COUNT];
    for (int i = 0; i < AMPLIFIER_COUNT; i++) {
        computers[i].load_program("program.txt");
    }

    std::vector<int> phases(AMPLIFIER_COUNT);
    for (int i = 0; i < AMPLIFIER_COUNT; i++) {
        phases[i] = i+5;
    }

    int max_thrust = 0;

    do {
        for (int i = 0; i < AMPLIFIER_COUNT; i++) {
            std::cout << phases[i] << " ";
        }

        for (int i = 0; i < AMPLIFIER_COUNT; i++) {
            IntcodeComputer* computer = &computers[i];
            computer->reset();
            computer->add_to_input(phases[i]);
        }

        int signal = 0;
        int comp = 0;
        while (!computers[AMPLIFIER_COUNT-1].is_halted()) {
            IntcodeComputer* computer = &computers[comp];
            computer->add_to_input(signal);
            while (!computer->step(false) && !computer->is_halted()) {
                ;
            }
            signal = computer->get_last_output();
            comp++;
            if (comp == AMPLIFIER_COUNT) {
                comp = 0;
            }
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