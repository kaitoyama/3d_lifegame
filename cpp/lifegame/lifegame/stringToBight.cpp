#include "stringToBight.h"
#include <vector>
#include <string>

std::vector<int> string_to_bigint(std::string S) {
    int N = S.size(); // N = (•¶Žš—ñ S ‚Ì’·‚³)
    std::vector<int> digit(N);
    for (int i = 0; i < N; ++i) {
        digit[i] = S[N - i - 1] - '0'; // 10^i ‚ÌˆÊ‚Ì”
    }
    return digit;
}