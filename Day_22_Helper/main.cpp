#include <iostream>
#include <math.h>
#include "PrimesInRange.h"

int main() {

    // factorize 119315717514047

    //const long n1 = 119315717514047L;
    const long n1 = 17098205319764;
    const long n2 = 101741582076661L;

    PrimesInRange primes((int)sqrt(n1));

    long n = n1;
    bool found = false;

    for (const int &div: primes.primes) {
        if (n % div == 0) {
            std::cout << div << std::endl;
            n /= div;
            found = true;
        }
    }
    std::cout << n << std::endl;

    if (!found) {
        std::cout << n1 << " is a prime number" << std::endl;
    }

    return 0;
}