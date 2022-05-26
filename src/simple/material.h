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

/**
 * @brief lambertian材质
 *
 */
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
/**
 * @brief 金属散射材质
 *
 */
class metal_material : public material
{
public:
    color albedo;
    /**反射方向随机扰动影响(扰动范围为半径为1的球体)*/
    double fuzz;

public:
    metal_material(const color &a, double f) : albedo(a), fuzz(f < 1 ? f : 1){};
    virtual bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override
    {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }
};
/**
 * @brief 电介质材质
 *
 */
class dielectric_material : public material
{
public:
    dielectric_material(double index_of_refraction) : ir(index_of_refraction) {}

    virtual bool scatter(
        const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override
    {
        attenuation = color(1.0, 1.0, 1.0);
        double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;
        //，总是折射
        vec3 unit_direction = unit_vector(r_in.direction());
        vec3 refracted = refract(unit_direction, rec.normal, refraction_ratio);

        scattered = ray(rec.p, refracted);
        return true;
    }

public:
    double ir; // Index of Refraction
};

#endif