#include "utils.h"

#include <iostream>
#include <iomanip> //格式化输出：https://blog.csdn.net/Lina_ACM/article/details/51586223
#include <math.h>
#include <stdlib.h> //https://www.runoob.com/cprogramming/c-standard-library-stdlib-h.html

int main()
{
    int N = 1000;
    int inside_circle = 0;
    for (int i = 0; i < N; i++)
    {
        auto x = random_double(-1,1);
        auto y = random_double(-1,1);
        if (x*x + y*y < 1)
        {
            inside_circle++;
        }
    }

    // std::cout << std::fixed <<
    
}