#ifndef SPHERE_H
#define SPHERE_H
#include "hittable.h"
#include "material.h"
#include "vec3.h"
#include "aabb.h"
class sphere : public hittable
{
public:
    point3 center;
    double radius;
    //挂载的材质
    shared_ptr<material> mat_ptr;

public:
    sphere() {}
    sphere(point3 cen, double r, shared_ptr<material> m) : center(cen), radius(r), mat_ptr(m){};
    // https://blog.csdn.net/qq_35516517/article/details/110072421 重写虚函数
    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb &output_box) const override;
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
    auto sqrtd = sqrt(discriminant);
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
    rec.mat_ptr = mat_ptr;
    return true;
}

bool sphere::bounding_box(double time0, double time1, aabb &output_box) const
{
    output_box = aabb(
        center - vec3(radius, radius, radius),
        center + vec3(radius, radius, radius));
    return true;
}

#endif