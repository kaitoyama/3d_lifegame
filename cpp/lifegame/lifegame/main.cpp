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


// ���̃T�C�Y
int n =6;

//������Ԃ̔Z�x(1/2^density)
int density = 2;

// �Ă��Ȃ܂��@�̃p�����[�^
const int maxIterations = 500;      // �ő�T����
const float initialTemperature = 10000.0f; // �������x
const float coolingRate = 0.97f;       // ��p��      
const float epsilon = 0.1f;            // ���]���i�Áj

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

// �o���񐔏��Ƀ\�[�g���邽�߂̔�r�֐�
bool sortByOccurrences(const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
    return a.second > b.second;
}

// �O����vector�̕\��
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


// �R�X�g�v�Z�֐� (�G�l���M�[)
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
    
    //���g�Z��
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

// ���x�Ɋ�Â��đJ�ڂ���m�����v�Z
double acceptanceProbability(double currentEnergy, double newEnergy, float temperature) {
    // �G�l���M�[��������ꍇ�͏�ɑJ�ڂ���
    if (newEnergy < currentEnergy) {
        return 1.0;
    }

    // ���x�Ɋ�Â��đJ�ڂ���m�����v�Z
    return exp((currentEnergy - newEnergy) / temperature);
}

// �����_���ȗv�f�̔��]
std::vector<std::vector<std::vector<int>>> flipElement(std::vector<std::vector<std::vector<int>>>& cube, float epsilon) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                if (dis(gen) < epsilon) {
                    cube[i][j][k] = 1 - cube[i][j][k];  // 0��1�𔽓]
                }
            }
        }
    }
    return cube;
}

// �Ă��Ȃ܂��@�ɂ��T��
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

    // ���݂̉��̃G�l���M�[
    double currentEnergy = energy(currentSolution);

    long int time = 0;
    

    // �ŗǉ��Ƃ��̃G�l���M�[
    std::vector<std::vector<std::vector<int>>> bestSolution = currentSolution;
    double bestEnergy = currentEnergy;
    std::chrono::system_clock::time_point estimateStartPoint;
    std::chrono::system_clock::time_point estimatePoint;
    estimateStartPoint = std::chrono::system_clock::now();

    // �Ă��Ȃ܂��@�̃��C�����[�v
    float temperature = initialTemperature;
    while (temperature>0.1) {
        std::vector<std::vector<std::vector<int>>> newSolution = currentSolution;
        for (int iteration = 0; iteration < maxIterations; ++iteration) {
            time += 1;
            // �����_���ȗv�f�̔��]
            newSolution = flipElement(newSolution, epsilon);
            // �V�������̃G�l���M�[
            double newEnergy = energy(newSolution);
            // �V���������̗p����邩����
            if (acceptanceProbability(currentEnergy, newEnergy, temperature) > dis(gen)) {
                currentEnergy = newEnergy;
                // �ŗǉ��̍X�V
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

        // ���x�̗�p
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
    // �Ă��Ȃ܂��@�ɂ��T��
    std::vector<std::vector<std::vector<int>>> solution = simulatedAnnealing();

    // ���ʂ̕\��
    printVector(solution);

    end = std::chrono::system_clock::now();

    double time = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0);
    printf("time %lf[ms]\n", time);
    return 0;
}


int tes() {
    // ���̓f�[�^�̗�
    std::vector<std::vector<std::vector<int>>> input = {
        {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}},
        {{0, 1, 0}, {0, 1, 0}, {0, 1, 0}},
        {{0, 0, 1}, {0, 0, 1}, {0, 0, 1}}
    };

    // �J�[�l���i�t�B���^�[�j�̗�
    std::vector<std::vector<std::vector<int>>> filter = {
        {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}},
        {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}},
        {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}}
    };

    // 3D��ݍ��݂����s
    std::vector<std::vector<std::vector<int>>> output = conv3D(input, filter);

    // ���ʂ̏o��
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


