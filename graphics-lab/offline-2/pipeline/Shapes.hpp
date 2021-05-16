#ifndef SHAPES_H
#define SHAPES_H

#include "Matrix.hpp"
#include "Vector.hpp"
#include <istream>

struct Triangle {
    Point a, b, c;
    friend std::istream &operator>>(std::istream &in, Triangle &t);

    void transform(Matrix m) {
        a = m * a;
        b = m * b;
        c = m * c;
    }
};

std::istream &operator>>(std::istream &in, Triangle &t) {
    in >> t.a >> t.b >> t.c;
    return in;
}

std::ostream &operator<<(std::ostream &out, Triangle &t) {
    out << t.a << '\n';
    out << t.b << '\n';
    out << t.c << '\n';
    return out;
}

#endif // SHAPES_H
