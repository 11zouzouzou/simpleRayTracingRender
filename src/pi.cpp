#include "utils.h"

#include <iostream>
#include <iomanip> //格式化输出：https://blog.csdn.net/Lina_ACM/article/details/51586223
#include <math.h>
#include <stdlib.h> //https://www.runoob.com/cprogramming/c-standard-library-stdlib-h.html

int main()
{
    int runs = 0;
    int inside_circle = 0;
    std::cout << std::fixed << std::setprecision(12); //详细展示数字，精确至12位
    while (true)
    {
        runs++;
        auto x = random_double(-1, 1);
        auto y = random_double(-1, 1);
        if (x * x + y * y < 1)
            inside_circle++;

        if (runs % 100000 == 0)
            std::cout << "Estimate of Pi = "
                      << 4 * double(inside_circle) / runs
                      << '\n';
    }
}