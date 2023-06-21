#include <iostream>
#include "generation.h"
#include "flatten.h"
#include "loop_counter.h"
#include "convertTo3DVector.h"
#include "generateFlatVector.h"
#include "binaryToDecimal.h"
#include "decimalToBinary.h"
#include "stringToBight.h"
#include "conv3D.h"
#include <algorithm>
#include <random>
#include <chrono>
#include <numeric>
#include <vector>
#include <cmath>
#include <string>


// 問題のサイズ
int n = 4;

// 焼きなまし法のパラメータ
const int maxIterations = 100;      // 最大探索回数
const float initialTemperature = 5000; // 初期温度
const float coolingRate = 0.95f;       // 冷却率      
const float epsilon = 0.1f;            // 反転率（ε）

int count = 100;
std::vector<int> rule = { 0, 4, 6 };

std::vector<std::vector<std::vector<int>>> filter = {
    {{1, 1, 1},
    {1, 1, 1},
    {1, 1, 1}},

    {{1, 1, 1},
    {1, 0, 1},
    {1, 1, 1}},

    {{1, 1, 1},
    {1, 1, 1},
    {1, 1, 1}}
};

// 三次元vectorの表示
void printVector(const std::vector<std::vector<std::vector<int>>>& cube) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                std::cout << cube[i][j][k] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

void print2DVector(const std::vector<std::vector<std::string>>& cube) {
    for (int i = 0; i < cube.size(); ++i) {
        for (int j = 0; j < cube[i].size(); ++j) {
                auto c = cube[i][j];
                std::cout << c ;
        }
        std::cout << std::endl;
    }
}

// コスト計算関数 (エネルギー)
double energy(const std::vector<std::vector<std::vector<int>>>& cube) {

    double cost;

    std::pair<int, std::vector<std::vector<std::string>>> result = generation(cube, rule, filter, count);
    int l = loop_counter(result.second);
    int sum = 0;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                sum += cube[i][j][k];
            }
        }
    }

    int m = 0;


    if (l==1)
    {
        m = 500;
    }

    cost = -2 * static_cast<double>(result.first)+ static_cast<double>(sum) * 0.3 + pow(m, 3) - pow(l, 3);
    
    return cost;
}

// 温度に基づいて遷移する確率を計算
double acceptanceProbability(double currentEnergy, double newEnergy, float temperature) {
    // エネルギーが下がる場合は常に遷移する
    if (newEnergy < currentEnergy) {
        return 1.0;
    }

    // 温度に基づいて遷移する確率を計算
    return exp((currentEnergy - newEnergy) / temperature);
}

// ランダムな要素の反転
std::vector<std::vector<std::vector<int>>> flipElement(std::vector<std::vector<std::vector<int>>>& cube, float epsilon) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                if (dis(gen) < epsilon) {
                    cube[i][j][k] = 1 - cube[i][j][k];  // 0と1を反転
                }
            }
        }
    }
    return cube;
}

// 焼きなまし法による探索
std::vector<std::vector<std::vector<int>>> simulatedAnnealing() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    std::uniform_int_distribution<int> dist(0, 1);
    std::vector<int> input_float = generateFlatVector(gen,n);
    // 入力データの作成
    std::vector<std::vector<std::vector<int>>> currentSolution = convertTo3DVector(input_float, n);

    printVector(currentSolution);

    // 現在の解のエネルギー
    double currentEnergy = energy(currentSolution);

    long int time = 0;
    

    // 最良解とそのエネルギー
    std::vector<std::vector<std::vector<int>>> bestSolution = currentSolution;
    double bestEnergy = currentEnergy;

    // 焼きなまし法のメインループ
    float temperature = initialTemperature;
    while (temperature>0.1) {
        std::vector<std::vector<std::vector<int>>> newSolution = currentSolution;
        for (int iteration = 0; iteration < maxIterations; ++iteration) {
            time += 1;
            // ランダムな要素の反転
            newSolution = flipElement(newSolution, epsilon);
            // 新しい解のエネルギー
            double newEnergy = energy(newSolution);
            // 新しい解が採用されるか判定
            if (acceptanceProbability(currentEnergy, newEnergy, temperature) > dis(gen)) {
                currentEnergy = newEnergy;
                // 最良解の更新
                if (newEnergy < bestEnergy) {
                    bestSolution = newSolution;
                    bestEnergy = newEnergy;
                }
            }
            if (time%1000==0)
            {
                std::cout << "-----------------------------" << std::endl;
                std::cout << "cost" << energy(bestSolution);
                std::pair<int, std::vector<std::vector<std::string>>> best_result = generation(bestSolution, rule, filter, 100);
                std::cout << best_result.second.size() << std::endl;
                print2DVector(best_result.second);

            }
        }

        // 温度の冷却
        temperature *= coolingRate;
    }
    std::cout << bestEnergy << std::endl;
    for (int i = 0; i < generation(bestSolution, rule, filter, count).second.size(); i++) {
        for (int j = 0; j < generation(bestSolution, rule, filter, count).second.at(i).size();j++) {
            auto l = generation(bestSolution, rule, filter, count).second.at(i).at(j);
            std::cout << j << "";

        }
        std::cout<<std::endl;
    }
    std::cout << std::endl;
    std::cout <<"loop:" << loop_counter(generation(bestSolution, rule, filter, count).second) << std::endl;
    return bestSolution;
}

int main() {
    // 焼きなまし法による探索
    std::vector<std::vector<std::vector<int>>> solution = simulatedAnnealing();

    // 結果の表示
    printVector(solution);

    return 0;
}


int test()
{
    n=2;

    std::mt19937 mt{ std::random_device{}() };

    int maxValue = static_cast<int>(std::pow(2, n * n * n)) - 1;

    std::cout << maxValue << std::endl;

    std::uniform_int_distribution<int> dist(0, maxValue);

    std::cout << dist(mt) << std::endl;

    std::string S = "255";

    std::cout << S << std::endl;

    std::vector<int> S_l = string_to_bigint(S);
    
    std::reverse(S_l.begin(), S_l.end());
    for (size_t i = 0; i < S_l.size(); i++)
    {
        std::cout << "size:" << S_l.at(i) << "";
    }
    std::cout << std::endl;

    std::vector<int> input_float = decimalToBinary(S_l);
    for (size_t i = 0; i < input_float.size(); i++)
    {
        std::cout << input_float.at(i) << "";
    }
    std::cout << std::endl;

    auto it = input_float.begin();

    int size_diff = std::pow(n, 3) - input_float.size();

    for (size_t i = 0; i <size_diff ; i++)
    {
        it = input_float.insert(it, 0);

    }

    for (size_t i = 0; i < input_float.size(); i++)
    {
        std::cout << input_float.at(i) <<"";
    }
    std::cout<<std::endl;
    std::cout << input_float.size() << std::endl;

    int sum = accumulate(input_float.begin(), input_float.end(), 0);

    std::cout << sum << std::endl;

    // 入力データの作成
    std::vector<std::vector<std::vector<int>>> input_vector = convertTo3DVector(input_float,n);
    printVector(input_vector);
    std::vector<int> rule = { 0, 0, 27 };

    std::vector<std::vector<std::vector<int>>> filter = {
        {{1, 1, 1}, 
        {1, 1, 1}, 
        {1, 1, 1}},

        {{1, 1, 1}, 
        {1, 0, 1}, 
        {1, 1, 1}},

        {{1, 1, 1}, 
        {1, 1, 1}, 
        {1, 1, 1}}
    };

    int count = 500;

    std::chrono::system_clock::time_point start, end;

    start = std::chrono::system_clock::now();
    // ゲームの実行
    std::pair<int, std::vector<std::vector <std::string>>> result = generation(input_vector, rule, filter, count);

    end = std::chrono::system_clock::now();

    double time = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0);
    printf("time %lf[ms]\n", time);


    // 結果の表示
    std::cout << "End: " << result.first << std::endl;
    std::cout << "Proc: ";
    print2DVector(result.second);
    std::cout << std::endl;

    printVector(conv3D(input_vector, filter));

    return 0;
}

