#include <iostream>
#include "AsciiComputer.h"

using namespace std;

int main() {

    AsciiComputer computer;
    cout << computer.run("") << endl;

    string input;

    while (true) {
        getline(cin, input);
        if (input == "exit") {
            break;
        }
        cout << computer.run(input) << endl;
    }
    return 0;
}