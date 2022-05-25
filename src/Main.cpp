#include "vec3.h"
#include "color.h"
#include "ray.h"
#include <iostream>

color ray_color(const ray &r)
{
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);//[-1,1]=>[0,1]//y方向有一点点不准确，精度问题
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);//线性混合
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