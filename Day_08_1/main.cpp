#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

std::vector<int> load_data(const char *file_name);

int count_in_layer(const std::vector<int> &data, int layer, int layer_size, int value) {
    auto from = data.begin() + layer * layer_size;
    auto to = data.begin() + (layer + 1) * layer_size;
    int result = std::count(from, to, value);
    return result;
}


int main() {

    auto data = load_data("image.sif");

    int layer_size = 6*25;
    int layers = data.size() / layer_size;

    int min_zeros = layer_size;
    int min_layer = 0;
    for (int i = 0; i < layers; i++) {

        int zeros = count_in_layer(data, i, layer_size, 0);
        if (zeros < min_zeros) {
            min_zeros = zeros;
            min_layer = i;
        }
    }

    int ones = count_in_layer(data, min_layer, layer_size, 1);
    int twos = count_in_layer(data, min_layer, layer_size, 2);

    std::cout << ones*twos << std::endl;

    // Render images
    std::vector<int> pixels(6*25, 2);
    for (int layer = 0; layer < layers; layer++) {
        int pos = layer * layer_size;
        for (int i = 0; i < 6*25; i++) {
            if (pixels[i]  == 2) {
                pixels[i] = data[pos];
            }
            pos++;
        }
    }

    // Show
    int pos = 0;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 25; j++) {
            switch (pixels[pos]) {
                case 0:
                    std::cout << " ";
                    break;
                case 1:
                    std::cout << "#";
                    break;
                default:
                    std::cout << ".";
                    break;
            }
            pos++;
        }
        std::cout << std::endl;
    }
    return 0;
}



std::vector<int> load_data(const char *file_name) {

    std::vector<int> result;

    std::ifstream  data(file_name);
    std::string line;
    while(std::getline(data,line))
    {
        for (const auto &ch : line) {
            result.push_back(ch - '0');
        }
    }
    return result;
}
