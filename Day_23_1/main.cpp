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

vector <vector<long>> inputs(50);
vector<int> idle_counts(50);

int next_input = 0;
long on_input(int computer_id) {
    long result;

    auto &input =inputs[computer_id];
    if (!input.empty()) {
        result = input.front();
        input.erase(input.begin());
        idle_counts[computer_id] = 0;
    }
    else {
        result = -1;
        idle_counts[computer_id]++;
    }
    return result;
}

using namespace std;

vector<long> convert_program(vector<string> program);

int main() {

    vector <IntcodeComputer> network;

    for (int i = 0; i < 50; i++) {
        IntcodeComputer computer;
        computer.load_program("program.txt");
        computer.set_input(vector<long> {i});
        network.push_back(computer);
        idle_counts[i] = 0;
        inputs[i].clear();
    }

    vector <vector<long>> packets(50);
    vector<long> nat_state (2,0);
    long nat_last = INT64_MIN;
    long nat_last2 = INT64_MIN+1;

    int cycle = 0;
    int addr = 0;
    do {
        for (int i = 0; i < network.size(); i++) {
            auto &computer = network[i];
            if (computer.is_halted()) {
                continue;
            }
            bool has_output = computer.step(false, on_input, i);
            if (has_output) {
                packets[i].push_back(computer.get_last_output());
                if (packets[i].size() == 3) {
                    addr = packets[i][0];
                    long x = packets[i][1];
                    long y = packets[i][2];
                    if (addr < 50) {
                        vector<long> new_package{x, y};
                        inputs[addr].insert(inputs[addr].end(), new_package.begin(), new_package.end());
                        idle_counts[addr] = 0;
                    }
                    else {
                        nat_state = vector<long>{x, y};
                    }
                    cout << "Packet [" << x << "," << y << "] has benn sent from "<< i << " to " << addr <<endl;
                    packets[i].clear();
                }
            }
        }
        // NAT logic
        if (all_of(idle_counts.begin(), idle_counts.end(),
                [](int idle_counter) { return idle_counter > 1; })) {

            if (!all_of(packets.begin(), packets.end(), [](vector<long> &packet) { return packet.empty(); })) {
                continue;
            }

            inputs[0] = nat_state;
            idle_counts[0] = 0;
            cout << "NAT sends " << nat_state[0] << "," << nat_state[1] << " to 0." << endl;
            nat_last2 = nat_last;
            nat_last = nat_state[1];
            if (nat_last == nat_last2) {
                cout << "Double NAT package: " << nat_last << endl;
            }
        }
    } while (nat_last != nat_last2);
}
