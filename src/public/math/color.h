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
#endif