#ifndef HITTABLE_H
#define HITTABLE_H

#include "utils.h"
#include "aabb.h"

class material;
struct hit_record
{
    /**交点*/
    point3 p;
    /**法线方向(归一化)*/
    vec3 normal;
    /**当前所碰撞的材质*/
    shared_ptr<material> mat_ptr;
    /**x坐标*/
    double t;
    /**根据当前射线方向与法线朝向决定是背面还是正面*/
    bool front_face;
    //命中点的uv 表面坐标
    double u;
    double v;

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
    virtual bool bounding_box(double time0, double time1, aabb &output_box) const = 0; // time0,time1为有可能移动的物体
};

#endif