#include <utility>

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <iomanip>

struct Chemical;

struct Ingredient {
    int units;
    std::string chemical_name;
};

struct Chemical {
    std::string name;
    std::vector<Ingredient> inputs;
    int output_qty;
    bool operator==(const Chemical &other) const {
        return name == other.name;
    }
};

// Required by unordered_map
template<> struct std::hash<Chemical> {
    std::size_t operator() (const Chemical &ch) const {
        return hash<std::string>()(ch.name);
    }
};

struct Reaction {
    std::vector<Ingredient> inputs;
    Ingredient output;
};

std::vector<Reaction> read_reactions(std::string file_name);

long calc_ore_amount(std::string name, long qty, std::unordered_map<std::string, Chemical> &chemicals,
        std::unordered_map<std::string, long> &overproduced);

int main() {

    Chemical ore { "ORE" };

    std::unordered_map<std::string, Chemical> chemicals;
    chemicals["ORE"] = ore;

    auto reactions = read_reactions("reactions.txt");

    for (const auto &reaction : reactions) {
        Chemical chemical;
        chemical.name = reaction.output.chemical_name;
        chemical.output_qty = reaction.output.units;
        chemical.inputs = reaction.inputs;
        chemicals[chemical.name] = chemical;
    }

    std::unordered_map<std::string, long> overproduced;
    overproduced["ORE"] = 1000000000000L;

    long amount = calc_ore_amount("FUEL", 1, chemicals, overproduced);
    std::cout << amount << std::endl;
    return 0;
}

long calc_ore_amount(std::string name, long qty, std::unordered_map<std::string, Chemical> &chemicals,
                    std::unordered_map<std::string, long> &overproduced) {
    if (name == "ORE") {
        overproduced["ORE"] -= qty;
        return qty;
    }

    auto chemical = chemicals[name];

    if (qty <= overproduced[name]) {
        overproduced[name] -= qty;
        return 0;
    }

    long todo = qty - overproduced[name];
    long factor = todo / chemical.output_qty;
    if (todo % chemical.output_qty > 0) {
        factor++;
    }
    overproduced[name] = factor * chemical.output_qty - todo;

    long res = 0;
    for (const auto &inp : chemical.inputs) {
        res += calc_ore_amount(inp.chemical_name, inp.units * factor, chemicals, overproduced);
    }
    return res;
}

Ingredient parse(std::string s) {
    Ingredient ing;

    // trim spaces
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
    std::istringstream strstr(s);
    strstr >> ing.units >> ing.chemical_name;
    return ing;
}

std::vector<Reaction> read_reactions(std::string file_name) {
    std::vector<Reaction> result;
    std::ifstream  data(file_name);
    std::string line;
    while(std::getline(data,line)) {
        int pos = line.find(" => ");
        std::string inputs = line.substr(0, pos);
        std::string out = line.substr(pos+4, 1000);
        Reaction reaction;
        reaction.output = parse(out);

        std::stringstream lineStream(inputs);
        std::string inp ;
        while(std::getline(lineStream, inp ,','))
        {
            reaction.inputs.push_back(parse(inp));
        }
        result.push_back(reaction);
    }
    return result;
}