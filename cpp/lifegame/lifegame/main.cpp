#include <iostream>
#include "generation.h"

int main()
{
    // ���̓f�[�^�̍쐬
    std::vector<std::vector<std::vector<int>>> first = {
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 1}, {0, 1, 1}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 1}, {0, 0, 0}}
    };

    std::vector<int> rule = { 0, 4, 6 };

    std::vector<std::vector<std::vector<int>>> filter = {
        {{1, 1, 1}, {1, 0, 1}, {1, 1, 1}},
        {{1, 0, 1}, {0, 0, 0}, {1, 0, 1}},
        {{1, 1, 1}, {1, 0, 1}, {1, 1, 1}}
    };

    int count = 5;

    // �Q�[���̎��s
    std::pair<int, std::vector<int>> result = generation(first, rule, filter, count);

    // ���ʂ̕\��
    std::cout << "End: " << result.first << std::endl;
    std::cout << "Proc: ";
    for (int i = 0; i < result.second.size(); ++i) {
        std::cout << result.second[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}

