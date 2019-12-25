#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <unordered_map>
#include <unordered_set>

using namespace std;

vector<vector<char>> read_map(string file_name);
void show_map(const vector<vector<char>>  &map);
void remove_deadends(vector<vector<char>> &map);
void show_green(int x, int y, char ch);
void show_cyan(int x, int y, char ch);

unsigned char display(const char &ch);

const int offsets[4][2] {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

struct Cell {
    int x;
    int y;
    int level = 0;
    int distance = 1;
    bool operator==(const Cell &other) const{
        return x == other.x && y == other.y;
    }

    bool operator!=(const Cell &other) const{
        return x != other.x || y != other.y;
    }
};

// Required by unordered_map
template<> struct std::hash<Cell> {
    std::size_t operator() (const Cell &node) const {
        return (hash<int>()(node.x) << 4) ^ hash<int>()(node.y);
    }
};


int main() {
    auto map = read_map("input.txt");
    show_map(map);
    remove_deadends(map);

    unordered_map<Cell, unordered_set<Cell>> connections;
    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[y].size(); x++) {
            if (map[y][x] != '.') {
                continue;
            }
            Cell cell{x, y};
            unordered_set<Cell> &neighbours = connections[cell];

            for (int i = 0; i < 4; i++) {
                Cell neighbour{
                        x + offsets[i][1],
                        y + offsets[i][0]
                };
                if (map[neighbour.y][neighbour.x] == '.') {
                    connections[cell].insert(neighbour);
                }
            }
        }
    }

    // Simplify connections
    vector<Cell> to_erase;
    do {
        for (const Cell &kill: to_erase) {
            connections.erase(kill);
            show_cyan(kill.x, kill.y, '#');
        }
        to_erase.clear();

        for (const auto &pair : connections) {
            if (pair.second.size()==2) {
                Cell left = *pair.second.begin();
                Cell right = *(++pair.second.begin());
                int distance = left.distance + right.distance;
                left.distance = distance;
                right.distance = distance;
                connections[left].erase(pair.first);
                connections[left].insert(right);
                connections[right].erase(pair.first);
                connections[right].insert(left);
                to_erase.push_back(pair.first);
            }
        }
    } while(!to_erase.empty());


    unordered_map <string, Cell> portals;

    // process portals
    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[y].size(); x++) {
            if (map[y][x] != '.') {
                continue;
            }
            Cell cell{x, y};
            unordered_set<Cell> &neighbours = connections[cell];

            for (int i = 0; i < 4; i++) {
                Cell neighbour{
                        x + offsets[i][1],
                        y + offsets[i][0]
                };
                if (isalpha(map[neighbour.y][neighbour.x])) {
                    // We have an alpha character here
                    string portal_name = string(1,map[neighbour.y][neighbour.x]);
                    Cell neighbour2{
                            x + 2*offsets[i][1],
                            y + 2*offsets[i][0]
                    };
                    portal_name += string(1,map[neighbour2.y][neighbour2.x]);
                    // normalize name
                    std::sort(portal_name.begin(), portal_name.end());
                    if (portals.count(portal_name)) {
                        auto other = portals[portal_name];
                        connections[cell].insert(other);
                        connections[other].insert(cell);
                        show_green(cell.x, cell.y, 'o');
                        show_green(other.x, other.y, 'o');
                    } else {
                        portals[portal_name] = cell;
                    }
                }

            }
        }
    }

    // find the shortest path
    // Dijkstra's algorithm
    // https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm

    std::unordered_map<Cell, int> unvisited_set;
    for (const auto &pair : connections) {
        unvisited_set[pair.first] = INT32_MAX;
    }

    Cell from = portals["AA"];
    Cell to = portals["ZZ"];

    unvisited_set[from] = 0;

    auto current_node = from;
    while (current_node != to) {
        for (auto neighbour : connections[current_node]) {
            if (!unvisited_set.count(neighbour)) {
                continue;
            }
            int new_distance = unvisited_set[current_node] + neighbour.distance;
            if (unvisited_set[neighbour] > new_distance) {
                unvisited_set[neighbour] = new_distance;
            }
        }
        unvisited_set.erase(current_node);
        int min_distance = INT32_MAX;
        for (const auto &pair : unvisited_set) {
            if (pair.second < min_distance) {
                current_node = pair.first;
                min_distance = pair.second;
            }
        }
    }

    cout <<  unvisited_set[current_node] << endl;
    return 0;
}

void show_red(int x, int y, char ch) {
    printf("\033[31m\033[%d;%dH%c\033[0m", y+1, x+1, ch);
    fflush(stdout);
}

void show_cyan(int x, int y, char ch) {
    printf("\033[36m\033[%d;%dH%c\033[0m", y+1, x+1, ch);
    fflush(stdout);
}



void remove_deadends(vector<vector<char>> &map) {
    bool deadends_found = true;

    while (deadends_found) {
        deadends_found = false;
        for (int y = 0; y < map.size(); y++) {
            for (int x = 0; x < map[y].size(); x++) {
                if (map[y][x] != '.') {
                    continue;
                }
                int walls = 0;
                for (int i = 0; i < 4; i++) {
                    char n = map[y+offsets[i][0]][x+offsets[i][1]];
                    if (n == '#') {
                        walls += 1;
                    }
                }
                if (walls == 3) {
                    map[y][x] = '#';
                    deadends_found = true;
                    show_red(x, y, '.');
                }
            }
        }
    }
}

void show_map(const vector<vector<char>>  &map) {
    printf("\033[2J");
    fflush(stdout);

    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[y].size(); x++) {
            printf("\033[%d;%dH%c", y+1, x+1, display(map[y][x]));
            fflush(stdout);
        }
    }
}

unsigned char display(const char &ch) {
    if (ch == '#') {
        return '.';
    }
    else if (ch == '.') {
        return '#';
    }
    else {
        return ch;
    }
}


vector<vector<char>> read_map(string file_name) {
    vector<vector<char>> result;

    std::ifstream  data(file_name);
    std::string line;
    while(std::getline(data,line))
    {
        vector<char> line_v(line.begin(), line.end());
        result.push_back(line_v);
    }
    return result;
}

void show_green(int x, int y, char ch) {
    printf("\033[32m\033[%d;%dH%c\033[0m", y+1, x+1, ch);
    fflush(stdout);
}




