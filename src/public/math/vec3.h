#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>
#include "math_com.h"

//释放以下变量到当前作用域
using std::fabs;
using std::sqrt;
/**
 * @brief
 * math vec3
 */
class vec3
{
public:
    double e[3];

public:
    // e{x,x,x}直接赋值e[3]
    vec3() : e{0, 0, 0} {}
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }
    //重载减法运算符，无输入时 例如 a = - vec3; 返回的新vec3
    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; }
    // https://blog.csdn.net/kuaileky/article/details/109715578; & 返回的是地址
    double &operator[](int i) { return e[i]; }

    vec3 &operator+=(const vec3 &v)
    {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3 &operator*=(const double t)
    {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3 &operator/=(const double t)
    {
        return *this *= 1 / t;
    }

    double length() const
    {
        return sqrt(length_squared());
    }

    double length_squared() const
    {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    bool near_zero() const
    {
        // Return true if the vector is close to zero in all dimensions.
        const auto s = 1e-8;
        return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
    }

    inline static vec3 random()
    {
        return vec3(random_double(), random_double(), random_double());
    }
    inline static vec3 random(double min, double max)
    {
        return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }
};
// Type aliases for vec3
using point3 = vec3; // 3D point
using color = vec3;  // RGB color

// vec3 Utility Functions
// https://www.runoob.com/w3cnote/cpp-inline-usage.html
// 定义vec3 的输出
inline std::ostream &operator<<(std::ostream &out, const vec3 &v)
{
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v)
{
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v)
{
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v)
{
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3 &v)
{
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3 &v, double t)
{
    return t * v;
}

inline vec3 operator/(vec3 v, double t)
{
    return (1 / t) * v;
}

inline double dot(const vec3 &u, const vec3 &v)
{
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v)
{
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}
/**
 * @brief 归一化vec3
 *
 * @param v
 * @return vec3
 */
inline vec3 unit_vector(vec3 v)
{
    return v / v.length();
}
/**
 * @brief 随机取单位球体中的一点，球心为(0,0,0)
 *
 * @return vec3
 */
inline vec3 random_in_unit_sphere()
{
    while (true)
    {
        auto p = vec3::random(-1, 1);
        if (p.length_squared() >= 1)
            continue;
        return p;
    }
}
/**
 * @brief 将球体内中的随机取得点进行归一化操作，为拾取单位球体表面上的随机点
 * lambertian reflectance
 * @return vec3
 */
inline vec3 random_unit_vector()
{
    return unit_vector(random_in_unit_sphere());
}

/**
 * @brief 半球采样
 *
 * @param normal
 * @return vec3
 */
inline vec3 random_in_hemisphere(const vec3 &normal)
{
    vec3 sample_sphere = random_in_unit_sphere();
    if (dot(sample_sphere, normal) > 0.0)
    {
        return sample_sphere;
    }
    else
    {
        return -sample_sphere;
    }
}
/**
 * @brief 镜面反射
 *
 * @param v 入射方向
 * @param n 法线方向
 * @return vec3 出射方向
 */
inline vec3 reflect(const vec3 &v, const vec3 &n)
{
    return v - 2 * dot(v, n) * n;
}

/**
 * @brief 折射
 *
 * @param uv
 * @param n
 * @param refract_index 折射率 //折射率计算公式为：n=c/v。这里n为折射率，c为真空光速，v为介质光速。比如按这个公式结算，光在水中的折射率为n=30/22.5≈1.33
 * @return vec3
 */
inline vec3 refract(const vec3 &uv, const vec3 &n, double refract_ratio)
{
    //推导：https://blog.csdn.net/yinhun2012/article/details/79472364
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp = refract_ratio * (uv + cos_theta * n);
    vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

/**
 * @brief 随机单位圆片取点
 *
 * @return vec3
 */
inline vec3 random_in_unit_disk()
{
    while (true)
    {
        auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
        if (p.length_squared() >= 1)
            continue;
        return p;
    }
}

/**余弦密度函数*/
inline vec3 random_cosine_direction()
{
    auto r1 = random_double();
    auto r2 = random_double();
    auto z = sqrt(1 - r2);

    auto phi = 2 * pi * r1;
    auto x = cos(phi) * sqrt(r2);
    auto y = sin(phi) * sqrt(r2);

    return vec3(x, y, z);
}

#endif