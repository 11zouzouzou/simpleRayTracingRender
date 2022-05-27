#ifndef TEXTURE_H
#define TEXTURE_H
#include "utils.h"
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
#endif