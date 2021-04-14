#ifndef SHAPES_H
#define SHAPES_H

#include "Vector.hpp"
#include <istream>

struct Triangle {
    Point a, b, c;
    friend std::istream &operator>>(std::istream &in, Triangle &t);
};

std::istream &operator>>(std::istream &in, Triangle &t) {
    in >> t.a >> t.b >> t.c;
    return in;
}

#endif // SHAPES_H
