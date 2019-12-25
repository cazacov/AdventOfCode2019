#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <unordered_map>
#include <unordered_set>

using namespace std;

#define PORTAL_OUTER 1
#define PORTAL_INNER -1

vector<vector<char>> read_map(string file_name);
void show_map(const vector<vector<char>>  &map);
void remove_deadends(vector<vector<char>> &map);
void show_green(int x, int y, char ch);
void show_cyan(int x, int y, char ch);

unsigned char display(const char &ch);

int portal_level(int x, int y, int height, int width);

const int offsets[4][2] {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

struct Point {
    int x;
    int y;
    int distance;

    Point() : x(0), y(0), distance(1){}
    Point(int _x, int _y) : x(_x), y(_y),distance(1) {}

    bool operator==(const Point &other) const{
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point &other) const{
        return x != other.x || y != other.y;
    }
};

// Required by unordered_map
template<> struct std::hash<Point> {
    std::size_t operator() (const Point &node) const {
        return (hash<int>()(node.x) << 4) ^ hash<int>()(node.y);
    }
};


struct Cell : public Point {

    Cell () : Point(0,0){}
    Cell(int x, int y) : Point(x,y) {  neighbours.empty();}
    int portal_type = 0;
    string portal_code;

    unordered_set<Point> neighbours;
};

struct Path {
    Point from;
    Point to;
    int distance;
    int level;
};


int find_path_to(Cell start, int start_level, int start_distance, int &min_distance, unordered_map<Point, Cell> &connections, vector<Path> &path);


int main() {
    auto map = read_map("input.txt");
    show_map(map);
    remove_deadends(map);

    int map_height = map.size();
    int map_width = 0;


    unordered_map<Point, Cell> connections;
    for (int y = 0; y < map_height; y++) {
        for (int x = 0; x < map[y].size(); x++) {
            if (map[y].size() > map_width) {
                map_width = map[y].size();
            }
            if (map[y][x] != '.') {
                continue;
            }
            Cell cell{x, y};
            connections[cell] = cell;
            for (int i = 0; i < 4; i++) {
                Point neighbour{
                        x + offsets[i][1],
                        y + offsets[i][0]
                };
                if (map[neighbour.y][neighbour.x] == '.') {
                    connections[cell].neighbours.insert(neighbour);
                }
            }
        }
    }

    // Simplify connections
    vector<Point> to_erase;
    do {
        for (const Point &kill: to_erase) {
            connections.erase(kill);
            show_cyan(kill.x, kill.y, '#');
        }
        to_erase.clear();

        for (const auto &pair : connections) {
            Cell &cell = connections[pair.first];
            if (cell.neighbours.size()==2) {
                Point left = *cell.neighbours.begin();
                Point right = *(++cell.neighbours.begin());
                int distance = left.distance + right.distance;
                left.distance = distance;
                right.distance = distance;
                connections[left].neighbours.erase(pair.first);
                connections[left].neighbours.insert(right);
                connections[right].neighbours.erase(pair.first);
                connections[right].neighbours.insert(left);
                to_erase.push_back(pair.first);
            }
        }
    } while(!to_erase.empty());

    unordered_map <string, Point> portals;

    // process portals
    for (const auto &pair : connections) {
        Cell &cell = connections[pair.first];

        for (int i = 0; i < 4; i++) {
            Point neighbour{
                    cell.x + offsets[i][1],
                    cell.y + offsets[i][0]
            };
            if (isalpha(map[neighbour.y][neighbour.x])) {
                // We have an alpha character here
                string portal_name = string(1,map[neighbour.y][neighbour.x]);
                Point neighbour2{
                        cell.x + 2*offsets[i][1],
                        cell.y + 2*offsets[i][0]
                };
                portal_name += string(1,map[neighbour2.y][neighbour2.x]);
                // normalize name
                std::sort(portal_name.begin(), portal_name.end());
                cell.portal_type = portal_level(cell.x, cell.y, map_width, map_height);
                cell.portal_code = portal_name;
                if (portals.count(portal_name)) {
                    Point other_p = portals[portal_name];
                    Cell &other = connections[other_p];
                    other_p.distance = 1;
                    cell.neighbours.insert(other_p);
                    Point cell_cord {cell.x, cell.y};
                    other.neighbours.insert(cell_cord);
                    show_green(cell.x, cell.y, cell.portal_type == PORTAL_OUTER ? 'O' : 'o');
                    show_green(other.x, other.y, other.portal_type == PORTAL_OUTER ? 'O' : 'o');
                } else {
                    cell.portal_code = portal_name;
                    portals[portal_name] = cell;
                }
            }

        }
    }


    int shortest_path = INT32_MAX;

    Cell from = connections[portals["AA"]];
    Cell to = connections[portals["ZZ"]];

    show_green(1, map_height+1, '_');

    vector<Path> path;

    int path_length = find_path_to(from, 0, 0, shortest_path, connections, path);
    cout << "Path length: " << path_length << endl;
    return 0;
}



int portal_level(int x, int y, int width, int height) {

    if (x < 5 || x >= width-5) {
        return PORTAL_OUTER;
    }
    if (y < 5 || y >= height-5) {
        return +1;
    }
    return PORTAL_INNER;
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

int find_path_to(Cell start, int start_level, int start_distance, int &min_distance, unordered_map<Point, Cell> &connections, vector<Path> &path) {

    if (start.portal_type && start.portal_code == "ZZ" && start_level == 0) {
        return start_distance;
    }

    if (start_level < 0) {
        cout << "That cannot be true!";
        exit(-1);
    }
    if (start_level > 100) {
        cout << "Recursion too deep";
        exit(-1);
    }

    for (const Point &neighbour_cord : start.neighbours) {
        Cell &neighbour = connections[neighbour_cord];

        if (!path.empty() && path.back().from == neighbour_cord) {
            // We have just came from that cell
            continue;
        }

        int new_level = start_level;
        if (start.portal_type && neighbour.portal_type && neighbour_cord.distance == 1) {
            // Warp from portal to portal
            new_level += neighbour.portal_type == PORTAL_OUTER ? 1 : -1;
        }


        if (neighbour.portal_type) {
            if (neighbour.portal_code == "AA" || neighbour.portal_code == "ZZ") {
                if (start_level != 0) {
                    continue;
                }
            }
            else {
                if (new_level == 0 && neighbour.portal_type == PORTAL_OUTER) { // allowed to exit only through AA or ZZ
                    continue;
                }
            }

            // Check if we someday have already passed through that portal
            bool loop_found = false;
            for (const auto &path_elem : path) {
                if (path_elem.from == start && path_elem.to == neighbour_cord) {
                    loop_found = true;
                    break;
                }
            }
            if (loop_found) {
                continue;
            }


        }
        int neighbour_distance = start_distance + neighbour_cord.distance;
        if (neighbour_distance > min_distance) {
            continue;
        }

        Path path_element = Path {
            start, neighbour_cord, neighbour_cord.distance, new_level
        };
        path.push_back(path_element);
        int result = find_path_to(neighbour, new_level, neighbour_distance, min_distance, connections, path);
        path.pop_back();
        if (result > 0 && result < min_distance) {
            min_distance = result;
            return min_distance;
        }
    }
    return 0;
}





