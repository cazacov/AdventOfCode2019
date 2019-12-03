#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <chrono>

struct Wire {
    char direction;
    int length;

    bool operator==(const Wire &other) const{
        return direction == other.direction && length == other.length;
    }
};

struct Node {
    int x;
    int y;
    bool operator==(const Node &other) const{
        return x == other.x && y == other.y;
    }
};
// Required by unordered_map
template<> struct std::hash<Node> {
    std::size_t operator() (const Node &node) const {
        return hash<int>()(node.x) ^ hash<int>()(node.y);
    }
};

const Node CENTER_POINT = Node {0,0};

std::vector<std::vector<Wire>> load_circuit(const char *file_name);


int main() {

    auto t1 = std::chrono::high_resolution_clock::now();
    auto circuit = load_circuit("wires.txt");

    std::unordered_map<Node, int> nodes;

    // trace the first wire
    Node node {CENTER_POINT};
    int distance = 0;
    for (const auto &wire: circuit[0]) {
        for (int i = 0; i < wire.length; i++) {
            switch (wire.direction) {
                case 'U':
                    node.y++;
                    break;
                case 'D':
                    node.y--;
                    break;
                case 'R':
                    node.x++;
                    break;
                case 'L':
                    node.x--;
                    break;
            }
            distance++;
            auto it = nodes.find(node);
            if (it != nodes.end()) {
                if ((*it).second == 0) {
                    (*it).second = distance;
                }
            }
            else {
                nodes[node] = distance;
            }
        }
    }


    // find the minimal signal delay
    int min_distance = INT32_MAX;

    node = {CENTER_POINT};
    distance = 0;
    for (const auto &wire: circuit[1]) {
        for (int i = 0; i < wire.length; i++) {
            switch (wire.direction) {
                case 'U':
                    node.y++;
                    break;
                case 'D':
                    node.y--;
                    break;
                case 'R':
                    node.x++;
                    break;
                case 'L':
                    node.x--;
                    break;
            }
            distance++;
            auto it = nodes.find(node);
            if (it != nodes.end()) {
                if ((*it).second + distance < min_distance) {
                    min_distance = (*it).second + distance;
                }
            }
        }
    }
    std::cout << min_distance << std::endl;
    auto t2 = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

    std::cout << "Duration (milliseconds): " << duration / 1000;
    return 0;
}



std::vector<std::vector<Wire>> load_circuit(const char *file_name) {

    std::vector<std::vector<Wire>> result;

    std::ifstream  data(file_name);
    std::string line;
    while(std::getline(data,line))
    {
        std::vector<Wire> wire;
        std::stringstream lineStream(line);
        std::string element;
        while(std::getline(lineStream, element ,','))
        {
            int length = strtol(&element[1], NULL,10);
            wire.push_back(Wire{element[0], length});
        }
        result.push_back(wire);
    }
    return result;
}