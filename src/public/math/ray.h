#ifndef RAY_H
#define RAY_H

#include "vec3.h"
class ray
{
public:
    /* data */
    point3 orig;
    vec3 dir;
    //这条光线存在的时间
    double tm;

public:
    ray(/* args */){};
    ray(const point3 &origin, const vec3 &direction, double
        : time = 0.0) : orig(origin), dir(direction), tm(time){};
    // https://blog.csdn.net/SMF0504/article/details/52311207 后面加 const表示函数不可以修改class的成员
    point3 origin() const { return orig; }
    vec3 direction() const { return dir; }
    double time() const { return tm; }

    point3 at(double t) const
    {
        return orig + t * dir;
    }
};

#endif