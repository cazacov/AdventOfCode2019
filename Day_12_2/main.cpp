#include <iostream>
#include <vector>
#include <numeric>
#include <iomanip>
#include <unordered_map>

struct SpaceBody {
    int position[3];
    int velocity[3];

    SpaceBody(int x, int y, int z)  {
        position[0] = x;
        position[1] = y;
        position[2] = z;
        velocity[0] = velocity[1] = velocity[2] = 0;
    }

    double potential_energy() const {
        return abs(position[0]) + abs(position[1]) + abs(position[2]);
    }
    double kinetic_energy() const {
        return abs(velocity[0]) + abs(velocity[1]) + abs(velocity[2]);
    }
};

struct Fingerprint {
    int values[8] = {0};

    Fingerprint(const std::vector<SpaceBody> &moons, int axis) {
        for (int i = 0; i < 4; i++) {
            values[i] = moons[i].position[axis];
            values[i+4] = moons[i].velocity[axis];
        }
    }

    bool operator==(const Fingerprint &other) const{
        for (int i = 0; i < 8; i++) {
            if (values[i] != other.values[i]) {
                return false;
            }
        }
        return true;
    }
};

template<> struct std::hash<Fingerprint> {
    std::size_t operator() (const Fingerprint &obj) const {
        size_t accu = 0;
        for (int i = 0; i< 8; i++) {
            accu ^= hash<int>()(obj.values[i] << i);
        }
        return accu;
    }
};


int sign(int i);
void show_coords(const std::vector<SpaceBody> &moons);
long gcd(long a, long b);


int main() {

    const long STEPS = 1000000000;

    SpaceBody io (4,1,1);
    SpaceBody europa(11, -18, -1);
    SpaceBody ganymede(-2, -10, -4);
    SpaceBody calisto(-7, -2, 14);


    std::vector<SpaceBody> moons {io, europa, ganymede, calisto};
    std::unordered_map<Fingerprint, long> map;
    long step = 0;
    long loops[3];

    for (int ax = 0; ax < 3; ax++) {
        map.clear();
        while (true) {
            step++;
            if (step % 10000 == 0) {
                std::cout << step << std::endl;
            }

            // apply gravity
            for (int i = 0; i < moons.size(); i++) {
                for (int j = i + 1; j < moons.size(); j++) {
                    SpaceBody &moon1 = moons[i];
                    SpaceBody &moon2 = moons[j];
                    for (int axis = 0; axis < 3; axis++) {
                        if (moon1.position[axis] != moon2.position[axis]) {
                            moon1.velocity[axis] += sign(moon2.position[axis] - moon1.position[axis]);
                            moon2.velocity[axis] += sign(moon1.position[axis] - moon2.position[axis]);
                        }

                    }
                }
            }
            // apply velocity
            for (auto &moon : moons) {
                for (int i = 0; i < 3; i++) {
                    moon.position[i] += moon.velocity[i];
                }
            }

            Fingerprint fp(moons, ax);
            if (map.count(fp) > 0) {
                std::cout << "Axis " << ax << "  Loop found:" << step - map[fp] << std::endl;
                loops[ax] = step - map[fp];
                break;
            }
            map[fp] = step;
        }
    }

    std::cout << "All 3 loops found:  ";
    for (int i = 0; i < 3; i++) {
        std::cout << loops[i] << "  ";
    }
    std::cout << std::endl;

    auto gcd1 = gcd(loops[0], loops[1]);
    auto gcd2 = gcd(loops[1], loops[2]);
    auto nok = loops[0] / gcd1 * loops[1] / gcd2 * loops[2];

    std::cout << "Cycle: " << nok << std::endl;
    return 0;
}


int sign(int arg) {
    if (arg < 0) {
        return -1;
    } else if (arg > 0) {
        return 1;
    }
    else {
        return 0;
    }
}

void show_coords(const std::vector<SpaceBody> &moons) {
    for (const auto &moon: moons) {
        std::cout << "   " << std::setw(4) << moon.position[0] << "," << std::setw(4)<< moon.position[1] << "," << std::setw(4)<< moon.position[2] << "  [" << std::setw(4) << moon.potential_energy() << "]   ";
    }
    for (const auto &moon: moons) {
        std::cout << "   " << std::setw(4) << moon.velocity[0] << "," << std::setw(4)<< moon.velocity[1] << "," << std::setw(4)<< moon.velocity[2] << "  [" << std::setw(4) << moon.kinetic_energy() << "]   ";
    }
    std::cout << std::endl;
}

long gcd(long a, long b) {
    while (b != 0) {
        auto t = b;
        b = a % b;
        a = t;
    }
    return a;
}
