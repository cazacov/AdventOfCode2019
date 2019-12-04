#include <iostream>
#include <stdlib.h>
#include <chrono>

int count_combinations(int FROM, int);

int main() {
    const int FROM = 246515;
    const int TO = 739105;
    const int REPEAT = 1000000;

    auto t1= std::chrono::high_resolution_clock::now();
    int count = 0;
    for (int i = 0; i < REPEAT; i++) {
        count = count_combinations(FROM, TO);   // Let's make one million calls!
        if (count != 677) {
            throw std::runtime_error("Wrong result!");
        }
    }
    auto t2 = std::chrono::high_resolution_clock::now();

    std::cout << count << std::endl;

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    std::cout << "Duration (microseconds): " << (double)duration / REPEAT;
    return 0;
}

int count_combinations(const int FROM, const int TO) {

    int result = 0;

    int digits[6];
    int n = FROM;
    for (int &digit : digits) {
        digit = n % 10;
        n/= 10;
    }

    // fast forward to next monotonic number
    for (int i = 5; i > 0; i--) {
        if (digits[i] > digits[i-1]) {
            int fill = digits[i];
            int j = i-1;
            while (j >= 0) {
                digits[j--] = fill;
            }
            break;
        }
    }

    int limit[6];
    n = TO;
    for (int &digit : limit) {
        digit = n % 10;
        n/= 10;
    }
    // down to the next monotonic number
    for (int i = 5; i > 0; i--) {
        if (limit[i] > limit[i-1]) {
            limit[i]--;
            int fill = 9;
            int j = i-1;
            while (j >= 0) {
                limit[j--] = fill;
            }
            break;
        }
    }


    bool limit_reached = false;
    while (!limit_reached) {
        bool hasDoubles = false;
        for (int i = 0; i < 5; i++) {
            if (digits[i] == digits[i+1]) {
                hasDoubles = true;
                if (i < 4 && digits[i] == digits[i+2]) {
                    hasDoubles=false;
                    int n = digits[i];
                    while (i < 5 && digits[i+1] == n) {
                        i++;
                    }
                } else {
                    break;
                }
            }
        }

        if (hasDoubles) {
            result++;   // That is the goal of this function!
        }


        // Go to next monotonic number
        int pos = 0;
        while (digits[pos] == 9) {
            pos++;
        }
        digits[pos] += 1;
        auto fill = digits[pos];
        pos--;
        while (pos >= 0) {
            digits[pos] = fill;
            pos--;
        }

        // check exit condition
        limit_reached = true;
        for (int i = 5; i >= 0; i-- ) {
            if (digits[i] != limit[i]) {
                limit_reached = false;
                break;
            }
        }
    }
    return result;
}