#ifndef LIGHT_H
#define LIGHT_H

#include "Color.h"
#include "Vector.h"
#include <istream>

struct Light {
    Point pos;
    Color color;

    Light() = default;
};

std::istream &operator>>(std::istream &in, Light &l) {
    in >> l.pos >> l.color;
    return in;
}

#endif // LIGHT_H
