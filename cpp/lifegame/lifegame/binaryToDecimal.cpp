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

std::string binaryToDecimal(const std::vector<int>& binary) {
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
    return y.str();
}