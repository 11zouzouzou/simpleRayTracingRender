#ifndef CAMERA_H
#define CAMERA_H

#include "utils.h"

class camera
{
private:
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;

public:
    camera(double vfov, // vertical field-of-view in degrees
           double aspect_ratio)
    {
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta / 2);
        //定义camera，视角看向的dir(0,0,-1)始终为视口中心
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;
        auto focal_length = 1.0;

        origin = point3(0, 0, 0);
        horizontal = vec3(viewport_width, 0, 0);                                               //水平视口长
        vertical = vec3(0, viewport_height, 0);                                                //垂直视口长
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length); //相机视口几何的左下角的点
    }

    ray get_ray(double u, double v) const
    {
        return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
    }
};

#endif