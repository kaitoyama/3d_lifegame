#include "generateFlatVector.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

std::vector<int> generateFlatVector(std::mt19937 gen,int x,int n)
{
    std::uniform_int_distribution<int> dist(0, 1);
    std::vector<int> binary_vec(x * x * x);
    for (int i = binary_vec.size() - 1; i >= 0; --i) {
        binary_vec[i] = std::pow(dist(gen),n);
    }
    return binary_vec;
}