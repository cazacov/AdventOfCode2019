#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <algorithm>
#include <iomanip>
#include "math.h"

struct SpaceCoord {
    int x;
    int y;
    double angle() const {
        double a = -atan2(-x, -y);
        if (a < 0) { a+=2*M_PI;}
        return a;
    };
    int distance() { return x*x + y*y;}
    bool operator==(const SpaceCoord &other) const {
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
    std::unordered_map<SpaceCoord, std::vector<SpaceCoord>> best_map;

    for (int i = 0; i < asteroids.size(); i++) {
        auto &center = asteroids[i];
        std::unordered_map<SpaceCoord, std::vector<SpaceCoord>> map;
        for (const auto &asteroid: asteroids) {
            int dx = asteroid.x - center.x;
            int dy = asteroid.y - center.y;
            if (dx == 0 && dy == 0) {   // Skip itself
                continue;
            }
            int gcd = greatest_common_divisor(abs(dx), abs(dy));
            SpaceCoord key { dx / gcd, dy / gcd};
            SpaceCoord ast {dx, dy};
            std::vector<SpaceCoord> &beam = map[key];
            auto it  = beam.begin();
            while (it != beam.end() && (*it).distance() < ast.distance()) {
                it++;
            }
            beam.insert(it, ast);
        }

        if (map.size() > max_visible) {
            max_visible = map.size();
            best_coord = center;
            best_map = map;
        }
    }
    std::cout << max_visible << std::endl;

    std::vector<std::vector<SpaceCoord>> beams;
    std::transform(best_map.begin(), best_map.end(), std::back_inserter(beams),
        [](const std::pair<const SpaceCoord, std::vector<SpaceCoord>> &pair) { return pair.second; });


    std::sort(beams.begin(), beams.end(),
            [](const std::vector<SpaceCoord> &a, const std::vector<SpaceCoord> &b) -> bool {
        return a[0].angle() < b[0].angle();
    });

    std::cout << "Giant laser installed at coordinates " << best_coord.x << "," << best_coord.y << std::endl;

    int vaporized = 0;
    int beam = 0;
    SpaceCoord last_asteroid;
    while (vaporized < 200) {
        while (beams[beam].empty()) {
            beam++;
            if (beam == beams.size()) {
                beam = 0;
            }
        }
        last_asteroid = SpaceCoord {
                beams[beam][0].x + best_coord.x,
                beams[beam][0].y + best_coord.y,
        };
        beams[beam].erase(beams[beam].begin());
        vaporized++;
        beam++;
        if (beam == beams.size()) {
            beam = 0;
        }
        std::cout << std::setw(3) << vaporized
          << "\t Vaporized the asteroid in position "
          << std::setw(2) << last_asteroid.x << ","
          << std::setw(2) << last_asteroid.y << std::endl;

    }

    std::cout << last_asteroid.x * 100 + last_asteroid.y;
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
