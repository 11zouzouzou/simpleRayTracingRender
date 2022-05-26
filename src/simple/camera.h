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
    camera(
        point3 lookfrom,
        point3 lookat,
        vec3 vup,
        double vfov, // vertical field-of-view in degrees
        double aspect_ratio)
    {
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta / 2);
        //定义camera，视角看向的dir(0,0,-1)始终为视口中心
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        auto w = unit_vector(lookfrom - lookat); //任意视角摄像机朝向
        auto u = unit_vector(cross(vup, w));
        auto v = cross(w, u); //得到垂直与w方向的平面

        origin = lookfrom;
        horizontal = viewport_width * u;                                //水平视口长
        vertical = viewport_height * v;                                 //垂直视口长
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - w; //相机视口几何的左下角的点//w为距相机点的距离，为单位长度
    }

    ray get_ray(double s, double t) const
    {
        return ray(origin, lower_left_corner + s * horizontal + t * vertical - origin);
    }
};

#endif