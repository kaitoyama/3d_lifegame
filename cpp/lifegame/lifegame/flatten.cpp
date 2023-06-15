#include "flatten.h"

#include <vector>

std::vector<int> flatten3D(const std::vector<std::vector<std::vector<int>>>& input)
{
    std::vector<int> result;
    for (const auto& layer : input) {
        for (const auto& row : layer) {
            result.insert(result.end(), row.begin(), row.end());
        }
    }
    return result;
}
