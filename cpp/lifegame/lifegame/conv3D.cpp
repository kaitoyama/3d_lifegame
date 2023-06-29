#include <iostream>
#include <vector>
#include "conv3D.h"

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

    // パディングのサイズ
    int paddingDepth = filterDepth / 2;
    int paddingHeight = filterHeight / 2;
    int paddingWidth = filterWidth / 2;

    // パディング後の入力データの次元
    int paddedDepth = inputDepth + 2 * paddingDepth;
    int paddedHeight = inputHeight + 2 * paddingHeight;
    int paddedWidth = inputWidth + 2 * paddingWidth;

    // 出力データの次元
    int outputDepth = inputDepth;
    int outputHeight = inputHeight;
    int outputWidth = inputWidth;

    // パディング後の入力データの初期化
    std::vector<std::vector<std::vector<int>>> paddedInput(paddedDepth, std::vector<std::vector<int>>(paddedHeight, std::vector<int>(paddedWidth, 0)));

    // パディング
    for (int d = 0; d < inputDepth; ++d) {
        for (int h = 0; h < inputHeight; ++h) {
            for (int w = 0; w < inputWidth; ++w) {
                paddedInput[d + paddingDepth][h + paddingHeight][w + paddingWidth] = input[d][h][w];
            }
        }
    }

    // 出力データの初期化
    std::vector<std::vector<std::vector<int>>> output(outputDepth, std::vector<std::vector<int>>(outputHeight, std::vector<int>(outputWidth, 0)));

    // 畳み込み演算
    for (int fd = 0; fd < filterDepth; ++fd) {
        for (int fh = 0; fh < filterHeight; ++fh) {
            for (int fw = 0; fw < filterWidth; ++fw) {
                int currentFilterValue = filter[fd][fh][fw];
                for (int d = 0; d < outputDepth; ++d) {
                    for (int h = 0; h < outputHeight; ++h) {
                        for (int w = 0; w < outputWidth; ++w) {
                            int inputValue = paddedInput[d + fd][h + fh][w + fw];
                            output[d][h][w] += inputValue * currentFilterValue;
                        }
                    }
                }
            }
        }
    }

    
    
    
    return output;
}