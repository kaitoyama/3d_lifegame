#include <vector>
#include "flatten.h"
#include "conv3D.h"
#include "bightToString.h"
#include "stringToBight.h"
#include "decimalToBinary.h"
#include <numeric>


std::pair<int, std::vector<std::vector<std::string>>> generation(const std::vector<std::vector<std::vector<int>>>& first, const std::vector<int>& rule, const std::vector<std::vector<std::vector<int>>>& filter, int count)
{
    int under = rule[0];
    int birth = rule[1];
    int over = rule[2];

    int end = 0;
    std::vector<std::vector<std::vector<int>>> after = first;

    int n = after.size();

    std::vector<std::vector<std::string>> proc;

    std::vector<int> after_l = flatten(after);

    std::vector<std::string> after_string = binaryToDecimal(after_l);   

    proc.push_back(after_string);
    for (int l = 0; l < count; ++l) {
        std::vector<std::vector<std::vector<int>>> before = after;
        std::vector<std::vector<std::vector<int>>> result = conv3D(before, filter);

        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                for (size_t k = 0; k < n; ++k) {
                    if ((result[i][j][k] >= under && result[i][j][k] <= over && before[i][j][k] == 1) || (result[i][j][k] == birth && before[i][j][k] == 0)) {
                        after[i][j][k] = 1;
                    }
                    else if (result[i][j][k] < under || result[i][j][k] > over) {
                        after[i][j][k] = 0;
                    }
                }
            }
        }
        
        std::vector<int> after_l = flatten(after);
        std::vector<std::string> after_string = binaryToDecimal(after_l);

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

