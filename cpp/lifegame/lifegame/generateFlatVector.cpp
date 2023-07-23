#include "generateFlatVector.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

std::vector<int> generateFlatVector(std::mt19937 gen, int x, int n)
{
    std::uniform_int_distribution<int> dist(0, 1);
    std::vector<int> binary_vec(x * x * x);
    int size = binary_vec.size();
    for (int i = size - 1; i >= 0; --i) {
        int k = 1;
        for (int j = 0; j < n; ++j) {
            k = k * dist(gen);
        }
        binary_vec[i] = k;
    }
    return binary_vec;
}
