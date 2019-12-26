#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <algorithm>

enum Command {
    NewStack,
    Cut,
    Increment
};

struct Technique {
    Command command;
    int parameter;
};

using namespace std;
vector<Technique> read_input(string file_name);

void apply_new_stack(vector<int> &stack);

void apply_increment(vector<int> &stack, int parameter);

void apply_cut(vector<int> &stack, int parameter);

int main() {
    auto techniques = read_input("input.txt");

    const int STACK_SIZE = 10007;

    vector<int> stack(STACK_SIZE);

    // Cards come in factory order
    for (int i = 0; i < STACK_SIZE; i++) {
        stack[i] = i;
    }

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
    return 0;
}

void apply_cut(vector<int> &stack, int parameter) {
    size_t cut_point = parameter;
    if (parameter < 0) {
        cut_point = stack.size() + parameter;
    }

    vector<int> temp(stack.begin(), stack.begin()+cut_point);
    stack.erase(stack.begin(), stack.begin()+cut_point);
    stack.insert(stack.end(), temp.begin(), temp.end());
}

void apply_increment(vector<int> &stack, int parameter) {

    int modulo = stack.size();

    vector<int> temp(stack.begin(), stack.end());
    for (int i = 0; i < temp.size(); i++) {
        stack[(i*parameter) % modulo] = temp[i];
    }
}

void apply_new_stack(vector<int> &stack) {
    reverse(stack.begin(), stack.end());
}

vector<Technique> read_input(string file_name) {

    vector<Technique> result;
    std::ifstream  data(file_name);
    std::string line;
    while(std::getline(data,line)) {
        Technique technique;

        if (line.rfind("deal with increment", 0) == 0) {
            technique.command = Command::Increment;
            technique.parameter = stoi(line.substr(strlen("deal with increment"), 1000));
        } else if (line.rfind("cut", 0) == 0) {
            technique.command = Command::Cut;
            technique.parameter = stoi(line.substr(strlen("cut"), 1000));
        } else if (line.rfind("deal into new stack", 0) == 0) {
            technique.command = Command::NewStack;
            technique.parameter = 0;
        } else {
            cout << "Unknown input! " << line << endl;
        }
        result.push_back(technique);
    }
    return result;
}
