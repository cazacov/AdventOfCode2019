#include <iostream>
#include <stdlib.h>
#include <map>
#include <algorithm>
#include <unordered_map>

int main() {

    const int FROM = 246515;
    const int TO = 739105;

    int count = 0;

    for (int n= FROM; n <= TO; n++) {
        auto digits = std::to_string(n);

        std::unordered_map<int,int> char_count;
        for (int i = 0; i < 6; i++) {
            char_count[digits[i]] = char_count[digits[i]]+1;
        }

        if (!std::any_of(char_count.begin(), char_count.end(), [](auto x) {return x.second >= 2;})) {
            continue;
        }

        bool is_monotonic = true;
        for (int i = 0; i < 5; i++) {
            if (digits[i] > digits[i+1]) {
                is_monotonic = false;
                break;
            }
        }
        if (!is_monotonic) {
            continue;
        }
        count++;
    }

    std::cout << count << std::endl;
    return 0;
}