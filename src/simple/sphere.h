#ifndef SPHERE_H
#define SPHERE_H
#include "hittable.h"
#include "vec3.h"
class sphere : public hittable
{
public:
    point3 center;
    double radius;

public:
    sphere() {}
    sphere(point3 cen, double r) center(cen), radius(r){};
    // https://blog.csdn.net/qq_35516517/article/details/110072421 重写虚函数
    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;
};

bool sphere::hit(const ray &r, double t_min, double t_max, hit_record &rec) const
{
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;

    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0)
        return false;
    //在可接受的范围内找出最近的根。
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root)
    {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 out_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, out_normal);
    return true;
}

#endif