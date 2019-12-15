#include <utility>

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_map>

struct Chemical;

struct Ingredient {
    int units;
    std::string chemical_name;
};

struct Chemical {
    std::string name;
    std::vector<Ingredient> inputs;
    int output_qty;
    std::vector<std::string> required_by;
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

int calc_ore_amount(std::string name, int qty, std::unordered_map<std::string, Chemical> &chemicals,
        std::unordered_map<std::string, int> &overproduced);

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

    for (const auto &pair : chemicals) {
        for (const auto &ingredient : pair.second.inputs) {
            chemicals[ingredient.chemical_name].required_by.push_back(pair.first);
        }
    }

    std::unordered_map<std::string, int> overproduced;
    int amount = calc_ore_amount("FUEL", 1, chemicals, overproduced);
    std::cout << amount << std::endl;
    return 0;
}

int calc_ore_amount(std::string name, int qty, std::unordered_map<std::string, Chemical> &chemicals,
                    std::unordered_map<std::string, int> &overproduced) {
    if (name == "ORE") {
        return qty;
    }

    if (qty <= overproduced[name]) {
        overproduced[name] -= qty;
        return 0;
    }
    qty -= overproduced[name];

    auto chemical = chemicals[name];

    int factor = qty / chemical.output_qty;
    if (qty % chemical.output_qty > 0) {
        factor++;
        overproduced[name] = factor * chemical.output_qty - qty;
    }

    int res = 0;
    for (const auto &inp : chemical.inputs) {
        int produce = calc_ore_amount(inp.chemical_name, inp.units * factor, chemicals, overproduced);
        res += produce;
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