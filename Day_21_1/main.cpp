#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include "IntcodeComputer.h"
#include "Screen.h"
#include "vector"
#include <unordered_map>

using namespace std;

long get_status(IntcodeComputer &computer, int x, int y);
int next_input = 0;
long on_input() {
    return next_input;
}

using namespace std;

IntcodeComputer computer;
vector<long> convert_program(vector<string> program);

int main() {
    computer.load_program("program.txt");

    vector<string> program {
        "NOT C J",  // jump = !c
        "NOT B T",  // t = !b
        "OR T J",   // jump = !b | !c
        "NOT A T",  // t = !a
        "OR T J",   // jump = !a | !b | !c
        "AND D J",  // jump = d & (!a | !b | !c)
        "WALK"
    };

    computer.set_input(convert_program(program));

    while(!computer.is_halted()) {
        auto has_out = computer.step(false, nullptr);
        if (!has_out) {
            continue;
        }

        long res = computer.get_last_output();
        if (res > 255) {
            // robot succeded
            cout << endl << "Robot reports: hull damage " << res;
            break;
        } else {
            if (res == 10)
            {
                cout << endl;
            }
            else {
                cout << (char) res;
            }
        }
    }
    if (computer.is_halted()) {
        cout << "Program halted";
    }
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

vector<long> convert_program(vector<string> program) {
    string result;
    for (const string &str : program) {
        result+=str;
        result += '\x0A';
    }

    vector<long> res;
    transform(result.begin(), result.end(), back_inserter(res), [](char ch) {return (long) ch;});

    return res;
}