#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

struct hit_record
{
    /**交点*/
    point3 p;
    /**法线方向(归一化)*/
    vec3 normal;
    /**x坐标*/
    double t;
    /**根据当前射线方向与法线朝向决定是背面还是正面*/
    bool front_face;

    inline void set_face_normal(const ray &r, const vec3 &outward_normal)
    {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable
{
public:
    // https://www.runoob.com/w3cnote/cpp-virtual-functions.html 纯虚函数
    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const = 0;
};

#endif