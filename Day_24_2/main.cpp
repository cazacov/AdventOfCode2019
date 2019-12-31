#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>

using namespace std;

struct GridCell {
    int x;
    int y;
    int level;

    bool operator==(const GridCell &other) const{
        return x == other.x && y == other.y && level == other.level;
    }

    bool operator!=(const GridCell &other) const{
        return x != other.x || y != other.y || level != other.level;
    }
};


// Required by unordered_map
template<> struct std::hash<GridCell> {
    std::size_t operator() (const GridCell &node) const {
        return (hash<int>()(node.x) << 8) ^ (hash<int>()(node.y) << 4) ^ (hash<int>()(node.level));
    }
};

unordered_set<GridCell> next_gen(const unordered_set<GridCell> &map);

vector<GridCell> get_neighbours(const unordered_set<GridCell> &map, const GridCell &cell);

int main() {

    unordered_set<GridCell> map;

    map.insert(GridCell{2,0,0});
    map.insert(GridCell{4,0,0});
    map.insert(GridCell{0,1,0});
    map.insert(GridCell{1,1,0});
    map.insert(GridCell{2,1,0});
    map.insert(GridCell{3,1,0});
    map.insert(GridCell{4,1,0});
    map.insert(GridCell{1,2,0});
    map.insert(GridCell{3,3,0});
    map.insert(GridCell{0,4,0});
    map.insert(GridCell{1,4,0});

    for (int i = 0; i < 200; i++) {
        map = next_gen(map);
    }

    cout << "Bugs found: " << map.size() << endl;
    return 0;
}

unordered_set<GridCell> next_gen(const unordered_set<GridCell> &map) {

    unordered_set<GridCell> result;
    unordered_set<GridCell> todo;

    // process existing cells
    for (const GridCell &cell: map) {
        vector<GridCell> neighbour_cells = get_neighbours(map, cell);
        int live_count = 0;
        for (const auto &neighbour : neighbour_cells) {
            todo.insert(neighbour);
            if (map.count(neighbour)) {
                live_count++;
            }
        }
        if (live_count == 1) {
            result.insert(cell);
        }
    }

    for (const GridCell &cell: todo) {
        if (map.count(cell)) {
            continue;
        }
        vector<GridCell> neighbour_cells = get_neighbours(map, cell);
        int live_count = 0;
        for (const auto &neighbour : neighbour_cells) {
            if (map.count(neighbour)) {
                live_count++;
            }
        }
        if (live_count == 1 || live_count == 2) {
            result.insert(cell);
        }
    }
    return result;
}

vector<GridCell> get_neighbours(const unordered_set<GridCell> &map, const GridCell &cell) {

    vector<GridCell> result;

    int offsets[4][2] = {{1,0}, {0,1}, {-1,0}, {0,-1}};

    for (const auto &offset : offsets) {
        int x = cell.x+offset[0];
        int y = cell.y+offset[1];
        int level = cell.level;

        bool outx = x < 0 || x ==5;
        bool outy = y < 0 || y ==5;
        if (outx && outy) {
            continue;
        }
        if (outx) {
            result.push_back(GridCell{ x < 0 ? 1 : 3,2,level-1});
            continue;
        }
        if (outy) {
            result.push_back(GridCell{ 2, y < 0 ? 1 : 3,level-1});
            continue;
        }
        if (x == 2 && y ==2) {
            for (int i = 0; i < 5; i++) {
                if (offset[0]) {
                    result.push_back( GridCell {offset[0] > 0 ? 0 : 4, i, level+1});
                }
                else {
                    result.push_back( GridCell {i, offset[1] > 0 ? 0 : 4, level+1});
                }
            }
        } else {
            result.push_back( GridCell {x,y, level});
        }
    }
    return result;
}

