/**
 * @author Brian Weir (https://github.com/bweir27)
 * this project was made in JetBrains CLion
 */

#include <iostream>

#include "RoundRobin.h"

using namespace std;

int main(int argc, char** argv) {
    string fileName = argv[1];
    uint32_t block = stoul(argv[2], nullptr, 10);
    uint32_t slice = stoi(argv[3], nullptr, 10);

    RoundRobin rr(fileName, block, slice);

    return 0;
}
