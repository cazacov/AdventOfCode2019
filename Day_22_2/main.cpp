#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <algorithm>
#include "ttmath/ttmath.h"
#include "mod_helper.h"


struct Technique {
    ttmath::Int<10> factor;
    ttmath::Int<10> bias;

    static Technique Id() { return Technique {1,0};}

    void combine(Technique other, const ttmath::Int<10> &p) {
        factor = (factor * other.factor) % p;
        bias = (bias * other.factor + other.bias) % p;

        if (factor < 0)  {
            factor += p;
        }
        if (bias < 0) {
            bias += p;
        }
    }

    Technique inverse(const ttmath::Int<10> &p) {
        ttmath::Int<10> inv_a = 1;
        if (factor != 1) {
            inv_a = modular_inverse(factor, p);
        }
        ttmath::Int<10> inv_bias = (-bias * inv_a) % p;
        if (inv_bias < 0) {
            inv_bias += p;
        }
        return Technique { inv_a, inv_bias};
    }
};

using namespace std;
vector<Technique> read_input(const string &file_name);

const long STACK_SIZE = 119315717514047L;

int main() {
    auto techniques = read_input("input.txt");

    Technique combined = Technique::Id();
    for (const auto &technique : techniques) {
        combined.combine(technique, STACK_SIZE);
    }

    auto inverse = combined.inverse(STACK_SIZE);

    ttmath::Int<10> a = inverse.factor;
    ttmath::Int<10> b = inverse.bias;
    ttmath::Int<10> n = 101741582076661L;   // Apply complete shuffle process to the deck 101741582076661 times in a row
    ttmath::Int<10> pos = 2020;

    /*
     * After applying the shuffle process n times in a row the card on position pos will be:
     *
     * a^n * pos + b * (a^(n-1) + a^(n-2) + ... + a + 1)
     *
     * The right  part of this expression is the sum of n terms of a geometric progression
     * with the first term=1 and ratio a. S = 1 * (1-a^n) / (1 - a)
     * https://en.wikipedia.org/wiki/Geometric_progression
     *
     * card = a^n * pos + b * (a^n - 1) / (a - 1)
     */


    auto an = mod_power<ttmath::Int<10>>(a, n, STACK_SIZE);
    std::cout << a << "^" << n << "=" << an << std::endl;

    auto card = ( an * pos + b * (an - 1) * modular_inverse(a - 1, STACK_SIZE) ) % STACK_SIZE;
    std::cout << "Card in position " << pos << "  is " << card << std::endl;
    return 0;
}


vector<Technique> read_input(const string &file_name) {

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