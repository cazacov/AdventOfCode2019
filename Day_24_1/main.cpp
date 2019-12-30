#include <iostream>
#include <vector>
#include <unordered_set>
using namespace std;

long calculate_rating(vector<vector<int>> &map);
vector<vector<int>> next_gen(vector<vector<int>> &map);

void show_map(vector<vector<int>> &map);

int main() {

    vector<vector<int>> map;

    map = {{0,0,1,0,1},
           {1,1,1,1,1},
           {0,1,0,0,0},
           {0,0,0,1,0},
           {1,1,0,0,0}};

    long rating = calculate_rating(map);
    unordered_set<long> layouts;
    //layouts.insert(rating);

    while (!layouts.count(rating)) {
        layouts.insert(rating);
        map = next_gen(map);
        rating = calculate_rating(map);

//        show_map(map);
    }
    show_map(map);
    cout << "First double rating: " << rating << endl;
    return 0;
}

void show_map(vector<vector<int>> &map) {
    cout << endl;
    for (int r = 0; r < map.size(); r++) {
        for (int c = 0; c < map[r].size(); c++) {
            if (map[r][c]) {
                cout << "#";
            } else {
                cout << ".";
            }
        }
        cout << endl;
    }
}

vector<vector<int>> next_gen(vector<vector<int>> &map) {

    static int offsets[4][2] {{0,1}, {1,0}, {0,-1}, {-1,0}};

    vector<vector<int>> result;

    for (int r = 0; r < map.size(); r++) {
        vector<int> row;
        for (int c = 0; c < map[r].size(); c++) {
            int count = 0;
            for (const auto &offset : offsets) {
                int x = c + offset[0];
                int y = r + offset[1];
                if (x < 0 || y < 0 || x == map[r].size() || y == map.size()) {
                    continue;
                }
                if (map[y][x]) {
                    count++;
                }
            }
            int new_val = 0;
            if (map[r][c]) {
                new_val = (count == 1) ? 1 : 0;
            } else {
                new_val = (count  == 1 || count  == 2) ? 1: 0;
            }
            row.push_back(new_val);
        }
        result.push_back(row);
    }
    return result;
}

long calculate_rating(vector<vector<int>> &map) {

    int pos = 0;
    long result = 0;

    for (int r = 0; r < map.size(); r++) {
        for (int c = 0; c < map[r].size(); c++) {
            if (map[r][c]) {
                result += 1L << pos;
            }
            pos++;
        }
    }
    return result;
}