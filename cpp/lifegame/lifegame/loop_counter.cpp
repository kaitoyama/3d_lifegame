#include "loop_counter.h"

std::vector<int> flatten(const std::vector<std::vector<std::vector<int>>>& input)
{
    std::vector<int> flattened;
    for (const auto& dim1 : input) {
        for (const auto& dim2 : dim1) {
            flattened.insert(flattened.end(), dim2.begin(), dim2.end());
        }
    }
    return flattened;
}