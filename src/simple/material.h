#ifndef MATERIAL_H
#define MATERIAL_H

#include "utils.h"

struct hit_record;

class material
{
private:
    /* data */
public:
    virtual bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const = 0;
};

class lambertian_material : public material
{
public:
    /**反照率,可以理解为反射的颜色值*/
    color albedo;

public:
    lambertian_material(const color &a) : albedo(a) {}
    virtual bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override
    {
        vec3 scatter_direction = rec.normal + random_unit_vector(); //法线方向的偏移
        if (scatter_direction.near_zero())
        {
            scatter_direction = rec.normal; //若正好与方向方向相反，直接使用法线方向
        }

        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }
};
#endif