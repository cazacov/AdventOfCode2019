#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_map>

struct SpaceCoord {
    int x;
    int y;
    bool operator==(const SpaceCoord &other) const{
        return x == other.x && y == other.y;
    }
};

// Required by unordered_map
template<> struct std::hash<SpaceCoord> {
    std::size_t operator() (const SpaceCoord &node) const {
        return (hash<int>()(node.x) << 4) ^ hash<int>()(node.y);
    }
};

std::vector<SpaceCoord> load_map(const char *file_name);

int greatest_common_divisor(int n1, int n2);

int main() {

    auto asteroids = load_map("starmap.txt");

    int max_visible = 0;
    SpaceCoord best_coord;


    for (int i = 0; i < asteroids.size(); i++) {
        auto &center = asteroids[i];
        std::unordered_map<SpaceCoord, int> map;
        for (const auto &asteroid: asteroids) {
            int dx = asteroid.x - center.x;
            int dy = asteroid.y - center.y;
            if (dx == 0 && dy == 0) {   // Skip itself
                continue;
            }
            int gcd = greatest_common_divisor(abs(dx), abs(dy));
            SpaceCoord key { dx / gcd, dy / gcd};
            map[key] += 1;
        }

        if (map.size() > max_visible) {
            max_visible = map.size();
            best_coord = center;
        }
    }

    std::cout << max_visible << std::endl;
    return 0;
}

//  Euclid's algorithm
int greatest_common_divisor(int n1, int n2) {

    if (n1 == 0) {
        return n2;
    }
    if (n2 == 0) {
        return n1;
    }

    while (n1 != n2) {
        if (n1 > n2) {
            n1 -= n2;
        } else {
            n2 -= n1;
        }
    }
    return n1;
}

std::vector<SpaceCoord> load_map(const char *file_name) {

    std::vector<SpaceCoord> result;
    std::ifstream  data(file_name);
    std::string line;
    int y = 0;
    while(std::getline(data,line)) {
        for (int x = 0; x < line.length(); x++) {
            if (line[x] == '#') {
                result.push_back(SpaceCoord{x,y});
            }
        }
        y++;
    }
    return result;
}
