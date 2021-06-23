#ifndef RAY_H
#define RAY_H

#include "Vector.h"

struct Ray {
    Point src;
    Vector dir;

    Ray(Point s, Vector d) : src(s), dir(d / d.norm()) {}

    Ray reflect(Point origin, Vector normal) { return {origin, dir - 2 * dot(dir, normal) * normal}; }
};

#endif // RAY_H
