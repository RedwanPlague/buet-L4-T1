#ifndef GENERAL_H
#define GENERAL_H

#include "1605004_Object.h"
#include "1605004_Vector.h"
#include <algorithm>
#include <cmath>
#include <istream>

class General : public Object {
    double A, B, C, D, E, F, G, H, I, J;
    bool clipX, clipY, clipZ;
    Point ref1, ref2;

  public:
    General() = default;

    void draw() const {}

    Vector getNormal(Ray ray) const {
        double x = ray.src.x, y = ray.src.y, z = ray.src.z;
        Vector normal(2 * A * x + D * y + E * z + G, 2 * B * y + D * x + F * z + H,
                      2 * C * z + E * x + F * y + I);
        return fixNormal(normal, ray);
    }

    bool fits(Point iPoint) const {
        return (clipX || (ref1.x <= iPoint.x && iPoint.x <= ref2.x)) &&
               (clipY || (ref1.y <= iPoint.y && iPoint.y <= ref2.y)) &&
               (clipZ || (ref1.z <= iPoint.z && iPoint.z <= ref2.z));
    }

    double intersect(Ray ray) const {
        Vector &R0 = ray.src, &Rd = ray.dir;
        double a = (A * Rd.x * Rd.x) + (B * Rd.y * Rd.y) + (C * Rd.z * Rd.z) + (D * Rd.x * Rd.y) +
                   (E * Rd.x * Rd.z) + (F * Rd.y * Rd.z);
        double b = (2 * A * Rd.x * R0.x) + (2 * B * Rd.y * R0.y) + (2 * C * Rd.z * R0.z) +
                   D * (Rd.x * R0.y + Rd.y * R0.x) + E * (Rd.x * R0.z + Rd.z * R0.x) +
                   F * (Rd.y * R0.z + Rd.z * R0.y) + (G * Rd.x) + (H * Rd.y) + (I * Rd.z);
        double c = (A * R0.x * R0.x) + (B * R0.y * R0.y) + (C * R0.z * R0.z) + (D * R0.x * R0.y) +
                   (E * R0.x * R0.z) + (F * R0.y * R0.z) + (G * R0.x) + (H * R0.y) + (I * R0.z) + J;

        if (a == 0)
            return -1;

        double D = b * b - 4 * a * c;
        if (D < 0)
            return -1;

        double sqrtD = sqrt(D);
        double aa = 2 * a;

        double tneg = (-b - sqrtD) / aa;
        double tpos = (-b + sqrtD) / aa;
        if (tpos < tneg)
            std::swap(tpos, tneg);

        if (tneg > 0 && fits(R0 + tneg * Rd))
            return tneg;

        if (tpos > 0 && fits(R0 + tpos * Rd))
            return tpos;

        return -1;
    }

    friend std::istream &operator>>(std::istream &in, General &g);
};

std::istream &operator>>(std::istream &in, General &g) {
    in >> g.A >> g.B >> g.C >> g.D >> g.E >> g.F >> g.G >> g.H >> g.I >> g.J;
    Point lim;
    in >> g.ref1 >> lim;
    in >> (Object &)g;
    g.ref2 = g.ref1 + lim;
    g.clipX = isZero(lim.x);
    g.clipY = isZero(lim.y);
    g.clipZ = isZero(lim.z);
    return in;
}

#endif // GENERAL_H
