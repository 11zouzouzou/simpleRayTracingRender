#include "utils.h"

#include <iostream>
#include <iomanip> //格式化输出：https://blog.csdn.net/Lina_ACM/article/details/51586223
#include <math.h>
#include <stdlib.h> //https://www.runoob.com/cprogramming/c-standard-library-stdlib-h.html

int main()
{
    //每个样本有效收益将会递减
    //  int runs = 0;
    // int inside_circle = 0;
    // std::cout << std::fixed << std::setprecision(12); //详细展示数字，精确至12位
    // while (true)
    // {
    //     runs++;
    //     auto x = random_double(-1, 1);
    //     auto y = random_double(-1, 1);
    //     if (x * x + y * y < 1)
    //         inside_circle++;

    //     if (runs % 100000 == 0)
    //         std::cout << "Estimate of Pi = "
    //                   << 4 * double(inside_circle) / runs
    //                   << '\n';
    // }
    //分层处理将[-1,1]正方形划格子(10000*10000的格子)采样
    int inside_circle = 0;
    int inside_circle_stratified = 0;
    int sqrt_N = 10000;
    for (int i = 0; i < sqrt_N; i++)
    {
        for (int j = 0; j < sqrt_N; j++)
        {
            auto x = random_double(-1, 1);
            auto y = random_double(-1, 1);
            if (x * x + y * y < 1)
            {
                inside_circle++;
            }
            x = 2 * ((i + random_double()) / sqrt_N) - 1;
            y = 2 * ((j + random_double()) / sqrt_N) - 1;
            if (x * x + y * y < 1)
            {
                inside_circle_stratified++;
            }
        }
    }
    auto N = static_cast<double>(sqrt_N) * sqrt_N;
    std::cout << std::fixed << std::setprecision(12);
    std::cout
        << "Regular estimate of pi ="
        << 4 * double(inside_circle) / N << "\n"
        << "Stratified estimate of pi ="
        << 4 * double(inside_circle_stratified) / N << "\n";
}