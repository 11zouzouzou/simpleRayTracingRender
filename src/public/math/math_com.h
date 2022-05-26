#ifndef MATH_COM_H
#define MATH_COM_H
#include <cmath>
#include <memory>//shared_ptr make_shared
#include <limits>//numeric_limits
#include <cstdlib>//rand()

// usings

using std::make_shared;
using std::shared_ptr;
using std::sqrt;
// constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees)
{
    return degrees * pi / 180.0;
}

inline double random_double()
{
    //[0,1)
    return rand() / (RAND_MAX + 1.0);
}

// #include <random>

// inline double random_double() {
//     static std::uniform_real_distribution<double> distribution(0.0, 1.0);
//     static std::mt19937 generator;
//     return distribution(generator);
// }

inline double random_double(double min, double max)
{
    return min + (max - min) * random_double();
}

inline double clamp(double x, double min, double max)
{
    if (x < min)
        return min;
    if (x > max)
        return max;
    return x;
}
#endif