#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/multiprecision/cpp_int.hpp>
namespace mp = boost::multiprecision;
// ”CˆÓ’·®”Œ^
using Bint = mp::cpp_int;
// ‰¼”•”‚ª10i”‚Å1024Œ…‚Ì•‚“®¬”“_”Œ^(TLE‚µ‚½‚ç¬‚³‚­‚·‚é)
using Real = mp::number<mp::cpp_dec_float<1024>>;

std::vector<std::string> binaryToDecimal(const std::vector<int>& binary) {
    std::vector<std::string> decimal;
    Bint power = 1;
    Bint decimalValue = 0;
    Bint y = 0;
    for (int i = 0; i < binary.size(); i++)
    {
        if (binary[i] == 1)
        {
            mp::bit_set(y, i);
        }
    }
    for (char digit : y.str()) {
        decimal.push_back(std::string(1, digit));
    }
    return decimal;
}