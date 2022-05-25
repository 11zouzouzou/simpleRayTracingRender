#include "vec3.h"
#include "color.h"
#include <iostream>

int main()
{
    // 图形大小

    const int image_width = 256;
    const int image_height = 256;

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
            auto r = double(i) / (image_width - 1);
            auto g = double(j) / (image_height - 1);
            auto b = 0.25;
            // static_cast http://c.biancheng.net/cpp/biancheng/view/3297.html
            color pixel_color(r, g, b);
            write_color(std::cout, pixel_color);
        }
    }
    std::cerr << "\nDone.\n";
}