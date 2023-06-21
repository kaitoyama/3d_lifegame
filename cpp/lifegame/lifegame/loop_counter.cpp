#include "loop_counter.h"
#include <string>

int loop_counter(const std::vector<std::vector <std::string>>& input)
{
    std::vector<std::vector <std::string>> loop = { input.back() };
    for (int i = 0; i < input.size()-1; ++i) {
        if (input[input.size() - 1 - (i + 1)] == loop[0]) {
            break;
        }
        else {
            loop.push_back(input[input.size() - 1 - (i + 1)]);
        }
    }
    return loop.size();
}