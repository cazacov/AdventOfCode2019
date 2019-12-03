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
    auto circuit = load_circuit("wires.txt");

    std::unordered_map<Node,unsigned long> nodes;

    for (int con = 0; con < circuit.size(); con++) {
        Node node {0,0};
        unsigned long distance = 0;
        for (const auto &wire: circuit[con]) {
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
                if (nodes.count(node)) {
                    unsigned long old_value = nodes[node];
                    unsigned long existing_distance = (old_value >> (con * 32)) & 0xFFFFFFFFL;
                    if (existing_distance > 0 && existing_distance <= distance) {
                        distance = existing_distance;
                    }
                    else {
                        if (con == 0) {
                            nodes[node] = (old_value & 0xFFFFFFFF00000000L) | distance;
                        } else {
                            nodes[node] = (distance << 32) | (old_value & 0xFFFFFFFFL);
                        }
                    }
                }
                else {
                    nodes[node] = distance << (con * 32);
                }
            }
        }
    }

    // find intersections
    int min_delay = INT32_MAX;

    for (const auto &node: nodes) {
        if (node.first.x == 0 && node.first.y ==0) {
            continue;
        }
        unsigned long dist = node.second;
        unsigned long d1 = dist & 0xFFFFFFFFL;
        unsigned long d2 = dist >> 32;
        if (!d1 || !d2) {
            continue;
        }
        if (d1+d2 < min_delay) {
            min_delay = d1+d2;
        }
    }

    std::cout << min_delay << std::endl;
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
