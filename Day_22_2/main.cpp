#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <algorithm>

struct Technique {
    int factor;
    int bias;
};

using namespace std;
vector<Technique> read_input(string file_name);

long apply_technique(const long p, long n, Technique tech) {
    long result = (n*tech.factor + tech.bias) % p;

    if (result < 0) {
        result += p;
    }
    return result;
}



int main() {
    auto techniques = read_input("input.txt");

    const int STACK_SIZE = 10;

    vector<int> stack(STACK_SIZE);

    // Cards come in factory order
    for (int i = 0; i < STACK_SIZE; i++) {
        stack[i] = i;
    }

    Technique new_stack {-1, -1 };   // new stack
    Technique cut_pos {1, 3 };   // cut 3
    Technique cut_neg {1, -4 };   // cut -4
    Technique increment {3, 0 };   // deal with increment 3


    for (int i = 0; i < STACK_SIZE; i++) {
        cout << apply_technique(STACK_SIZE, i, increment) << " ";
    }
    cout << endl;
/*




    for (const auto &technique : techniques) {
        switch (technique.command) {
            case Command::NewStack:
                apply_new_stack(stack);
                break;
            case Command::Increment:
                apply_increment(stack, technique.parameter);
                break;
            case Command::Cut:
                apply_cut(stack, technique.parameter);
                break;
        }
    }

    // Show result
    for (int i = 0; i <= STACK_SIZE / 20; i++) {
        cout << setw(5) << i * 20 << "\t";
        for (int j = 0; j < 20; j++) {
            int pos = i * 20 + j;
            if (pos > stack.size() - 1) {
                break;
            }
            cout << " " << stack[pos];
        }
        cout << endl;
    }

    for (int i = 0; i < stack.size(); i++) {
        if (stack[i] == 2019) {
            std::cout << "Card 2019 in position " << i << std::endl;
        }
    }
*/
    return 0;
}

vector<Technique> read_input(string file_name) {

    vector<Technique> result;
    std::ifstream  data(file_name);
    std::string line;
    while(std::getline(data,line)) {
        Technique technique;

        if (line.rfind("deal with increment", 0) == 0) {
            technique.factor = stoi(line.substr(strlen("deal with increment"), 1000));
            technique.bias = 0;
        } else if (line.rfind("cut", 0) == 0) {
            technique.factor = stoi(line.substr(strlen("cut"), 1000));
            technique.bias = stoi(line.substr(strlen("cut"), 1000));
        } else if (line.rfind("deal into new stack", 0) == 0) {
            technique.factor = -1;
            technique.bias = 0;
        } else {
            cout << "Unknown input! " << line << endl;
        }
        result.push_back(technique);
    }
    return result;
}
