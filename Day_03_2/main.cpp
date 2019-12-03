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

    std::vector<std::unordered_map<Node,int>> maps;

    for (auto &connection : circuit) {
        std::unordered_map<Node,int> nodes;
        Node node {0,0};
        unsigned int distance = 0;
        for (const auto &wire: connection) {
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
                    if (distance >= nodes[node]) {
                        distance = nodes[node];
                    }
                }
                nodes[node] = distance;
            }
        }
        maps.push_back(nodes);
    }

    // find intersections
    int min_delay = INT32_MAX;


    for (const auto &node: maps[0]) {
        if (node.first.x == 0 && node.first.y ==0) {
            continue;
        }
        for (const auto &node2: maps[1]) {
            if (node2.first.x == 0 && node2.first.y ==0) {
                continue;
            }

            if (node.first == node2.first) {
                auto signal_delay = node.second + node2.second;
                if (signal_delay < min_delay) {
                    min_delay = signal_delay;
                }
            }
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
