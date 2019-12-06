#include <utility>

#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <numeric>
#include <algorithm>

class Body {
public:
    std::string code;
    Body* center;
    std::string center_code;
    std::vector<Body*> satellites;
    Body(std::string _code, std::string _center_code): code(std::move(_code)), center_code(std::move(_center_code)) {
        center = nullptr;
    }
};

class Orbit {
public:
    std::string center;
    std::string satellite;
};

std::vector<Orbit> load_orbits(const char *file_name);

int main() {
    std::vector<Orbit> orbits = std::move(load_orbits("orbits.txt"));

    std::map<std::string, Body*> bodies;
    Body* com = new Body("COM", "");
    bodies["COM"] = com;

    for (const auto &orbit: orbits) {
        Body *body = new Body(orbit.satellite, orbit.center);
        if (bodies.count(orbit.center)) {
            body->center = bodies[orbit.center];
            body->center->satellites.push_back(body);
        }
        for (auto &pair : bodies) {
            if (pair.second->center_code == body->code) {
                pair.second->center = body;
                body->satellites.push_back(pair.second);
            }
        }
        bodies[orbit.satellite] = body;
    }

    int child_orbits = 0;

    for (const auto &pair : bodies) {
        Body* body = pair.second->center;
        while (body != nullptr) {
            child_orbits++;
            body = body->center;
        }
    }
    std::cout << child_orbits << std::endl;

    // cleanup
    for (const auto &pair : bodies) {
        Body* body = pair.second;
        body->satellites.clear();
        delete body;
    }
    return 0;
}



std::vector<Orbit> load_orbits(const char *file_name) {

    std::vector<Orbit> result;

    std::ifstream  data(file_name);
    std::string line;
    while(std::getline(data,line)) {
        // trim trailing blanks
        line.erase(std::find_if(line.rbegin(), line.rend(), [](int ch) {
            return !std::isspace(ch);
        }).base(), line.end());

        int del_pos = line.find(")");
        Orbit orbit;
        orbit.center = line.substr(0, del_pos);
        orbit.satellite = line.substr(del_pos+1, 100);
        result.push_back(orbit);
    }
    return result;
}