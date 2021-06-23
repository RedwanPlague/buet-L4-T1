#ifndef COLOR_H
#define COLOR_H

#include "Vector.h"
#include <algorithm>
#include <cassert>

double clamp(double a) { return std::clamp(a, 0.0, 1.0); }

struct Color : public Vector {
    Color() = default;
    Color(double x, double y, double z) : Vector(x, y, z) {}

    Color operator*(double m) const { return {x * m, y * m, z * m}; }
    Color operator*(const Color &c) const { return {x * c.x, y * c.y, z * c.z}; }
    Color operator+(const Color &c) const { return {clamp(x + c.x), clamp(y + c.y), clamp(z + c.z)}; }
    void operator-(const Color &) const { assert(false); }
};

#endif // COLOR_H
