#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <numeric>

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
const Node CENTER_POINT = Node {0,0};

// Required by unordered_map
template<> struct std::hash<Node> {
    std::size_t operator() (const Node &node) const {
        return hash<int>()(node.x) + hash<int>()(node.y);
    }
};


std::vector<std::vector<Wire>> load_circuit(const char *file_name);


int main() {
    auto circuit = load_circuit("wires.txt");

    std::unordered_map<Node, std::vector<int>> nodes;

    for (int con = 0; con < circuit.size(); con++) {
        Node node {CENTER_POINT};
        int distance = 0;
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
                    std::vector<int> distances = nodes[node];
                    if (distances[con] == 0) {
                        nodes[node][con] = distance;
                    }
                }
                else {
                    std::vector<int> distances(circuit.size(),0);
                    distances[con] = distance;
                    nodes[node] = distances;
                }
            }
        }
    }

    // find minimal signal delay
    int min_delay = INT32_MAX;
    for (const auto &node: nodes) {
        if (node.first == CENTER_POINT) {
            continue;
        }

        std::vector<int> distances = node.second;
        
        // Are all wires are connected to the node?
        if ( !std::all_of(distances.begin(), distances.end(), [](int d) { return d > 0;})) {
            continue;
        }

        int total_delay = std::accumulate(distances.begin(), distances.end(), 0, std::plus<>());
        if (total_delay < min_delay) {
            min_delay = total_delay;
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