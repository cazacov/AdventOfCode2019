#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <algorithm>
#include "ttmath/ttmath.h"


struct Technique {
    ttmath::Int<10> factor;
    ttmath::Int<10> bias;

    static Technique Id() { return Technique {1,0};}
};

using namespace std;
vector<Technique> read_input(string file_name);

const int STACK_SIZE = 10007 ;

ttmath::Int<10> modular_inverse(ttmath::Int<10> p, ttmath::Int<10> a) {
    // https://en.wikipedia.org/wiki/Extended_Euclidean_algorithm

    ttmath::Int<10> t = 0;
    ttmath::Int<10> r = p;
    ttmath::Int<10> new_t = 1;
    ttmath::Int<10> new_r = a;

    while (new_r != 0) {
        auto quotient = r / new_r;
        auto temp = t;
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

Technique combine(Technique first, Technique second, const ttmath::Int<10> p) {
    auto new_factor = (first.factor * second.factor) % p;
    auto new_bias = (first.bias * second.factor + second.bias) % p;

    if (new_factor < 0)  {
        new_factor += p;
    }
    if (new_bias < 0) {
        new_bias += p;
    }
    return Technique {new_factor, new_bias};
}


ttmath::Int<10> apply_technique(Technique tech, ttmath::Int<10> n, const ttmath::Int<10> p) {
    auto result = (n*tech.factor + tech.bias) % p;

    if (result < 0) {
        result += p;
    }
    return result;
}

Technique inverse_technique(Technique t, const ttmath::Int<10> p) {

    ttmath::Int<10> inv_a = 1;
    if (t.factor != 1) {
        inv_a = modular_inverse(STACK_SIZE, t.factor);
    }

    ttmath::Int<10> bias = (-t.bias * inv_a) % p;
    if (bias < 0) {
        bias += p;
    }

    return Technique { inv_a, bias};
}

int main() {
    auto techniques = read_input("input.txt");

    Technique combined = Technique::Id();
    for (const auto &technique : techniques) {
        combined = combine(combined, technique, STACK_SIZE);
    }

    auto factor = combined.factor.ToString();
    auto bias = combined.bias.ToString();

    ttmath::Int<10> pos = apply_technique(combined, 2019, STACK_SIZE);
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
