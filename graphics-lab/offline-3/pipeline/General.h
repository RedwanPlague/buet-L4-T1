#ifndef GENERAL_H
#define GENERAL_H

#include "Object.h"
#include "Vector.h"
#include <istream>

class General : public Object {
    double a, b, c, d, e, f, g, h, i, j;
    double length, width, height;
    Point ref;

  public:
    General() = default;

    void draw() {}

    double intersect(Ray ray, Color &color, int depth) { return -1; }

    friend std::istream &operator>>(std::istream &in, General &g);
};

std::istream &operator>>(std::istream &in, General &g) {
    in >> g.a >> g.b >> g.c >> g.d >> g.e >> g.f >> g.g >> g.h >> g.i >> g.j;
    in >> g.ref >> g.length >> g.width >> g.height;
    in >> (Object &)g;
    return in;
}

#endif // GENERAL_H
