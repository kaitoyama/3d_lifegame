#pragma once
#ifndef CONVOLUTION3D_H
#define CONVOLUTION3D_H

#include <vector>

// 3次元の畳み込み関数（ゼロパディングあり）
std::vector<std::vector<std::vector<int>>> conv3D(const std::vector<std::vector<std::vector<int>>>& input, const std::vector<std::vector<std::vector<int>>>& filter);

#endif // CONVOLUTION3D_H
