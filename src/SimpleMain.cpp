#include "utils.h"
#include "camera.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "moving_sphere.h"
#include "bvh.h"
#include "texture.h"
#include "aarect.h"
#include "box.h"
#include "constant_medium.h"
#include <iostream>
// test
#include <time.h>

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
        // point3 target = rec.p + rec.normal + random_unit_vector();
        /**
         * 当前射线击中目标时,击中点法线方向为圆心,击中点表面外的单位球上半球内随机取一点作为下一条射线的目标点
         * image7.ppm
         */
        point3 target = rec.p + random_in_hemisphere(rec.normal);

        // 0.5为每次衰减值
        return 0.5 * ray_color_hit_track(ray(rec.p, target - rec.p), world, depth - 1);
        // use material
    }
    //环境颜色
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

color ray_color_hit_track_mat(const ray &r, const hittable &world, int depth)
{
    hit_record rec;
    // 递归超出范围默认为黑色，不做光贡献
    if (depth <= 0)
        return color(0, 0, 0);
    // t_min = 0的话 ，一些反射光线击中了它们所反射的物体，不是精确地𝑡 = 0，导致每次都渲染到最大递归值，无光贡献，画面就会很黑
    if (world.hit(r, 0.001, infinity, rec))
    {
        //递归
        // use material
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        {
            // attenuation 为这次射线的材质的颜色值
            return attenuation * ray_color_hit_track_mat(scattered, world, depth - 1);
        }
        return color(0, 0, 0);
    }
    //环境颜色
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

color ray_color_hit_track_mat_light(const ray &r, const color &background, const hittable &world, int depth)
{
    hit_record rec;
    // 递归超出范围默认为黑色，不做光贡献
    if (depth <= 0)
        return color(0, 0, 0);

    // If the ray hits nothing, return the background color.//t_min = 0的话 ，一些反射光线击中了它们所反射的物体，不是精确地𝑡 = 0，导致每次都渲染到最大递归值，无光贡献，画面就会很黑
    if (!world.hit(r, 0.001, infinity, rec))
        return background;

    //递归
    // use material
    ray scattered;
    color attenuation;
    color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p); //击中的材质是否发光

    if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
    {
        // attenuation 为这次射线的材质的颜色值
        return emitted + attenuation * ray_color_hit_track_mat_light(scattered, background, world, depth - 1);
    }
    return emitted;
}

hittable_list create_scene3d()
{
    hittable_list world;
    // material
    auto material_center = make_shared<lambertian_material>(make_shared<solid_color>(0.7, 0.7, 0.0));
    auto material_checker = make_shared<lambertian_material>(make_shared<checker_texture>(color(0.2, 0.3, 0.3), color(0.9, 0.9, 0.9)));
    auto material_ground = make_shared<lambertian_material>(color(0.5, 1.0, 1.0));
    auto material_left = make_shared<metal_material>(color(0.7, 0.6, 0.5), 0.0);
    auto material_right = make_shared<metal_material>(color(1, 0, 0.5), 0.2);
    auto material_dielectric = make_shared<dielectric_material>(1.5);
    //随机生成
    for (int a = -3; a < 3; a++)
    {
        for (int b = -3; b < 3; b++)
        {
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.0, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9)
            {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.7)
                {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian_material>(albedo);
                    auto center2 = center + vec3(0, random_double(0, 0.2), 0); //随机高度
                    world.add(make_shared<moving_sphere>(center, center2, 0.0, 1.0, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95)
                {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal_material>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else
                {
                    // glass
                    sphere_material = make_shared<dielectric_material>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100.0, material_checker));
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5, material_left));
    world.add(make_shared<sphere>(point3(-1, 0, -1), 0.5, material_dielectric));
    world.add(make_shared<sphere>(point3(-1, 0, -1), -0.3, material_dielectric)); //负值 ，几何形状不受影响，但表面法线指向内部。这可以用作制造空心玻璃球的气泡
    world.add(make_shared<sphere>(point3(0, 0, -0.5), 0.1, material_center));
    world.add(make_shared<sphere>(point3(1, 0, -1), 0.5, material_right));
    // return world;
    return hittable_list(make_shared<bvh_node>(world, 0.0, 1.0)); //当数量比较多时起作用
}

hittable_list create_two_sphere_scene3d()
{
    hittable_list world;
    auto material_checker_down = make_shared<lambertian_material>(make_shared<checker_texture>(color(1.0, 0.5, 0.0), color(0.8, 0.8, 0.8)));
    auto material_checker_up = make_shared<lambertian_material>(make_shared<checker_texture>(color(0.9, 0.2, 0.5), color(0.8, 0.8, 0.8)));
    world.add(make_shared<sphere>(point3(0.0, -20, 0.0), 20.0, material_checker_down));
    world.add(make_shared<sphere>(point3(0.0, 20.0, 0.0), 20.0, material_checker_up));
    return world;
}

hittable_list create_two_perlin_spheres()
{
    hittable_list world;
    auto pertext = make_shared<noise_texture>(5);
    auto pertext_trub = make_shared<noise_turb_texture>(5);
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian_material>(pertext_trub)));
    world.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian_material>(pertext_trub)));
    return world;
}

hittable_list create_earth()
{
    auto earth_texture = make_shared<image_texture>("static/earthmap.jpg");
    auto earth_surface = make_shared<lambertian_material>(earth_texture);
    auto globe = make_shared<sphere>(point3(0, 0, 0), 2, earth_surface);

    return hittable_list(globe);
}

hittable_list simple_light()
{
    hittable_list objects;

    auto pertext = make_shared<noise_turb_texture>(4);
    objects.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian_material>(pertext)));
    objects.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian_material>(pertext)));

    auto earth_texture = make_shared<image_texture>("static/earthmap.jpg");
    auto earth_difflight = make_shared<diffuse_light>(earth_texture);
    auto difflight = make_shared<diffuse_light>(color(4, 4, 4));
    objects.add(make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));
    objects.add(make_shared<sphere>(point3(0, 2, 6), 2, earth_difflight));

    return objects;
}

hittable_list cornell_box()
{
    hittable_list objects;

    auto red = make_shared<lambertian_material>(color(.65, .05, .05));
    auto white = make_shared<lambertian_material>(color(.73, .73, .73));
    auto green = make_shared<lambertian_material>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<xz_rect>(213, 343, 227, 332, 554, light));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    shared_ptr<hittable> box1 = make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265, 0, 295));
    objects.add(box1);

    shared_ptr<hittable> box2 = make_shared<box>(point3(0, 0, 0), point3(165, 165, 165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130, 0, 65));
    objects.add(box2);
    return objects;
}

hittable_list cornell_smoke()
{
    hittable_list objects;

    auto red = make_shared<lambertian_material>(color(.65, .05, .05));
    auto white = make_shared<lambertian_material>(color(.73, .73, .73));
    auto green = make_shared<lambertian_material>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(7, 7, 7));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<xz_rect>(113, 443, 127, 432, 554, light));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    shared_ptr<hittable> box1 = make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265, 0, 295));

    shared_ptr<hittable> box2 = make_shared<box>(point3(0, 0, 0), point3(165, 165, 165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130, 0, 65));

    objects.add(make_shared<constant_medium>(box1, 0.01, color(0, 0, 0)));
    objects.add(make_shared<constant_medium>(box2, 0.01, color(0.5, 0.5, 1)));

    return objects;
}

hittable_list final_scene()
{
    hittable_list boxes1;
    auto ground = make_shared<lambertian_material>(color(0.48, 0.83, 0.53));

    const int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++)
    {
        for (int j = 0; j < boxes_per_side; j++)
        {
            auto w = 100.0;
            auto x0 = -1000.0 + i * w;
            auto z0 = -1000.0 + j * w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = random_double(1, 101);
            auto z1 = z0 + w;
            //凹凸地板，漫反射
            boxes1.add(make_shared<box>(point3(x0, y0, z0), point3(x1, y1, z1), ground));
        }
    }

    hittable_list objects;

    objects.add(make_shared<bvh_node>(boxes1, 0, 1));

    auto light = make_shared<diffuse_light>(color(7, 7, 7));
    objects.add(make_shared<xz_rect>(123, 423, 147, 412, 554, light)); //矩形灯

    auto center1 = point3(400, 400, 200);
    auto center2 = center1 + vec3(30, 0, 0);
    auto moving_sphere_material = make_shared<lambertian_material>(color(0.7, 0.3, 0.1));
    objects.add(make_shared<moving_sphere>(center1, center2, 0, 1, 50, moving_sphere_material)); //移动黄球

    objects.add(make_shared<sphere>(point3(260, 150, 45), 50, make_shared<dielectric_material>(1.5)));//电介质材质球
    objects.add(make_shared<sphere>(
        point3(0, 150, 145), 50, make_shared<metal_material>(color(0.8, 0.8, 0.9), 1.0)));//白色金属球

    auto boundary = make_shared<sphere>(point3(360, 150, 145), 70, make_shared<dielectric_material>(1.5));
    objects.add(boundary);
    objects.add(make_shared<constant_medium>(boundary, 0.2, color(0.2, 0.4, 0.9)));//蓝球雾
    boundary = make_shared<sphere>(point3(0, 0, 0), 5000, make_shared<dielectric_material>(1.5));
    objects.add(make_shared<constant_medium>(boundary, .0001, color(1, 1, 1)));//巨大白球雾

    auto emat = make_shared<lambertian_material>(make_shared<image_texture>("static/earthmap.jpg"));
    objects.add(make_shared<sphere>(point3(400, 200, 400), 100, emat));//地球
    auto pertext = make_shared<noise_texture>(0.1);
    objects.add(make_shared<sphere>(point3(220, 280, 300), 80, make_shared<lambertian_material>(pertext)));//噪声球

    hittable_list boxes2;
    auto white = make_shared<lambertian_material>(color(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++)
    {
        boxes2.add(make_shared<sphere>(point3::random(0, 165), 10, white));//聚集的方形白球
    }

    objects.add(make_shared<translate>(
        make_shared<rotate_y>(
            make_shared<bvh_node>(boxes2, 0.0, 1.0), 15),
        vec3(-100, 270, 395)));

    return objects;
}

int main()
{
    time_t c_start = clock();

    // 图形大小 设置为16:9;
    auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    int image_height = static_cast<int>(image_width / aspect_ratio);

    // camera
    point3 lookfrom(3, 2, 3);
    point3 lookat(0, 0, -1);
    auto dist_to_focus = (lookfrom - lookat).length(); //焦点的距离
    auto aperture = 0.1;                               //孔距
    double vfov = 45.0;
    //构造场景
    // World
    hittable_list world = create_scene3d();

    color background = color(0.70, 0.80, 1.00);

    //抗锯齿周围像素样本采样数
    int samples_per_pixel = 10;
    //一条射线反弹递归最大数
    const int max_depth = 50;

    switch (8)
    {
    case 1:
        world = create_scene3d();
        lookfrom = point3(3, 2, 3);
        lookat = point3(0, 0, -1);
        dist_to_focus = (lookfrom - lookat).length(); //焦点的距离
        aperture = 0.1;
        break;
    case 2:
        lookfrom = point3(0, 3, 20);
        lookat = point3(0, 0, 0);
        aperture = 0.0;
        dist_to_focus = (lookfrom - lookat).length();
        world = create_two_sphere_scene3d();
        break;
    case 3:
        world = create_two_perlin_spheres();
        lookfrom = point3(13, 2, 3);
        lookat = point3(0, 0, 0);
        aperture = 0.0;
        dist_to_focus = (lookfrom - lookat).length();
        vfov = 20.0;
        break;
    case 4:
        world = create_earth();
        lookfrom = point3(13, 2, 3);
        lookat = point3(0, 0, 0);
        aperture = 0.0;
        dist_to_focus = (lookfrom - lookat).length();
        vfov = 20.0;
        break;
    case 5:
        world = simple_light();
        samples_per_pixel = 400;
        background = color(0, 0, 0);
        lookfrom = point3(26, 3, 6);
        lookat = point3(0, 2, 0);
        aperture = 0.0;
        dist_to_focus = (lookfrom - lookat).length();
        vfov = 20.0;
        break;
    default:
    case 6:
        // world = cornell_box();
        world = cornell_smoke();
        aspect_ratio = 1.0;
        image_height = static_cast<int>(image_width / aspect_ratio);
        samples_per_pixel = 10;
        background = color(0.0, 0.0, 0.0);
        lookfrom = point3(278, 278, -800);
        lookat = point3(278, 278, 0);
        dist_to_focus = (lookfrom - lookat).length();
        vfov = 40.0;
        break;
    case 8:
        world = final_scene();
        aspect_ratio = 1.0;
        image_height = static_cast<int>(image_width / aspect_ratio);
        samples_per_pixel = 400;
        background = color(0, 0, 0);
        lookfrom = point3(478, 278, -600);
        lookat = point3(278, 278, 0);
        dist_to_focus = (lookfrom - lookat).length();
        vfov = 40.0;
        break;
    }
    // camera
    camera cam(lookfrom, lookat, vec3(0, 1, 0), vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

    time_t c_scene3d_end = clock();
    std::cerr << "create scene3d time: " << difftime(c_scene3d_end, c_start) << "\n";
    time_t c_render_start = clock();
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
                //  pixel_color += ray_color_hit_track(r, world, max_depth);
                // pixel_color += ray_color_hit_track_mat(r, world, max_depth);
                pixel_color += ray_color_hit_track_mat_light(r, background, world, max_depth);
            }
            write_color_multiply_samples(std::cout, pixel_color, samples_per_pixel);
        }
    }
    time_t c_render_end = clock();
    std::cerr << "\n render time:" << difftime(c_render_end, c_render_start) << "\n";
    std::cerr << "\nDone.\n";
}