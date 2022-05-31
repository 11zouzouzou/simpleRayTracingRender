#ifndef CONSTANT_MEDIUM_H
#define CONSTANT_MEDIUM_H
#include "utils.h"

#include "hittable.h"
#include "material.h"
#include "texture.h"

class constant_medium : public hittable
{
public:
    shared_ptr<hittable> boundary;
    shared_ptr<material> phase_function;
    double neg_inv_density;

public:
    constant_medium(shared_ptr<hittable> b, double d, shared_ptr<texture> a) : boundary(b), neg_inv_density(-1 / d), phase_function(make_shared<isotropic>(a)){};
    constant_medium(shared_ptr<hittable> b, double d, color c)
        : boundary(b),
          neg_inv_density(-1 / d),
          phase_function(make_shared<isotropic>(c))
    {
    }

    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb &output_box) const override
    {
        return boundary->bounding_box(time0, time1, output_box);
    }
};

bool constant_medium::hit(const ray &r, double t_min, double t_max, hit_record &rec) const
{
    // https://blog.csdn.net/tiao_god/article/details/106723303
    const bool enableDebug = false;
    const bool debugging = enableDebug && random_double() < 0.00001;

    /**
     * 我们必须对边界周围的逻辑如此小心的原因是我们需要确保这适用于体积内的射线源。在云中，事物经常反弹，所以这是一种常见的情况。
     * 此外，上面的代码假设，一旦光线离开恒定介质边界，它将永远在边界外继续。换句话说，它假设边界形状是凸的。所以这个特定的实现将适用于像盒子或球体这样的边界，但不适用于包含空隙的环面或形状。
     */
    hit_record rec1, rec2;

    /**
     * 两次判断，因为你不能确定这玩意是向哪边散射的（参考之前的isotropic类，散射方向是球面，任意方向），这两次判断要保证散射方向是在体空间内传播的。
     */
    if (!boundary->hit(r, -infinity, infinity, rec1))
    {
        return false;
    }

    if (!boundary->hit(r, rec1.t + 0.00001, infinity, rec2))
    {
        return false;
    }
    if (debugging)
        std::cerr << "\nt_min=" << rec1.t << ", t_max=" << rec2.t << '\n';

    if (rec1.t < t_min)
        rec1.t = t_min;
    if (rec2.t > t_max)
        rec2.t = t_max;

    if (rec1.t >= rec2.t)
        return false;

    if (rec1.t < 0)
        rec1.t = 0;

    const auto ray_length = r.direction().length();
    const auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length; //光线在体空间内能够传输的距离（光线方向上体空间的长度）
    const auto hit_distance = neg_inv_density * log(random_double());     //随机数生成一个此光线当前散射传输的距离

    if (hit_distance > distance_inside_boundary)
        return false; //如果光线直接传出去，那就算为采样到头了，返回false。如果传不出去，就记录体空间内散射的信息。
    rec.t = rec1.t + hit_distance / ray_length;
    rec.p = r.at(rec.t);
    if (debugging)
    {
        std::cerr << "hit_distance = " << hit_distance << '\n'
                  << "rec.t = " << rec.t << '\n'
                  << "rec.p = " << rec.p << '\n';
    }

    rec.normal = vec3(1, 0, 0); // arbitrary
    rec.front_face = true;      // also arbitrary
    rec.mat_ptr = phase_function;
    return true;
}

#endif