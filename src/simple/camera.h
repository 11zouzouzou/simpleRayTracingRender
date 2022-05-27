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
    vec3 u, v, w; //保存uv平面与w方向
    double lens_radius;

public:
    camera(
        point3 lookfrom,
        point3 lookat,
        vec3 vup,
        double vfov, // vertical field-of-view in degrees
        double aspect_ratio,
        double aperture,  //光圈孔直径
        double focus_dist //强制设置内距比例
    )
    {
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta / 2);
        //定义camera，视角看向的dir(0,0,-1)始终为视口中心
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        w = unit_vector(lookfrom - lookat); //任意视角摄像机朝向
        u = unit_vector(cross(vup, w));
        v = cross(w, u); //得到垂直与w方向的平面

        origin = lookfrom;
        horizontal = focus_dist * viewport_width * u;                                //水平视口长
        vertical = focus_dist * viewport_height * v;                                 //垂直视口长
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w; //相机视口几何的左下角的点//w为距相机点的距离，为单位长度

        lens_radius = aperture / 2;
    }

    ray get_ray(double s, double t) const
    {
        //景深：https://zhuanlan.zhihu.com/p/400260362
        vec3 rd = lens_radius * random_in_unit_disk(); //随机圆片取点
        vec3 offset = u * rd.x() + v * rd.y();         //沿uv平面偏移量
        return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset);
    }
};

#endif