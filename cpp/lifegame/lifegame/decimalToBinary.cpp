#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>
#include <bitset>
#include <boost/multiprecision/cpp_int.hpp>
#include "decimalToBinary.h"
namespace mp = boost::multiprecision;
// ”CˆÓ’·®”Œ^
using Bint = mp::cpp_int;

std::vector<int> decimalToBinary(const std::string decimal_string,const int dimention) {
    Bint decimal_value(decimal_string);

    std::vector<int> binary=std::vector<int>(dimention,0);

    for (int i = 0; i < dimention; i++) {
        if (mp::bit_test(decimal_value, i)) {
            binary[i] = 1;
        }
    }

    return binary;
}