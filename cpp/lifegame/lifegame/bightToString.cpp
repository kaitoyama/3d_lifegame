#include <vector>
#include "bightToString.h"


std::string bigint_to_string(std::vector<int> digit) {
    long long int N = digit.size(); // N = (�z�� digit �̒���)
    std::string str = "";
    for (int i = N - 1; i >= 0; --i) {
        str += digit[i] + '0';
    }
    return str;
}
