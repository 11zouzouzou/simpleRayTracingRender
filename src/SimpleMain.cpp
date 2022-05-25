#include "vec3.h"
#include "color.h"
#include "ray.h"
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

int main()
{
    // 图形大小 设置为16:9;
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    //定义camera，视角看向的dir(0,0,-1)始终为视口中心
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);                                               //水平视口长
    auto vertical = vec3(0, viewport_height, 0);                                                //垂直视口长
    auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length); //相机视口几何的左下角的点

    // 渲染图像
    std::cout << "P3\n"
              << image_width << ' ' << image_height << "\n255\n";

    //循环颜色输出
    for (int j = image_height - 1; j >= 0; --j)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i)
        {
            // auto 使用https://blog.csdn.net/xiaoquantouer/article/details/51647865
            auto u = double(i) / (image_width - 1);
            auto v = double(j) / (image_height - 1);
            // static_cast http://c.biancheng.net/cpp/biancheng/view/3297.html
            ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }
    std::cerr << "\nDone.\n";
}