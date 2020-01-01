//
// Created by Victor on 23.10.2019.
//

#include "PrimesInRange.h"
#include <stdexcept>
#include <cmath>

PrimesInRange::PrimesInRange(int upper_limit) {

    this->range = upper_limit;
    // Sieve of Eratosthenes algorithm

    // Estimate the value of the biggest number as p(n) ~ n*log(n)
    // Source: G. H. Hardy and E. M. Wright, An introduction to the theory of numbers, Oxford University Press, 1979.  ISBN 0198531702.

    std::vector<bool> is_prime(upper_limit+1, true);

    for (int i = 2; i <= upper_limit;  i++) {
        if (is_prime[i]) {
            for (int j = 2 * i; j <= upper_limit; j += i) {
                is_prime[j] = false;
            }
        }
    }

    this->primes.clear();
    for (size_t i = 2; i < upper_limit; i++) {
        if (is_prime[i]) {
            this->primes.insert(i);
        }
    }
}

bool PrimesInRange::is_prime(long n) {
    int lim = (int)(sqrt(n) + 1);
    if (lim > range) {
        throw std::runtime_error("Not in range");
    }
    for (const int prime : this->primes)
    {
        if (prime > lim) {
            return true;
        }
        if (n % prime == 0) {
            return false;
        }
    }
    return true;
}
