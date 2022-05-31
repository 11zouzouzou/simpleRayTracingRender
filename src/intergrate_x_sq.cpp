#include "utils.h"

#include <iostream>
#include <iomanip>
#include <math.h>
#include <stdlib.h>

inline double pdf(double x)
{
    return 3 * x * x / 8;
}

int main()
{
    int N = 1000000;
    auto sum = 0.0;
    for (int i = 0; i < N; i++)
    {
        // auto x = random_double(0, 2);
        // sum += x * x;
        auto x = pow(random_double(0, 8), 1. / 3.);
        sum += x * x / pdf(x);
    }
    std::cout << std::fixed << std::setprecision(12);
    // std::cout << "I = " << 2 * sum / N << '\n';
    std::cout << "I = " << sum / N << '\n';
}