#include "generateFlatVector.h"
#include <iostream>
#include <vector>
#include <algorithm>

std::vector<int> generateFlatVector(int n,int x)
{
    std::vector<int> binary_vec(x * x * x);
    for (int i = binary_vec.size() - 1; i >= 0; --i) {
        binary_vec[i] = n % 2;
        n >>= 1;
    }
    return binary_vec;
}