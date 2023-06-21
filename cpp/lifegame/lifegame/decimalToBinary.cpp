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

std::vector<int> decimalToBinary(const std::vector<int>& decimal) {
    std::vector<int> binary;
    Bint binaryValue = 0;
    Bint power = 1;

    for (int i = decimal.size() - 1; i >= 0; i--) {
        Bint digit = decimal[i];
        binaryValue += digit * power;
        power *= 10;
    }

    while (binaryValue > 0) {
        binary.push_back(static_cast<int>(binaryValue % 2));
        binaryValue /= 2;
    }

    std::reverse(binary.begin(), binary.end());

    return binary;
}