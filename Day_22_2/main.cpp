#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <algorithm>

struct Technique {
    long factor;
    long bias;
};

using namespace std;
vector<Technique> read_input(string file_name);

const int STACK_SIZE = 10007 ;

long modular_inverse(long p, long a) {
    // https://en.wikipedia.org/wiki/Extended_Euclidean_algorithm

    long t = 0;
    long r = p;
    long new_t = 1;
    long new_r = a;

    while (new_r != 0) {
        long quotient = r / new_r;
        long temp = t;
        t = new_t;
        new_t = temp - quotient * new_t;
        temp = r;
        r = new_r;
        new_r = temp - quotient * new_r;
    }
    if (t < 0) {
        t += p;
    }
    return t;
}


long apply_technique(const long p, long n, Technique tech) {
    long result = (n*tech.factor + tech.bias) % p;

    if (result < 0) {
        result += p;
    }
    return result;
}

Technique inverse_technique(Technique t) {

    long inv_a = 1;
    if (t.factor != 1) {
        inv_a = modular_inverse(STACK_SIZE, t.factor);
    }

    long bias = (-t.bias * inv_a) % STACK_SIZE;
    if (bias < 0) {
        bias += STACK_SIZE;
    }

    return Technique { inv_a, bias};
}





int main() {
    auto techniques = read_input("input.txt");



    vector<int> stack(STACK_SIZE);

    // Cards come in factory order
    for (int i = 0; i < STACK_SIZE; i++) {
        stack[i] = i;
    }

    /*
    Technique new_stack {-1, -1 };   // new stack
    Technique cut_pos {1, 3 };   // cut 3
    Technique cut_neg {1, -4 };   // cut -4
    Technique increment {(int)modular_inverse(STACK_SIZE, 3L), 0 };   // deal with increment 3


    for (int i = 0; i < STACK_SIZE; i++) {
        cout << apply_technique(STACK_SIZE, i, increment) << " ";
    }
    cout << endl;
*/

    int pos = 2019;


    for (const auto &technique : techniques) {
        pos = apply_technique(STACK_SIZE, pos, technique);
    }

    std::cout << "Card 2019 in position " << pos << std::endl;

    return 0;
}

vector<Technique> read_input(string file_name) {

    vector<Technique> result;
    std::ifstream  data(file_name);
    std::string line;
    while(std::getline(data,line)) {
        Technique technique;

        if (line.rfind("deal with increment", 0) == 0) {
            int inc = stoi(line.substr(strlen("deal with increment"), 1000));
            technique.factor = inc;
            technique.bias = 0;
        } else if (line.rfind("cut", 0) == 0) {
            technique.factor = 1;
            technique.bias = -stoi(line.substr(strlen("cut"), 1000));
        } else if (line.rfind("deal into new stack", 0) == 0) {
            technique.factor = -1;
            technique.bias = -1;
        } else {
            cout << "Unknown input! " << line << endl;
        }
        result.push_back(technique);
    }
    return result;
}
