//
// Created by Victor on 05.01.2020.
//

#ifndef DAY_22_2_MOD_HELPER_H
#define DAY_22_2_MOD_HELPER_H

ttmath::Int<10> modular_inverse(const ttmath::Int<10> &a, const ttmath::Int<10> &p) {
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

template <class T>
T mod_power(T a, T n, T mod) {

    if (n == 0) {
        return T(1);
    }
    else if (n == 1) {
        return a;
    }
    else {
        T result;
        if (n % 2 == 0) {
            result = mod_power(a, n/2, mod);
            result*=result;
        }
        else {
            result = mod_power(a, n/2, mod);
            result*=result;
            result %= mod;
            result*=a;
        }
        result %= mod;
        return result;
    }
}

#endif //DAY_22_2_MOD_HELPER_H
