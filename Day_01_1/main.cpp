#include <iostream>
#include <vector>
#include <fstream>

std::vector<int> read_data(const char *file_name);

int main() {

    auto module_masses = read_data("module_masses.txt");

    long fuel_total = 0;

    for (auto mass : module_masses) {
        auto fuel = mass / 3 - 2;
        fuel_total += fuel;
    }

    std::cout << fuel_total << std::endl;
    return 0;
}


std::vector<int> read_data(const char *file_name) {
    std::vector<int> result;

    std::ifstream  data(file_name);
    std::string line;
    while(std::getline(data,line))
    {
        result.push_back(stoi(line));
    }
    return result;
}
