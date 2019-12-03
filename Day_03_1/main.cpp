#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <unordered_map>

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

template<> struct std::hash<Node> {
    std::size_t operator() (const Node &node) const {
        return hash<int>()(node.x) + hash<int>()(node.y);
    }
};


std::vector<std::vector<Wire>> load_circuit(const char *file_name);


int main() {
    std::unordered_map<Node,unsigned int> nodes;

    auto circuit = load_circuit("wires.txt");

    for (int connection = 0; connection < circuit.size(); connection++)
    {
        unsigned int mask = 1u << connection;
        Node node {0,0};
        for (const auto &wire: circuit[connection]) {
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
                if (nodes.count(node)) {
                    nodes[node] |= mask;
                }
                else {
                    nodes[node] = mask;
                }
            }
        }
    }


    int min_distance = INT32_MAX;
    unsigned int intersection_mask = 0;
    for (int i = 0; i < circuit.size(); i++) {
        intersection_mask |= (1u << i);
    }

    for (auto node : nodes) {
        if (node.second != intersection_mask) { // No or self-intersection
            continue;
        }
        if (node.first.x == 0 && node.first.y == 0) { // Exclude the central point
            continue;
        }
        int distance = abs(node.first.x) + abs(node.first.y);   // Manhattan distance
        if (distance < min_distance) {
            min_distance = distance;
        }
    }

    std::cout << min_distance << std::endl;
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
