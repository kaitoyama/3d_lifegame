#include "generateFlatVector.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

std::vector<int> generateFlatVector(std::mt19937 gen,int x)
{
    std::uniform_int_distribution<int> dist(0, 1);
    std::vector<int> binary_vec(x * x * x);
    for (int i = binary_vec.size() - 1; i >= 0; --i) {
        binary_vec[i] = dist(gen);
    }
    return binary_vec;
}