#include "convertTo3DVector.h"
#include <vector>

std::vector<std::vector<std::vector<int>>> convertTo3DVector(const std::vector<int>& floatVector, int n)
{
    std::vector<std::vector<std::vector<int>>> result(n, std::vector<std::vector<int>>(n, std::vector<int>(n)));
    int index = 0;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                result[i][j][k] = floatVector[index++];
            }
        }
    }
    return result;
}
