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


// ���̃T�C�Y
int n = 4;

// �Ă��Ȃ܂��@�̃p�����[�^
const int maxIterations = 100;      // �ő�T����
const float initialTemperature = 5000; // �������x
const float coolingRate = 0.95f;       // ��p��      
const float epsilon = 0.1f;            // ���]���i�Áj

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

// �O����vector�̕\��
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

// �R�X�g�v�Z�֐� (�G�l���M�[)
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
    std::vector<int> input_float = generateFlatVector(gen,n);
    // ���̓f�[�^�̍쐬
    std::vector<std::vector<std::vector<int>>> currentSolution = convertTo3DVector(input_float, n);

    printVector(currentSolution);

    // ���݂̉��̃G�l���M�[
    double currentEnergy = energy(currentSolution);

    long int time = 0;
    

    // �ŗǉ��Ƃ��̃G�l���M�[
    std::vector<std::vector<std::vector<int>>> bestSolution = currentSolution;
    double bestEnergy = currentEnergy;

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
            if (time%1000==0)
            {
                std::cout << "-----------------------------" << std::endl;
                std::cout << "cost" << energy(bestSolution);
                std::pair<int, std::vector<std::vector<std::string>>> best_result = generation(bestSolution, rule, filter, 100);
                std::cout << best_result.second.size() << std::endl;
                print2DVector(best_result.second);

            }
        }

        // ���x�̗�p
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
    // �Ă��Ȃ܂��@�ɂ��T��
    std::vector<std::vector<std::vector<int>>> solution = simulatedAnnealing();

    // ���ʂ̕\��
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

    // ���̓f�[�^�̍쐬
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
    // �Q�[���̎��s
    std::pair<int, std::vector<std::vector <std::string>>> result = generation(input_vector, rule, filter, count);

    end = std::chrono::system_clock::now();

    double time = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0);
    printf("time %lf[ms]\n", time);


    // ���ʂ̕\��
    std::cout << "End: " << result.first << std::endl;
    std::cout << "Proc: ";
    print2DVector(result.second);
    std::cout << std::endl;

    printVector(conv3D(input_vector, filter));

    return 0;
}

