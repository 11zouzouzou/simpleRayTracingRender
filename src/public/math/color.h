#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <iostream>
/**
 * @brief wirte color (RGB)(0~255)
 *
 * @param out
 * @param pixel_color
 */
void write_color(std::ostream &out, color pixel_color)
{
    out << static_cast<int>(255.99 * pixel_color.x()) << ' '
        << static_cast<int>(255.99 * pixel_color.y()) << ' '
        << static_cast<int>(255.99 * pixel_color.z()) << '\n';
}
/**
 * @brief
 *
 * @param out
 * @param pixel_color not normalize ,up to samples_per_pixel
 * @param samples_per_pixel
 */
void write_color_multiply_samples(std::ostream &out, color pixel_color, int samples_per_pixel)
{
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Divide the color by the number of samples and gamma-correct for gamma=2.0.
    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);//gamma矫正输出
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}
#endif