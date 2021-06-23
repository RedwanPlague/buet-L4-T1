#ifndef OBJECT_H
#define OBJECT_H

#include "Color.h"
#include "Ray.h"
#include <istream>

struct Coeffs {
    double amb, dif, spc, rfl, rfr;
};
std::istream &operator>>(std::istream &in, Coeffs &k) {
    in >> k.amb >> k.dif >> k.spc >> k.rfl;
    k.rfr = k.rfl / 2;
    return in;
}

class Object {
  protected:
    Color color;
    Coeffs k;
    int shine;

  public:
    Object() = default;
    virtual ~Object() = default;

    virtual void draw() = 0;
    virtual double intersect(Ray ray, Color &color, int depth) = 0;

    friend std::istream &operator>>(std::istream &in, Object &o);
};

std::istream &operator>>(std::istream &in, Object &o) {
    in >> o.color >> o.k >> o.shine;
    return in;
}

#endif // OBJECT_H
