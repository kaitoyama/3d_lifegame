#include <iostream>
#include <vector>
#include "conv3D.h"

// 3次元の畳み込み関数
std::vector<std::vector<std::vector<int>>> conv3D(const std::vector<std::vector<std::vector<int>>>& input, const std::vector<std::vector<std::vector<int>>>& filter)
{
    // 入力データの次元
    int inputDepth = input.size();
    int inputHeight = input[0].size();
    int inputWidth = input[0][0].size();

    // フィルタの次元
    int filterDepth = filter.size();
    int filterHeight = filter[0].size();
    int filterWidth = filter[0][0].size();

    // ゼロパディング後の出力データの次元
    int outputDepth = inputDepth;
    int outputHeight = inputHeight + 2 * (filterHeight / 2);
    int outputWidth = inputWidth + 2 * (filterWidth / 2);

    // ゼロパディング後の入力データの作成
    std::vector<std::vector<std::vector<int>>> paddedInput(inputDepth, std::vector<std::vector<int>>(outputHeight, std::vector<int>(outputWidth, 0)));
    for (int d = 0; d < inputDepth; ++d) {
        for (int h = 0; h < inputHeight; ++h) {
            for (int w = 0; w < inputWidth; ++w) {
                paddedInput[d][h + (filterHeight / 2)][w + (filterWidth / 2)] = input[d][h][w];
            }
        }
    }

    // 出力データの初期化
    std::vector<std::vector<std::vector<int>>> output(outputDepth, std::vector<std::vector<int>>(outputHeight - filterHeight + 1, std::vector<int>(outputWidth - filterWidth + 1, 0)));

    // 畳み込み演算
    for (int d = 0; d < outputDepth; ++d) {
        for (int h = 0; h < outputHeight - filterHeight + 1; ++h) {
            for (int w = 0; w < outputWidth - filterWidth + 1; ++w) {
                for (int fd = 0; fd < filterDepth; ++fd) {
                    for (int fh = 0; fh < filterHeight; ++fh) {
                        for (int fw = 0; fw < filterWidth; ++fw) {
                            output[d][h][w] += paddedInput[d][h + fd][w + fw] * filter[fd][fh][fw];
                        }
                    }
                }
            }
        }
    }

    return output;
}