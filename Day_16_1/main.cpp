#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <numeric>
#include "Eigen/Dense"

using Eigen::Map;
using Eigen::VectorXi;
using Eigen::Unaligned;

void fill_pattern_matrix(Eigen::MatrixXi &matrix, std::vector<int> pattern);
std::vector<int> load_signals(const char *file_name);

int main() {

    auto data = load_signals("signals.txt");

    Eigen::VectorXi signals = Map<VectorXi, Unaligned>(data.data(), data.size());
    Eigen::MatrixXi pattern(signals.size(), signals.size());

    fill_pattern_matrix(pattern, {0,1,0,-1});

    for (int phase = 1; phase <= 100; phase++) {
        // Apply patterns
        signals = (pattern * signals);

        // Keep the ones digit only
        signals = signals.unaryExpr([](int elem) {return abs(elem % 10);});
    }
    std:: cout << signals.head(8);
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

void fill_pattern_matrix(Eigen::MatrixXi &matrix, std::vector<int> pattern) {

    int rows = matrix.rows();
    int cols = matrix.cols();

    for (int i = 0; i < rows; i++) {
        auto pos = 0;
        auto it = pattern.begin();
        auto repeats = 1;

        while (pos < cols) {
            while (repeats < i+1 && pos < cols) {
                matrix(i,pos) = *it;
                pos++;
                repeats++;
            }
            it++;
            if (it == pattern.end()) {
                it = pattern.begin();
            }
            repeats = 0;
        }
    }
}
