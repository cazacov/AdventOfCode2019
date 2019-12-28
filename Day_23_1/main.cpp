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

vector<long> convert_program(vector<string> program);

int main() {

    vector <IntcodeComputer> network;

    for (int i = 0; i < 50; i++) {
        IntcodeComputer computer;
        computer.load_program("program.txt");
        computer.set_input(vector<long> {i});
        computer.set_default_input(-1);
        network.push_back(computer);
    }

    vector <vector<long>> packets(50);

    int addr = 0;
    do {
        for (int i = 0; i < network.size(); i++) {
            auto &computer = network[i];
            if (computer.is_halted()) {
                continue;
            }
            bool has_output = computer.step(false, nullptr);
            if (has_output) {
                packets[i].push_back(computer.get_last_output());
                if (packets[i].size() == 3) {
                    addr = packets[i][0];
                    long x = packets[i][1];
                    long y = packets[i][2];
                    if (addr < 50) {
                        network[addr].set_input(vector<long>{x, y});
                    }
                    cout << "Packet [" << x << "," << y << "] has benn sent from "<< i << " to " << addr <<endl;
                    packets[i].clear();
                }
            }
        }
    } while (addr != 255);
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