#ifndef TEXTURE_H
#define TEXTURE_H
#include "utils.h"
#include "perlin.h"
class texture
{
public:
    virtual color value(double u, double v, const point3 &p) const = 0;
};

class solid_color : public texture
{
private:
    color color_value;

public:
    solid_color() {}
    solid_color(color c) : color_value(c) {}
    solid_color(double red, double green, double blue) : color_value(red, green, blue) {}
    virtual color value(double u, double v, const vec3 &p) const override
    {
        // TODO
        return color_value;
    }
};

class checker_texture : public texture
{
public:
    checker_texture() {}

    checker_texture(shared_ptr<texture> _even, shared_ptr<texture> _odd)
        : even(_even), odd(_odd) {}

    checker_texture(color c1, color c2)
        : even(make_shared<solid_color>(c1)), odd(make_shared<solid_color>(c2)) {}

    virtual color value(double u, double v, const point3 &p) const override
    {
        auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
        if (sines < 0)
            return odd->value(u, v, p);
        else
            return even->value(u, v, p);
    }

public:
    shared_ptr<texture> odd;
    shared_ptr<texture> even;
};

class noise_texture : public texture
{
public:
    noise_texture() {}

    noise_texture(double sc) : scale(sc){};

    virtual color value(double u, double v, const point3 &p) const override
    {
        return color(1, 1, 1) * 0.5 * (1.0 - noise.noise(scale * p));
    }

public:
    perlin noise;
    double scale;
};

class noise_turb_texture : public texture
{
public:
    noise_turb_texture() {}

    noise_turb_texture(double sc) : scale(sc){};

    virtual color value(double u, double v, const point3 &p) const override
    {
        // return color(1, 1, 1) * noise.turb(p);
        return color(1, 1, 1) * 0.5 * (1 + sin(scale * p.x() + 10 * noise.turb(p)));//类大理石纹理
    }

public:
    perlin noise;
    double scale = 1.0;
};
#endif