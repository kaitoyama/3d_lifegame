#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <bitset>
#include "flatten.h"
#include "conv3D.h"


std::pair<int, std::vector<int>> generation(const std::vector<std::vector<std::vector<int>>>& first, const std::vector<int>& rule, const std::vector<std::vector<std::vector<int>>>& filter, int count)
{
    int under = rule[0];
    int birth = rule[1];
    int over = rule[2];

    int end = 0;
    std::vector<std::vector<std::vector<int>>> after = first;

    std::vector<int> proc;

    for (int _ = 0; _ < count; ++_) {
        std::vector<std::vector<std::vector<int>>> before = after;
        std::vector<std::vector<std::vector<int>>> result = conv3D(before, filter);

        // Update the result tensor directly
        for (size_t i = 0; i < after.size(); ++i) {
            for (size_t j = 0; j < after[i].size(); ++j) {
                for (size_t k = 0; k < after[i][j].size(); ++k) {
                    if ((result[i][j][k] >= under) && (result[i][j][k] <= over) && (before[i][j][k] == 1)) {
                        after[i][j][k] = 1;
                    }
                    if ((result[i][j][k] == birth) && (before[i][j][k] == 0)) {
                        after[i][j][k] = 1;
                    }
                    if ((result[i][j][k] < under) || (result[i][j][k] > over)) {
                        after[i][j][k] = 0;
                    }
                }
            }
        }

        std::vector<int> after_l = flatten(after);
        std::vector<int> after_l_binary(after_l.size());
        std::transform(after_l.begin(), after_l.end(), after_l_binary.begin(), [](int x) { return x == 1 ? 1 : 0; });

        int after_int = 0;
        for (int bit : after_l_binary) {
            after_int = (after_int << 1) | bit;
        }

        proc.push_back(after_int);

        if (std::accumulate(after_l.begin(), after_l.end(), 0) == 0) {
            break;
        }
        else {
            end += 1;
        }
    }

    return std::make_pair(end, proc);
}

