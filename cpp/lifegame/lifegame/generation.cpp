#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <bitset>
#include "flatten.h"
#include "conv3D.h"
#include "bightToString.h"
#include "stringToBight.h"
#include "decimalToBinary.h"
#include "bigInt.h"


std::pair<int, std::vector<std::vector<std::string>>> generation(const std::vector<std::vector<std::vector<int>>>& first, const std::vector<int>& rule, const std::vector<std::vector<std::vector<int>>>& filter, int count)
{
    int under = rule[0];
    int birth = rule[1];
    int over = rule[2];

    int end = 0;
    std::vector<std::vector<std::vector<int>>> after = first;

    std::vector<std::vector<std::string>> proc;

    std::vector<int> after_l = flatten(after);

    std::vector<std::string> after_string = binaryToDecimal(after_l);   

    proc.push_back(after_string);

    for (int k = 0; k < count; ++k) {
        std::vector<std::vector<std::vector<int>>> before = after;
        std::vector<std::vector<std::vector<int>>> result = conv3D(before, filter);

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
        
        after_l = flatten(after);
        after_string = binaryToDecimal(after_l);

        proc.push_back(after_string);

        if (std::accumulate(after_l.begin(), after_l.end(), 0) == 0) {
            break;
        }
        else {
            end += 1;
        }
    }

    return std::make_pair(end, proc);
}

