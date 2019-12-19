#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <numeric>

std::vector<int> load_signals(const char *file_name);

int main() {

    auto raw_data = load_signals("signals.txt");

    int start_offset = 0;
    for (int i  = 0; i < 7; i++) {
        start_offset*=10;
        start_offset += raw_data[i];
    }
    int block_length = raw_data.size() * 10000 - start_offset;

    std::vector<int> signals(block_length);
    int pos = start_offset % raw_data.size();

    auto from_it = raw_data.begin() + pos;
    auto to_it = signals.begin();

    for (int i = 0; i < block_length; i++) {
        *to_it++ = *from_it++;
        if (from_it == raw_data.end()) {
            from_it = raw_data.begin();
        }
    }

    std::vector<int> next(block_length);
    for (int phase = 0; phase < 100; phase++) {
        int sum = std::accumulate(signals.begin(), signals.end(), 0, std::plus<>());
        for (int i = 0; i < block_length; i++) {
            if (i > 0) {
                sum -= signals[i-1];
            }
            next[i] = sum % 10;
        }
        signals = next;
        std::cout << "Phase " << phase << std::endl;
    }

    for (auto it = signals.begin(); it != signals.begin()+8; it++) {
        std::cout << *it;
    }
    return 0;
}

std::vector<int> load_signals(const char *file_name) {

    std::vector<int> result;
    std::ifstream  data(file_name);
    std::string line;
    while(std::getline(data,line))
    {
        for (const char &ch : line) {
            result.push_back(ch-'0');
        }
    }
    return result;
}