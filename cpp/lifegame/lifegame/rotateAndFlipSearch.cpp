#include <vector>
#include <iostream>
#include <set>
#include <algorithm>
#include "flatten.h"
#include "binaryToDecimal.h"
#include "decimalToBinary.h"
#include "convertTo3DVector.h"

typedef std::vector<std::vector<std::vector<int>>> ThreeDimensionalVector;

// 各次元の要素数を計算
int getAxisSize(const std::vector<std::vector<std::vector<int>>>& arr, int dim) {
    if (dim == 0) return arr.size();
    if (dim == 1) return arr[0].size();
    if (dim == 2) return arr[0][0].size();
    return -1;
}

// 外側の面が全て0かどうか判定
bool isOuterFaceZero(const std::vector<std::vector<std::vector<int>>>& arr, int dim, int pos) {
    if (dim == 0) {
        for (int i = 0; i < getAxisSize(arr, dim + 1); i++) {
            for (int j = 0; j < getAxisSize(arr, dim + 2); j++) {
                if (arr[pos][i][j] != 0) {
                    return false;
                }
            }
        }
        return true;
    }
    else if (dim == 1) {
        for (int i = 0; i < getAxisSize(arr, dim - 1); i++) {
            for (int j = 0; j < getAxisSize(arr, dim + 1); j++) {
                if (arr[i][pos][j] != 0) {
                    return false;
                }
            }
        }
        return true;
    }
    else if (dim == 2) {
        for (int i = 0; i < getAxisSize(arr, dim - 2); i++) {
            for (int j = 0; j < getAxisSize(arr, dim - 1); j++) {
                if (arr[i][j][pos] != 0) {
                    return false;
                }
            }
        }
        return true;
    }
    return false;
}

std::vector<std::vector<std::vector<std::vector<int>>>> sliceByZeroFace(const std::vector<std::vector<std::vector<int>>>& arr) {
    std::vector<std::vector<std::vector<std::vector<int>>>> slices;
    std::vector<std::vector<std::vector<int>>> slice;
    for (int i = 0; i < arr.size(); i++)
    {
        int check = 1;
        for (int j = 0; j < arr[i].size(); j++)
        {
            for (int k = 0; k < arr[i][j].size(); k++)
            {
                for (int l = 0; l < arr[i][j][k]; l++)
                {
                    if (arr[i][j][k] == 1)
                    {
                        check = 0;
                    }
                }
            }
        }
        if (check == 0)
        {
            slice.push_back(arr[i]);
        }
        else if (check == 1 && slice.size() > 0)
        {
            slices.push_back(slice);
            slice.clear();
        }
        else
        {
            continue;
        }
    }

    if (slice.size() > 0)
    {
        slices.push_back(slice);
    }
    return slices;
}


// 隣接する1のセルをまとめるDFS関数
void dfs(int x, int y, int z, ThreeDimensionalVector& input, std::vector<std::vector<std::vector<bool>>>& visited, ThreeDimensionalVector& outputVector) {
    const int DIM_X = input.size();
    const int DIM_Y = input[0].size();
    const int DIM_Z = input[0][0].size();
    if (x < 0 || y < 0 || z < 0 || x >= DIM_X || y >= DIM_Y || z >= DIM_Z || visited[x][y][z] || input[x][y][z] == 0) {
        return;
    }

    visited[x][y][z] = true;
    outputVector[x][y][z] = 1;

    // 隣接する6方向に対して再帰呼び出し
    dfs(x + 1, y, z, input, visited, outputVector);
    dfs(x - 1, y, z, input, visited, outputVector);
    dfs(x, y + 1, z, input, visited, outputVector);
    dfs(x, y - 1, z, input, visited, outputVector);
    dfs(x, y, z + 1, input, visited, outputVector);
    dfs(x, y, z - 1, input, visited, outputVector);
}

// 三次元ベクトルをまとめて出力する関数
std::vector<ThreeDimensionalVector> collectThreeDimensionalVectors(ThreeDimensionalVector& input) {
    const int DIM_X = input.size();
    const int DIM_Y = input[0].size();
    const int DIM_Z = input[0][0].size();
    std::vector<ThreeDimensionalVector> output;

    // 未訪問のフラグ配列を初期化
    std::vector<std::vector<std::vector<bool>>> visited(DIM_X, std::vector<std::vector<bool>>(DIM_Y, std::vector<bool>(DIM_Z, false)));

    for (int x = 0; x < DIM_X; ++x) {
        for (int y = 0; y < DIM_Y; ++y) {
            for (int z = 0; z < DIM_Z; ++z) {
                // 未訪問の1のセルを見つけたら、新たな部分三次元ベクトルとしてDFSで探索し、outputに追加
                if (input[x][y][z] == 1 && !visited[x][y][z]) {
                    output.push_back(std::vector<std::vector<std::vector<int>>>(DIM_X, std::vector<std::vector<int>>(DIM_Y, std::vector<int>(DIM_Z, 0))));
                    dfs(x, y, z, input, visited, output.back());
                }
            }
        }
    }

    return output;
}

std::vector<std::vector<std::vector<int>>> removeOuterFaces(const std::vector<std::vector<std::vector<int>>>& arr) {
    int xSize = getAxisSize(arr, 0);
    int ySize = getAxisSize(arr, 1);
    int zSize = getAxisSize(arr, 2);

    // 外側の面を取り除いた後のサイズを計算
    int xStart = 0, xEnd = xSize;
    int yStart = 0, yEnd = ySize;
    int zStart = 0, zEnd = zSize;

    while (xStart < xSize && isOuterFaceZero(arr, 0, xStart)) {
        xStart++;
    }
    while (xEnd > xStart && isOuterFaceZero(arr, 0, xEnd - 1)) {
        xEnd--;
    }
    while (yStart < ySize && isOuterFaceZero(arr, 1, yStart)) {
        yStart++;
    }
    while (yEnd > yStart && isOuterFaceZero(arr, 1, yEnd - 1)) {
        yEnd--;
    }
    while (zStart < zSize && isOuterFaceZero(arr, 2, zStart)) {
        zStart++;
    }
    while (zEnd > zStart && isOuterFaceZero(arr, 2, zEnd - 1)) {
        zEnd--;
    }

    // 新しい三次元ベクターを生成
    std::vector<std::vector<std::vector<int>>> result;
    for (int i = xStart; i < xEnd; i++) {
        std::vector<std::vector<int>> yVec;
        for (int j = yStart; j < yEnd; j++) {
            std::vector<int> zVec;
            for (int k = zStart; k < zEnd; k++) {
                zVec.push_back(arr[i][j][k]);
            }
            yVec.push_back(zVec);
        }
        result.push_back(yVec);
    }

    return result;
}

// 3次元ベクターをx軸周りに90度回転を行列を利用して計算
std::vector<std::vector<std::vector<int>>> rotateX(const std::vector<std::vector<std::vector<int>>>& arr) {

    int xSize = getAxisSize(arr, 0);
    int ySize = getAxisSize(arr, 1);
    int zSize = getAxisSize(arr, 2);

    int resultxSize = ySize;
    int resultySize = xSize;
    int resultzSize = zSize;

    std::vector<std::vector<std::vector<int>>> result(resultxSize);
    for (auto& yVec : result) {
        yVec.resize(resultySize);
        for (auto& zVec : yVec) {
            zVec.resize(resultzSize);
        }
    }

    for (int i = 0; i < resultxSize; i++) {

        std::vector<std::vector<int>>& yVec = result[i];

        for (int j = 0; j < resultySize; j++) {

            std::vector<int>& zVec = yVec[j];

            for (int k = 0; k < resultzSize; k++) {
                zVec[k] = arr[j][i][k];
            }

        }

    }

    return result;
}


// 3次元ベクターをy軸周りに90度回転
std::vector<std::vector<std::vector<int>>> rotateY(const std::vector<std::vector<std::vector<int>>>& arr) {
    int xSize = getAxisSize(arr, 0);
    int ySize = getAxisSize(arr, 1);
    int zSize = getAxisSize(arr, 2);

    int resultxSize = xSize;
    int resultySize = zSize;
    int resultzSize = ySize;

    std::vector<std::vector<std::vector<int>>> result(resultxSize);
    for (auto& yVec : result) {
        yVec.resize(resultySize);
        for (auto& zVec : yVec) {
            zVec.resize(resultzSize);
        }
    }


    for (int i = 0; i < resultxSize; i++) {

        std::vector<std::vector<int>>& yVec = result[i];

        for (int j = 0; j < resultySize; j++) {

            std::vector<int>& zVec = yVec[j];

            for (int k = 0; k < resultzSize; k++) {
                zVec[k] = arr[i][resultzSize-k-1][j];
            }

        }

    }

    return result;
}

// 3次元ベクターをz軸周りに90度回転
std::vector<std::vector<std::vector<int>>> rotateZ(const std::vector<std::vector<std::vector<int>>>& arr) {
    int xSize = getAxisSize(arr, 0);
    int ySize = getAxisSize(arr, 1);
    int zSize = getAxisSize(arr, 2);

    int resultxSize = zSize;
    int resultySize = ySize;
    int resultzSize = xSize;

    std::vector<std::vector<std::vector<int>>> result(resultxSize, std::vector<std::vector<int>>(resultySize, std::vector<int>(resultzSize)));
    for (int i = 0; i < resultxSize; i++) {
        std::vector<std::vector<int>> yVec;
        for (int j = 0; j < resultySize; j++) {
            std::vector<int> zVec;
            for (int k = 0; k < resultzSize; k++) {
                zVec.push_back(arr[resultzSize - k - 1][j][i]);
            }
            yVec.push_back(zVec);
        }
        result.push_back(yVec);
    }

    return result;
}

// 3次元ベクターの回転を全て列挙
std::vector<std::vector<std::vector<std::vector<int>>>> enumerateRotations(const std::vector<std::vector<std::vector<int>>>& arr) {
    std::vector<std::vector<std::vector<int>>> rotatedX = rotateX(arr);
    std::vector<std::vector<std::vector<int>>> rotatedY = rotateY(arr);
    std::vector<std::vector<std::vector<int>>> rotatedXX = rotateX(rotatedX);
    std::vector<std::vector<std::vector<int>>> rotatedXY = rotateY(rotatedX);
    std::vector<std::vector<std::vector<int>>> rotatedYX = rotateX(rotatedY);
    std::vector<std::vector<std::vector<int>>> rotatedYY = rotateY(rotatedY);
    std::vector<std::vector<std::vector<int>>> rotatedXXX = rotateX(rotatedXX);
    std::vector<std::vector<std::vector<int>>> rotatedXXY = rotateY(rotatedXX);
    std::vector<std::vector<std::vector<int>>> rotatedXYX = rotateX(rotatedXY);
    std::vector<std::vector<std::vector<int>>> rotatedXYY = rotateY(rotatedXY);
    std::vector<std::vector<std::vector<int>>> rotatedYXX = rotateX(rotatedYX);
    std::vector<std::vector<std::vector<int>>> rotatedYYX = rotateX(rotatedYY);
    std::vector<std::vector<std::vector<int>>> rotatedYYY = rotateY(rotatedYY);
    std::vector<std::vector<std::vector<int>>> rotatedXXXY = rotateY(rotatedXXX);
    std::vector<std::vector<std::vector<int>>> rotatedXXYX = rotateX(rotatedXXY);
    std::vector<std::vector<std::vector<int>>> rotatedXXYY = rotateY(rotatedXXY);
    std::vector<std::vector<std::vector<int>>> rotatedXYXX = rotateX(rotatedXYX);
    std::vector<std::vector<std::vector<int>>> rotatedXYYY = rotateY(rotatedXYY);
    std::vector<std::vector<std::vector<int>>> rotatedYXXX = rotateX(rotatedYXX);
    std::vector<std::vector<std::vector<int>>> rotatedYYYX = rotateX(rotatedYYY);
    std::vector<std::vector<std::vector<int>>> rotatedXXXYX = rotateX(rotatedXXXY);
    std::vector<std::vector<std::vector<int>>> rotatedXYXXX = rotateX(rotatedXYXX);
    std::vector<std::vector<std::vector<int>>> rotatedXYYYX = rotateX(rotatedXYYY);

    std::vector<std::vector<std::vector<std::vector<int>>>> result;

    result.resize(24);

    result[0] = arr;
    result[1] = rotatedX;
    result[2] = rotatedY;
    result[3] = rotatedXX;
    result[4] = rotatedXY;
    result[5] = rotatedYX;
    result[6] = rotatedYY;
    result[7] = rotatedXXX;
    result[8] = rotatedXXY;
    result[9] = rotatedXYX;
    result[10] = rotatedXYY;
    result[11] = rotatedYXX;
    result[12] = rotatedYYX;
    result[13] = rotatedYYY;
    result[14] = rotatedXXXY;
    result[15] = rotatedXXYX;
    result[16] = rotatedXXYY;
    result[17] = rotatedXYXX;
    result[18] = rotatedXYYY;
    result[19] = rotatedYXXX;
    result[20] = rotatedYYYX;
    result[21] = rotatedXXXYX;
    result[22] = rotatedXYXXX;
    result[23] = rotatedXYYYX;


    return result;
}

// 3次元ベクターをxy平面に反転
std::vector<std::vector<std::vector<int>>> flipXY(const std::vector<std::vector<std::vector<int>>>& arr) {
    int xSize = getAxisSize(arr, 0);
    int ySize = getAxisSize(arr, 1);
    int zSize = getAxisSize(arr, 2);

    std::vector<std::vector<std::vector<int>>> result;
    for (int i = 0; i < xSize; i++) {
        std::vector<std::vector<int>> yVec;
        for (int j = 0; j < ySize; j++) {
            std::vector<int> zVec;
            for (int k = 0; k < zSize; k++) {
                zVec.push_back(arr[i][ySize - j - 1][k]);
            }
            yVec.push_back(zVec);
        }
        result.push_back(yVec);
    }

    return result;
}

// 3次元ベクターをyz平面に反転
std::vector<std::vector<std::vector<int>>> flipYZ(const std::vector<std::vector<std::vector<int>>>& arr) {
    int xSize = getAxisSize(arr, 0);
    int ySize = getAxisSize(arr, 1);
    int zSize = getAxisSize(arr, 2);

    std::vector<std::vector<std::vector<int>>> result;
    for (int i = 0; i < xSize; i++) {
        std::vector<std::vector<int>> yVec;
        for (int j = 0; j < ySize; j++) {
            std::vector<int> zVec;
            for (int k = 0; k < zSize; k++) {
                zVec.push_back(arr[i][j][zSize - k - 1]);
            }
            yVec.push_back(zVec);
        }
        result.push_back(yVec);
    }

    return result;
}

// 3次元ベクターをzx平面に反転
std::vector<std::vector<std::vector<int>>> flipZX(const std::vector<std::vector<std::vector<int>>>& arr) {
    int xSize = getAxisSize(arr, 0);
    int ySize = getAxisSize(arr, 1);
    int zSize = getAxisSize(arr, 2);

    std::vector<std::vector<std::vector<int>>> result;
    for (int i = 0; i < xSize; i++) {
        std::vector<std::vector<int>> yVec;
        for (int j = 0; j < ySize; j++) {
            std::vector<int> zVec;
            for (int k = 0; k < zSize; k++) {
                zVec.push_back(arr[xSize - i - 1][j][k]);
            }
            yVec.push_back(zVec);
        }
        result.push_back(yVec);
    }

    return result;
}

// 3次元ベクターの反転を全て列挙
std::vector<std::vector<std::vector<std::vector<int>>>> enumerateFlip(const std::vector<std::vector<std::vector<int>>>& arr) {
    std::vector<std::vector<std::vector<std::vector<int>>>> result;
    result.resize(24);
    std::vector<std::vector<std::vector<int>>> flip_arr = flipXY(arr);
    result = enumerateRotations(flip_arr);
    return result;
}

// 3次元ベクターの同一性を回転と反転まで許して判定
bool isSame(const std::vector<std::vector<std::vector<int>>>& arr1, const std::vector<std::vector<std::vector<int>>>& arr2) {
    std::vector<std::vector<std::vector<std::vector<int>>>> arr1Rotations = enumerateRotations(arr1);
    std::vector<std::vector<std::vector<std::vector<int>>>> arr1Flips = enumerateFlip(arr1);
    std::vector<std::vector<std::vector<std::vector<int>>>> arr2Rotations = enumerateRotations(arr2);
    std::vector<std::vector<std::vector<std::vector<int>>>> arr2Flips = enumerateFlip(arr2);

    std::set<std::vector<std::vector<std::vector<int>>>> arr1Set;
    arr1Set.insert(arr1Rotations.begin(), arr1Rotations.end());
    arr1Set.insert(arr1Flips.begin(), arr1Flips.end());

    std::set<std::vector<std::vector<std::vector<int>>>> arr2Set;
    arr2Set.insert(arr2Rotations.begin(), arr2Rotations.end());
    arr2Set.insert(arr2Flips.begin(), arr2Flips.end());

    return arr1Set == arr2Set;
}

// 関数: 入力された直方体を最小の立方体に整形する
std::vector<std::vector<std::vector<int>>> reshapeToCube(const std::vector<std::vector<std::vector<int>>>& input) {
    // 入力の直方体のサイズを取得
    int xSize = input.size();
    int ySize = 0;
    int zSize = 0;
    if (xSize > 0) {
        ySize = input[0].size();
        if (ySize > 0) {
            zSize = input[0][0].size();
        }
    }

    // 最小の立方体のサイズを計算
    int maxSize = std::max(xSize, std::max(ySize, zSize));

    // 最小の立方体に0パディングして整形する
    std::vector<std::vector<std::vector<int>>> output(maxSize, std::vector<std::vector<int>>(maxSize, std::vector<int>(maxSize, 0)));
    for (int x = 0; x < xSize; x++) {
        for (int y = 0; y < ySize; y++) {
            for (int z = 0; z < zSize; z++) {
                output[x][y][z] = input[x][y][z];
            }
        }
    }

    return output;
}

std::string normalize(const std::vector<std::vector<std::vector<int>>>& arr, int n) {
    std::vector<std::vector<std::vector<std::vector<int>>>> result = enumerateRotations(arr);
    std::vector<std::vector<std::vector<std::vector<int>>>> arrFlips = enumerateFlip(arr);
    result.insert(result.end(), arrFlips.begin(), arrFlips.end());
    std::string ans = "zzz";

    for (int i = 0; i < result.size(); i++)
    {
        std::string ans_t = binaryToDecimal(flatten(result[i]));
        if (ans_t < ans) {
            ans = ans_t;
        }
    }

    std::vector<int> f = decimalToBinary(ans, n * n * n);
    std::vector<std::vector<std::vector<int>>>h = convertTo3DVector(f, n);

    std::vector<std::vector<std::vector<int>>>r = reshapeToCube(h);
    std::string ts = binaryToDecimal(flatten(arr));

    return ts;

}





