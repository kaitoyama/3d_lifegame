#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/multiprecision/cpp_int.hpp>
namespace mp = boost::multiprecision;
// 任意長整数型
using Bint = mp::cpp_int;
// 仮数部が10進数で1024桁の浮動小数点数型(TLEしたら小さくする)
using Real = mp::number<mp::cpp_dec_float<1024>>;

std::vector<std::string> binaryToDecimal(const std::vector<int>& binary) {
    std::vector<std::string> decimal;
    Bint power = 1;
    Bint decimalValue = 0;

    for (int i = binary.size() - 1; i >= 0; i--) {
        if (binary[binary.size() - 1 - i] == 1) {
            decimalValue += power;
        }

        power *= 2;
    }

    while (decimalValue > 0) {
        Bint x = (decimalValue % 10);
        decimal.push_back(x.str());
        decimalValue /= 10;
    }

    std::reverse(decimal.begin(), decimal.end());

    return decimal;
}