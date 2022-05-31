#include "utils.h"

#include <iostream>
#include <iomanip>
#include <math.h>
#include <stdlib.h>

int main()
{
    /**随机球面采样*/
    for (int i = 0; i < 200; i++)
    {
        auto r1 = random_double();
        auto r2 = random_double();
        auto x = cos(2 * pi * r1) * 2 * sqrt(r2 * (1 - r2));
        auto y = sin(2 * pi * r1) * 2 * sqrt(r2 * (1 - r2));
        auto z = 1 - 2 * r2;
        std::cout << x << " " << y << " " << z << '\n';
    }
}