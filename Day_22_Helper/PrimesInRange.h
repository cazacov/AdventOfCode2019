//
// Created by Victor on 23.10.2019.
//

#ifndef PROBLEM027_PRIMEPOOL_H
#define PROBLEM027_PRIMEPOOL_H

#include<vector>
#include <set>

class PrimesInRange {
private:
    int range;
public:
    std::set<int> primes;
    explicit PrimesInRange(int n);
    bool is_prime(long n);
};


#endif //PROBLEM027_PRIMEPOOL_H
