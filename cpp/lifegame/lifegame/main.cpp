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
#include "rotateAndFlipSearch.h"
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>
#include <numeric>
#include <vector>
#include <cmath>
#include <map>
#include <string>


// 問題のサイズ
int n =6;

//初期状態の濃度(1/2^density)
int density = 2;

// 焼きなまし法のパラメータ
const int maxIterations = 500;      // 最大探索回数
const float initialTemperature = 10000.0f; // 初期温度
const float coolingRate = 0.97f;       // 冷却率      
const float epsilon = 0.1f;            // 反転率（ε）

int count = 500;
std::vector<int> rule = { 2, 5, 10 };

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

std::map<std::string, int> countOccurrences(const std::vector<std::string>& inputVector) {
    std::map<std::string, int> occurrences;

    for (const auto& element : inputVector) {
        occurrences[element]++;
    }

    return occurrences;
}

// 出現回数順にソートするための比較関数
bool sortByOccurrences(const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
    return a.second > b.second;
}

// 三次元vectorの表示
void printVector(const std::vector<std::vector<std::vector<int>>>& cube) {
    for (int i = 0; i < cube.size(); ++i) {
        for (int j = 0; j < cube[0].size(); ++j) {
            for (int k = 0; k < cube[0][0].size(); ++k) {
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

void print1DVector(const std::vector<std::string>& cube) {
    for (int i = 0; i < cube.size(); ++i) {
        auto c = cube[i];
        std::cout << c << std::endl;
    }
}


std::vector<std::string>patternChecker(const std::vector<std::string>result_history) {
    std::vector<std::string> history;

    for (int i = 0; i < result_history.size(); i++)
    {
        std::vector<int> f = decimalToBinary(result_history[i], n * n * n);
        std::vector<std::vector<std::vector<int>>>h = convertTo3DVector(f, n);
        std::vector<std::vector<std::vector<std::vector<int>>>>h_p = collectThreeDimensionalVectors(h);
        for (int j = 0; j < h_p.size(); j++) {
            h_p[j] = removeOuterFaces(h_p[j]);
            std::string nomalized = normalize(h_p[j], n);
            if (nomalized != "1") {
                history.push_back(nomalized);
            }
        }

    }
    return history;
}


// コスト計算関数 (エネルギー)
double energy(const std::vector<std::vector<std::vector<int>>>& cube) {

    double cost;

    std::pair<int, std::vector<std::string>> result = generation(cube, rule, filter, count);
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
    int n_m = 0;
    int c = 0;


    if (l==1)
    {
        m = 500;
    }
    //loop
    cost = -2 * static_cast<double>(result.first)+ static_cast<double>(sum) * 0.3 + pow(m, 2) - pow(l, 3);
    
    //メトセラ
    if (loop_counter(result.second) > 0)
    {
        n_m = 500;
    }
    else
    {
        c = 1;
    }

    cost =n_m - result.first / static_cast<double>(sum);

    std::vector<std::string> history;

    history = patternChecker(result.second);

    std::map<std::string, int> occurrences = countOccurrences(history);
    std::vector<std::pair<std::string, int>> sortedOccurrences;
    for (const auto& entry : occurrences) {
        sortedOccurrences.push_back(entry);
    }
    std::sort(sortedOccurrences.begin(), sortedOccurrences.end(), sortByOccurrences);
    const auto& firstEntry = sortedOccurrences[0];
    const auto& secondEntry = sortedOccurrences[1];

    cost = n_m - c*firstEntry.second*firstEntry.first.size()/3-c*secondEntry.second*secondEntry.first.size()/6;

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
    std::vector<int> input_flat = generateFlatVector(gen,n,density);
    std::vector<std::vector<std::vector<int>>> currentSolution = convertTo3DVector(input_flat, n);

    printVector(currentSolution);

    int estimate_counts=0;
    while (initialTemperature * std::pow(coolingRate, estimate_counts)>0.1)
    {
        estimate_counts += 1;
    }

    // 現在の解のエネルギー
    double currentEnergy = energy(currentSolution);

    long int time = 0;
    

    // 最良解とそのエネルギー
    std::vector<std::vector<std::vector<int>>> bestSolution = currentSolution;
    double bestEnergy = currentEnergy;
    std::chrono::system_clock::time_point estimateStartPoint;
    std::chrono::system_clock::time_point estimatePoint;
    estimateStartPoint = std::chrono::system_clock::now();

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

            if (time%maxIterations==0)
            {
                std::pair<int, std::vector<std::string>> best_result = generation(bestSolution, rule, filter, count);
                std::vector<std::string> history;
                history = patternChecker(best_result.second);

                std::map<std::string, int> occurrences = countOccurrences(history);
                std::vector<std::pair<std::string, int>> sortedOccurrences;
                for (const auto& entry : occurrences) {
                    sortedOccurrences.push_back(entry);
                }
                std::sort(sortedOccurrences.begin(), sortedOccurrences.end(), sortByOccurrences);
                const auto& firstEntry = sortedOccurrences[0];

                double best_energy = energy(bestSolution);

                std::cout << "-----------------------------time:" << time<<"||temperature:"<<temperature << "-----------------------------" << std::endl;

                std::cout << "cost" <<best_energy<<"loop"<< loop_counter(best_result.second)<< std::endl;
                std::cout << "cost" << energy(bestSolution) << "end:" << best_result.first << std::endl;
                std::cout << "most frequency pattern" << firstEntry.first << " : " << firstEntry.second << std::endl;
                std::cout << best_result.second.size() << std::endl;
                //print1DVector(best_result.second);

            }
            if (time==1000)
            {
                estimatePoint = std::chrono::system_clock::now();
                double estimateTime = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(estimatePoint - estimateStartPoint).count() / (std::pow(1000.0, 2)));
                std::cout << estimateTime << std::endl;
                std::cout << (((estimate_counts * maxIterations) - time) / time) * estimateTime << std::endl;
            }
            if (time == maxIterations * 10)
            {
                if (bestEnergy >=0||generation(bestSolution,rule,filter,count).first<50)
                {
                    std::mt19937 gen(rd());
                    std::uniform_int_distribution<> dist_l(0, 25);
                    rule[0] = dist_l(gen);
                    std::uniform_int_distribution<> dist_b(rule[0], 26);
                    rule[1] = dist_b(gen);
                    std::uniform_int_distribution<> dist_o(rule[1], 27);
                    rule[2] = dist_o(gen);
                    std::cout << rule[0] << "," << rule[1] << "," << rule[2] << std::endl;
                    iteration = 0;
                    std::cout << iteration << std::endl;
                    time = 0;
                    temperature = initialTemperature;
                    bestEnergy = pow(100, 3);
                }
            }
        }

        // 温度の冷却
        temperature *= coolingRate;
    }
    std::cout << std::endl;
    std::cout <<"loop:" << loop_counter(generation(bestSolution, rule, filter, count).second) << std::endl;
    std::pair<int, std::vector<std::string>> best_result = generation(bestSolution, rule, filter, count);
    std::cout << best_result.second.size() << std::endl;
    int sum = 0;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                sum += bestSolution[i][j][k];
            }
        }
    }
    for (int i = 0; i < best_result.second.size(); i++)
    {
        std::cout << best_result.second[i] << std::endl;
    }
    return bestSolution;
}

int main() {
    std::chrono::system_clock::time_point start, end;

    start = std::chrono::system_clock::now();
    // 焼きなまし法による探索
    std::vector<std::vector<std::vector<int>>> solution = simulatedAnnealing();

    // 結果の表示
    printVector(solution);

    end = std::chrono::system_clock::now();

    double time = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0);
    printf("time %lf[ms]\n", time);
    return 0;
}


int tes() {
    // 入力データの例
    std::vector<std::vector<std::vector<int>>> input = {
        {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}},
        {{0, 1, 0}, {0, 1, 0}, {0, 1, 0}},
        {{0, 0, 1}, {0, 0, 1}, {0, 0, 1}}
    };

    // カーネル（フィルター）の例
    std::vector<std::vector<std::vector<int>>> filter = {
        {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}},
        {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}},
        {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}}
    };

    // 3D畳み込みを実行
    std::vector<std::vector<std::vector<int>>> output = conv3D(input, filter);

    // 結果の出力
    std::cout << "Output:" << std::endl;
    for (const auto& slice : output) {
        for (const auto& row : slice) {
            for (int value : row) {
                std::cout << value << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    return 0;
}


