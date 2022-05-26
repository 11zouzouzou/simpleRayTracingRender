#include "utils.h"
#include "camera.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include <iostream>

double hit_sphere(const point3 &center, double radius, const ray &r)
{
    // https://blog.csdn.net/air_liang1212/article/details/89633793
    // vec3 oc = r.origin() - center;
    // auto a = dot(r.direction(), r.direction());
    // auto b = 2.0 * dot(oc, r.direction());
    // auto c = dot(oc, oc) - radius * radius;
    // auto discriminant = b * b - 4 * a * c;
    // if (discriminant < 0)
    // {
    //     return -1;
    // }
    // else
    // {
    //     return (-b - sqrt(discriminant)) / (2.0 * a);
    // }
    //优化计算
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared(); //自身点积结果等于向量长度的平方
    auto half_b = dot(oc, r.direction());    //只取b的half
    auto c = oc.length_squared() - radius * radius;
    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0)
    {
        return -1;
    }
    else
    {
        return (-half_b - sqrt(discriminant)) / a;
    }
}

color ray_color(const ray &r)
{
    //与球碰撞相交
    auto center = point3(0, 0, -1);
    auto x = hit_sphere(center, 0.5, r); // x坐标
    if (x > 0.0)
    {
        vec3 N = unit_vector(r.at(x) - center); // r.at(x):球面交点
        return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);
    }

    vec3 unit_direction = unit_vector(r.direction());                   //渐变色
    auto t = 0.5 * (unit_direction.y() + 1.0);                          //[-1,1]=>[0,1]//y方向有一点点不准确，精度问题
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0); //线性混合
}

color ray_color_hit(const ray &r, const hittable &world)
{
    hit_record rec;
    if (world.hit(r, 0, infinity, rec))
    {
        return 0.5 * (rec.normal + color(1, 1, 1));
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}
color ray_color_hit_track(const ray &r, const hittable &world, int depth)
{
    hit_record rec;
    // 递归超出范围默认为黑色，不做光贡献
    if (depth <= 0)
        return color(0, 0, 0);
    // t_min = 0的话 ，一些反射光线击中了它们所反射的物体，不是精确地𝑡 = 0，导致每次都渲染到最大递归值，无光贡献，画面就会很黑
    if (world.hit(r, 0.001, infinity, rec))
    {
        //递归
        /**
         * 当前射线击中目标时,击中点法线方向为圆心,击中点表面外的单位球内的随机取一点作为下一条射线的目标点 
         * 接近法线的高概率在半球上拾取方向，并且在掠射角处散射光线的概率较低。
         * 以小角度到达的光会散布在更大的区域上，因此对最终颜色的贡献较小。
         * image5.ppm
         */
        // point3 target = rec.p + rec.normal + random_in_unit_sphere();
        /**
         * 当前射线击中目标时,击中点法线方向为圆心,击中点表面外的单位球表面的随机取一点作为下一条射线的目标点 
         * Lambertian reflectance
         * 比取球体内点 光线的散射更均匀；朝向法线散射的光线更少
         * 完全漫反射
         * image6.ppm
         */
        point3 target = rec.p + rec.normal + random_unit_vector();
        //0.5为每次衰减值
        return 0.5 * ray_color_hit_track(ray(rec.p, target - rec.p), world, depth - 1);
    }
    //环境颜色
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main()
{
    // 图形大小 设置为16:9;
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // camera
    camera cam(aspect_ratio);

    //构造场景
    // World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    //抗锯齿周围像素样本采样数
    const int samples_per_pixel = 100;
    //一条射线反弹递归最大数
    const int max_depth = 50;

    // 渲染图像
    std::cout << "P3\n"
              << image_width << ' ' << image_height << "\n255\n";

    //循环颜色输出
    for (int j = image_height - 1; j >= 0; --j)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i)
        {
            //正常采样
            // // auto 使用https://blog.csdn.net/xiaoquantouer/article/details/51647865
            // auto u = double(i) / (image_width - 1);
            // auto v = double(j) / (image_height - 1);
            // // static_cast http://c.biancheng.net/cpp/biancheng/view/3297.html
            // ray r = cam.get_ray(u, v);
            // // color pixel_color = ray_color(r);
            // color pixel_color = ray_color_hit(r, world);
            //  write_color(std::cout, pixel_color);
            //采样周围像素混合，抗锯齿采样
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; s++)
            {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                // pixel_color += ray_color_hit(r, world);
                pixel_color += ray_color_hit_track(r, world, max_depth);
            }
            write_color_multiply_samples(std::cout, pixel_color, samples_per_pixel);
        }
    }
    std::cerr << "\nDone.\n";
}