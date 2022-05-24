#include <iostream>

int main(){
    // 图形大小

    const int image_width = 256;
    const int image_height = 256;

    // 渲染图像

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
         std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            //auto 使用https://blog.csdn.net/xiaoquantouer/article/details/51647865
            auto r = double(i) / (image_width-1);
            auto g = double(j) / (image_height-1);
            auto b = 0.25;
            //static_cast http://c.biancheng.net/cpp/biancheng/view/3297.html
            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);

            std::cout << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
     std::cerr << "\nDone.\n";
}