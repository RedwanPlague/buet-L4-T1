#ifndef OBJECT_H
#define OBJECT_H

#include "Color.h"
#include "Light.h"
#include "Ray.h"
#include <istream>
#include <vector>

#define __unused __attribute__((unused))

const double ahead = eps;

class Object;

extern std::vector<Light> lights;
extern std::vector<Object *> objects;
extern bool shadowOff, reflectionOff;

Object *getNearest(Ray ray, double nearLimit, double farLimit);

struct Coeffs {
    double amb, dif, spc, rfl, rfr;
};
std::istream &operator>>(std::istream &in, Coeffs &k) {
    in >> k.amb >> k.dif >> k.spc >> k.rfl;
    k.rfr = k.rfl / 2;
    return in;
}

class Object;

class Object {
  protected:
    Color color;
    Coeffs k;
    int shine;

  public:
    Object() = default;
    virtual ~Object() = default;

    virtual void draw() const = 0;
    virtual double intersect(Ray ray) const = 0;

    Vector fixNormal(Vector normal, Ray ray) const {
        normal /= normal.norm();
        return dot(normal, ray.dir) < 0 ? -normal : normal;
    }
    virtual Vector getNormal(Ray ray) const = 0;

    virtual Color getColor(Point __unused iPoint) const { return color; }

    virtual Color trace(Ray ray, int depth) const {
        double t = intersect(ray);
        Point iPoint = ray.src + t * ray.dir;
        Vector normal = getNormal({iPoint, -ray.dir});

        Color iColor = color * k.amb;

        for (auto l : lights) {
            Vector fromL = iPoint - l.pos;
            double lightDistance = fromL.norm();
            fromL /= lightDistance;

            Ray rayToLight = Ray(iPoint - ahead * fromL, -fromL);
            Object *nearest = getNearest(rayToLight, 0, lightDistance);
            if (shadowOff || !nearest) {
                Vector toR = reflect(fromL, normal);

                double lambValue = std::max(0.0, dot(normal, -fromL));
                double phongValue = std::max(0.0, dot(toR, -ray.dir));

                iColor += l.color * k.dif * color * lambValue;
                iColor += l.color * k.spc * pow(phongValue, shine);
            }
        }

        if (depth <= 1 || reflectionOff)
            return iColor;

        Vector rflDir = reflect(ray.dir, normal);
        Ray rflRay(iPoint + ahead * rflDir, rflDir);

        Object *nearest = getNearest(rflRay, 0, inf);
        if (nearest) {
            Color rflColor = nearest->trace(rflRay, depth - 1);
            iColor += rflColor * k.rfl;
        }

        return iColor;
    };

    friend std::istream &operator>>(std::istream &in, Object &o);
};

std::istream &operator>>(std::istream &in, Object &o) {
    in >> o.color >> o.k >> o.shine;
    return in;
}

Object *getNearest(Ray ray, double nearLimit, double farLimit) {
    double tmin = farLimit;
    Object *nearest = nullptr;
    for (auto o : objects) {
        double t = o->intersect(ray);
        if (t > nearLimit && t < tmin) {
            tmin = t;
            nearest = o;
        }
    }
    return nearest;
}

#endif // OBJECT_H
