#ifndef GENERAL_H
#define GENERAL_H

#include "Object.h"
#include "Vector.h"
#include <cmath>
#include <istream>

class General : public Object {
    double A, B, C, D, E, F, G, H, I, J;
    double length, width, height;
    Point ref;

  public:
    General() = default;

    void draw() const {}

    Vector getNormal(Ray ray) const { return Vector(); }

    double intersect(Ray ray) const {
        return -1;
        Vector R0 = ray.src, Rd = ray.dir;
        double a =
            A * Rd.x * Rd.x + B * Rd.y * Rd.y + C * Rd.z * Rd.z + D * Rd.x * Rd.y + E * Rd.x * Rd.z + F * Rd.y * Rd.z;
        double b = 2 * A * Rd.x * R0.x + 2 * B * Rd.y * R0.y + 2 * C * Rd.z * R0.z + D * (Rd.x * R0.y + Rd.y * R0.x) +
                   E * (Rd.x * R0.z + Rd.z * R0.x) + F * (Rd.y * R0.z + Rd.z * R0.y) + G * Rd.x + H * Rd.y + I * Rd.z;
        double c = A * R0.x * R0.x + B * R0.y * R0.z + C * R0.y * R0.z + D * R0.x * R0.y + E * R0.x * R0.z +
                   F * R0.y * R0.z + G * R0.x + H * R0.y + I * R0.z + J;

        if (a == 0)
            return -1;

        double D = b * b - 4 * a * c;
        if (D < 0)
            return -1;

        double sqrtD = sqrt(D);

        double aa = 2 * a;
        double tneg = (-b - sqrtD) / aa;

        if (tneg > 0)
            return tneg;

        double tpos = (-b + sqrtD) / aa;
        return tpos;
    }

    friend std::istream &operator>>(std::istream &in, General &g);
};

std::istream &operator>>(std::istream &in, General &g) {
    in >> g.A >> g.B >> g.C >> g.D >> g.E >> g.F >> g.G >> g.H >> g.I >> g.J;
    in >> g.ref >> g.length >> g.width >> g.height;
    in >> (Object &)g;
    return in;
}

#endif // GENERAL_H
