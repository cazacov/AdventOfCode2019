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

std::vector<Body *> path_to(Body *target);

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
    std::cout << "Total number of orbits: " << child_orbits << std::endl;

    std::vector<Body*> path_to_you = path_to(bodies["YOU"]);
    std::vector<Body*> path_to_Santa = path_to(bodies["SAN"]);

    int common_pos = 0;
    while (path_to_you[common_pos] == path_to_Santa[common_pos]) {
        common_pos++;
    }

    size_t transfers = path_to_you.size() + path_to_Santa.size() - 2 * common_pos;
    std::cout << transfers << std::endl;
    std::cout << "Hops to Santa: " << transfers << std::endl;

    // cleanup
    for (const auto &pair : bodies) {
        Body* body = pair.second;
        body->satellites.clear();
        delete body;
    }
    return 0;
}

std::vector<Body *> path_to(Body *target) {

    std::vector<Body *> result;
    while (target->center) {
        result.insert(result.begin(), target->center);
        target = target->center;
    }
    return result;
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