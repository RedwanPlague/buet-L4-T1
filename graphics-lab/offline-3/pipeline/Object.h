#ifndef OBJECT_H
#define OBJECT_H

#include "Color.h"
#include "Light.h"
#include "Ray.h"
#include <istream>
#include <vector>

#define __unused __attribute__((unused))

class Object;

extern std::vector<Light> lights;
extern std::vector<Object *> objects;
extern bool shadowOff, reflectionOff;

Object *getNearest(Ray ray, double nearLimit, double farLimit, Object *ignore);

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
    virtual Vector getNormal(Point iPoint) const = 0;

    virtual Color getColor(Point __unused iPoint) const { return color; }

    virtual Color trace(Ray ray, int depth) const {
        double t = intersect(ray);
        Point iPoint = ray.src + t * ray.dir;
        Vector normal = getNormal(iPoint);

        Color iColor = color * k.amb;

        for (auto l : lights) {
            Vector fromL = iPoint - l.pos;
            double lightDistance = fromL.norm();
            fromL /= lightDistance;

            Ray rayToLight = Ray(iPoint, -fromL);
            Object *nearest = getNearest(rayToLight, 0, lightDistance, (Object *)this);
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
        Ray rflRay(iPoint, rflDir);

        Object *nearest = getNearest(rflRay, 0, inf, (Object *)this);
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

Object *getNearest(Ray ray, double nearLimit, double farLimit, Object *ignore) {
    double tmin = farLimit;
    Object *nearest = nullptr;
    for (auto o : objects) {
        if (o != ignore) {
            double t = o->intersect(ray);
            if (t > nearLimit && t < tmin) {
                tmin = t;
                nearest = o;
            }
        }
    }
    return nearest;
}

#endif // OBJECT_H
