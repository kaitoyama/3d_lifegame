#include <vector>
#include <cmath>
#include <fftw3.h>

std::vector<std::vector<std::vector<int>>> conv3D(const std::vector<std::vector<std::vector<int>>>& input, const std::vector<std::vector<std::vector<int>>>& filter) {
    // 入力データの形状を取得
    int input_depth = input.size();
    int input_height = input[0].size();
    int input_width = input[0][0].size();

    // カーネルの形状を取得
    int filter_depth = filter.size();
    int filter_height = filter[0].size();
    int filter_width = filter[0][0].size();

    // 出力データの形状を計算
    int output_depth = input_depth + filter_depth - 1;
    int output_height = input_height + filter_height - 1;
    int output_width = input_width + filter_width - 1;

    // 1次元配列にフラット化
    std::vector<double> flat_input(output_depth * output_height * output_width);
    std::vector<double> flat_filter(output_depth * output_height * output_width);

    int index = 0;
    for (int z = 0; z < output_depth; ++z) {
        for (int y = 0; y < output_height; ++y) {
            for (int x = 0; x < output_width; ++x) {
                if (z <= (output_depth - filter_depth) && y <= (output_height - filter_height) && x <= (output_width - filter_width))
                {
                    flat_input[index] = static_cast<double>(input[z][y][x]);
                }
                if (z < filter_depth && y < filter_height && x < filter_width)
                {
                    flat_filter[index] = static_cast<double>(filter[z][y][x]);
                }
                ++index;
            }
        }
    }

    // FFTを実行
// メモリ割り当て
    fftw_complex* input_freq = reinterpret_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex) * (output_depth * output_height * output_width)));
    fftw_complex* filter_freq = reinterpret_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex) * (output_depth * output_height * output_width)));

    // データのコピー
    for (int i = 0; i < output_depth * output_height * output_width; ++i) {
        input_freq[i][0] = flat_input[i];
        input_freq[i][1] = 0.0;
        filter_freq[i][0] = flat_filter[i];
        filter_freq[i][1] = 0.0;
    }


    fftw_plan input_plan = fftw_plan_dft_3d(output_depth, output_height, output_width, input_freq, input_freq, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_plan filter_plan = fftw_plan_dft_3d(output_depth, output_height, output_width, filter_freq, filter_freq, FFTW_FORWARD, FFTW_ESTIMATE);

    fftw_execute(input_plan);
    fftw_execute(filter_plan);

    // 周波数領域での要素ごとの積を計算
    for (int i = 0; i < output_depth * output_height * output_width; ++i) {
        double real = input_freq[i][0] * filter_freq[i][0] - input_freq[i][1] * filter_freq[i][1];
        double imag = input_freq[i][0] * filter_freq[i][1] + input_freq[i][1] * filter_freq[i][0];
        input_freq[i][0] = real;
        input_freq[i][1] = imag;
    }

    // 逆FFTを実行
    fftw_plan output_plan = fftw_plan_dft_3d(output_depth, output_height, output_width, input_freq, input_freq, FFTW_BACKWARD, FFTW_ESTIMATE);
    fftw_execute(output_plan);

    // 逆変換結果を元の形状に戻す
    std::vector<std::vector<std::vector<int>>> output_trim(output_depth - filter_depth + 1, std::vector<std::vector<int>>(output_height - filter_height + 1, std::vector<int>(output_width - filter_width + 1)));

    for (int z = 0; z < output_depth - filter_depth + 1; ++z) {
        for (int y = 0; y < output_height - filter_height + 1; ++y) {
            for (int x = 0; x < output_width - filter_width + 1; ++x) {
                int input_z = z + 1;
                int input_y = y + 1;
                int input_x = x + 1;
                output_trim[z][y][x] = static_cast<int>(std::round(input_freq[(input_z * output_height * output_width) + (input_y * output_width) + input_x][0] / (output_depth * output_height * output_width)));
            }
        }
    }


    // メモリの解放
    fftw_destroy_plan(input_plan);
    fftw_destroy_plan(filter_plan);
    fftw_destroy_plan(output_plan);
    fftw_free(input_freq);
    fftw_free(filter_freq);

    return output_trim;
}
