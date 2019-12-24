#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include "IntcodeComputer.h"
#include "Screen.h"
#include "vector"
#include <unordered_map>

using namespace std;

struct BeamWidth {
    int from;
    int to;
};


long get_status(IntcodeComputer &computer, int x, int y);

int next_input = 0;
long on_input() {
    return next_input;
}


IntcodeComputer computer;

int main() {
    Screen screen;
    computer.load_program("program.txt");
    vector<BeamWidth> beam;
    screen.cls();

    int affected_points = 0;

    int top = 0;
    for (int x = 0; x < 500000; x++) {
        BeamWidth bw;
        bool beam_found = false;
        int limit_from = 0;
        int limit_to = x;
        if (x > 10) {
            limit_from = top;
            limit_to = INT32_MAX;
        }
        for (int y = limit_from; y < limit_to; y++) {
            long result = get_status(computer, x, y);
            if (result == 0) {
//                screen.text(x+1,y+1, ".");
                if (beam_found) {
                    bw.to = y - 1;
                    break;
                }
                else if (top < y) {
                    top = y;
                }
            }
            else {
                if (!beam_found) {
                    beam_found = true;
                    bw.from = y;
                }
                affected_points++;
//                screen.text(x+1,y+1, "#");
            }
        }
        beam.push_back(bw);
        cout << x << "\t" << bw.from << "\t" << bw.to << std::endl;
        // check square
        if (bw.to - bw.from > 100 && beam.size() > 100) {
            int candidate_to = bw.from + 99;
            if (beam[x-99].to >= candidate_to) {
                // Square found!
                cout << (x - 99)*10000 + bw.from;
                return 0;
            }
        }


    }
    cout << endl << affected_points;
}

long get_status(IntcodeComputer &computer, int x, int y) {

    computer.reset();
    computer.set_input(vector<long> { x, y});
    while (!computer.is_halted() && !computer.step(false, on_input)) {
        ;
    }

    if (computer.is_halted()) {
        return -1;
    }
    else {
        return computer.get_last_output();
    }
}

