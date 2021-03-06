#ifndef MATERIAL_H
#define MATERIAL_H

#include "utils.h"
#include "texture.h"
#include "onb.h"

struct hit_record;

class material
{
public:
    virtual bool scatter(
        const ray &r_in, const hit_record &rec, color &albedo, ray &scattered) const
    {
        return false;
    }

    virtual bool scatter(
        const ray &r_in, const hit_record &rec, color &albedo, ray &scattered, double &pdf) const
    {
        return false;
    }

    virtual double scattering_pdf(
        const ray &r_in, const hit_record &rec, const ray &scattered) const
    {
        return 0;
    }

    virtual color emitted(double u, double v, const point3 &p) const
    {
        return color(0, 0, 0);
    }
};

/**
 * @brief lambertian材质
 *
 */
class lambertian_material : public material
{
public:
    lambertian_material(const color &a) : albedo(make_shared<solid_color>(a)) {}
    lambertian_material(shared_ptr<texture> a) : albedo(a) {}

    virtual bool scatter(
        const ray &r_in, const hit_record &rec, color &alb, ray &scattered, double &pdf) const override
    {
        //随机-1,1采样
        // auto scatter_direction = rec.normal + random_unit_vector();

        // Catch degenerate scatter direction
        // if (scatter_direction.near_zero())
        //     scatter_direction = rec.normal;
        //半球采样
        // auto scatter_direction = random_in_hemisphere(rec.normal);
        //正交基//https://zhuanlan.zhihu.com/p/351071035
        onb uvw;
        uvw.build_from_w(rec.normal);
        auto scatter_direction = uvw.local(random_cosine_direction());
        scattered = ray(rec.p, unit_vector(scatter_direction), r_in.time());
        alb = albedo->value(rec.u, rec.v, rec.p);
        // pdf = dot(rec.normal, scattered.direction()) / pi; // mc
        // pdf = 0.5 / pi;
        pdf = dot(uvw.w(), scattered.direction()) / pi;
        return true;
    }
    double scattering_pdf(
        const ray &r_in, const hit_record &rec, const ray &scattered) const override
    {
        auto cosine = dot(rec.normal, unit_vector(scattered.direction()));
        return cosine < 0 ? 0 : cosine / pi;
    }

public:
    shared_ptr<texture> albedo;
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
        scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere(), r_in.time());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }
};
/**
 * @brief 电介质材质(总发生折射)
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
        vec3 unit_direction = unit_vector(r_in.direction());
        // a sina = b sinb 斯涅耳等式判断
        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0; //默认从空气射入

        vec3 direction;

        // reflectance为光被反射的概率
        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
            direction = reflect(unit_direction, rec.normal); //通常是在固体物体内部
        else
            direction = refract(unit_direction, rec.normal, refraction_ratio);

        scattered = ray(rec.p, direction, r_in.time());
        return true;
    }

public:
    double ir; // Index of Refraction

private:
    static double reflectance(double cosine, double ref_idx)
    {
        // Use Schlick's approximation for reflectance.
        // https://zhuanlan.zhihu.com/p/372110183
        // https://zhuanlan.zhihu.com/p/96498097
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};

class diffuse_light : public material
{
public:
    shared_ptr<texture> emit;

public:
    diffuse_light(shared_ptr<texture> a) : emit(a) {}
    diffuse_light(color c) : emit(make_shared<solid_color>(c)) {}
    virtual bool scatter(
        const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override
    {
        //无散射or 反射
        return false;
    }
    virtual color emitted(double u, double v, const point3 &p) const override
    {
        return emit->value(u, v, p);
    }
};

class isotropic : public material
{
public:
    shared_ptr<texture> albedo;

public:
    isotropic(color c) : albedo(make_shared<solid_color>(c)) {}
    isotropic(shared_ptr<texture> a) : albedo(a) {}
    virtual bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override
    {
        /**如果当前ray在体中，但是新的投射中雾没有被击中，则又会去击中其他物体，同时雾的颜色也会作为attenuation乘积到最后的结果中。*/
        scattered = ray(rec.p, random_in_unit_sphere(), r_in.time());
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }
};

#endif