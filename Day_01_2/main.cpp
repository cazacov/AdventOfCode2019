#include <iostream>
#include <vector>
#include <fstream>

std::vector<int> read_data(const char *file_name);

int fuel_for_mass(int mass);

int main() {

    auto module_masses = read_data("module_masses.txt");

    long fuel_total = 0;

    for (auto mass : module_masses) {
        int fuel = fuel_for_mass(mass);
        while (fuel >= 0) {
            fuel_total += fuel;
            fuel = fuel_for_mass(fuel);
        }
    }

    std::cout << fuel_total << std::endl;
    return 0;
}

int fuel_for_mass(int mass) {
    auto fuel = mass / 3 - 2;
    return fuel;
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
