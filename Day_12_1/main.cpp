#include <iostream>
#include <vector>
#include <numeric>
#include <iomanip>

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


int sign(int i);

int main() {

    const int STEPS = 1000;

    SpaceBody io (4,1,1);
    SpaceBody europa(11, -18, -1);
    SpaceBody ganymede(-2, -10, -4);
    SpaceBody calisto(-7, -2, 14);


    std::vector<SpaceBody> moons {io, europa, ganymede, calisto};

    for (int step = 0; step < STEPS; step++) {

        for (const auto &moon: moons) {
            std::cout << "\t(" << std::setw(4) << moon.position[0] << "," << std::setw(4)<< moon.position[1] << "," << std::setw(4)<< moon.position[2] << ")\t";
        }
        for (const auto &moon: moons) {
            std::cout << "\t(" << std::setw(4) << moon.velocity[0] << "," << std::setw(4)<< moon.velocity[1] << "," << std::setw(4)<< moon.velocity[2] << ")\t";
        }
        std::cout << std::endl;

        // apply gravity
        for (int i = 0; i < moons.size(); i++) {
            for (int j = i+1; j < moons.size(); j++) {
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
    }
    int total_energy = std::accumulate(moons.begin(), moons.end(), 0, [](int acc, const SpaceBody &moon) {
        return acc + moon.kinetic_energy()*moon.potential_energy();
    });

    std::cout << "Total energy after "<< STEPS << " steps: " << total_energy << std::endl;
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