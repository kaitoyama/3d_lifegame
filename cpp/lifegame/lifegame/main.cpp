#include <iostream>
#include "generation.h"
#include "convertTo3DVector.h"
#include "generateFlatVector.h"
#include <random>
#include <numeric>

int main()
{
    int n;
    std::cin >> n;

    std::mt19937 mt{ std::random_device{}() };

    int maxValue = static_cast<int>(std::pow(2, n * n * n)) - 1;

    std::cout << maxValue << std::endl;

    std::uniform_int_distribution<int> dist(0, maxValue);

    std::cout << dist(mt) << std::endl;

    std::vector<int> input_float = generateFlatVector(dist(mt),n);

    int sum = accumulate(input_float.begin(), input_float.end(), 0);

    std::cout << sum << std::endl;

    // 入力データの作成
    std::vector<std::vector<std::vector<int>>> input_vector = convertTo3DVector(input_float,n);

    std::vector<int> rule = { 0, 4, 6 };

    std::vector<std::vector<std::vector<int>>> filter = {
        {{1, 1, 1}, {1, 0, 1}, {1, 1, 1}},
        {{1, 0, 1}, {0, 0, 0}, {1, 0, 1}},
        {{1, 1, 1}, {1, 0, 1}, {1, 1, 1}}
    };

    int count = 5;

    // ゲームの実行
    std::pair<int, std::vector<int>> result = generation(input_vector, rule, filter, count);
    for (size_t i = 0; i <10000 ; i++)
    {
        generation(input_vector, rule, filter, count);
    }

    // 結果の表示
    std::cout << "End: " << result.first << std::endl;
    std::cout << "Proc: ";
    for (int i = 0; i < result.second.size(); ++i) {
        std::cout << result.second[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}

