#ifndef OBJECT_H
#define OBJECT_H

#include "1605004_Color.h"
#include "1605004_Light.h"
#include "1605004_Ray.h"
#include <istream>
#include <vector>

#define __unused __attribute__((unused))

const double ahead = eps;

class Object;

extern std::vector<Light> lights;
extern std::vector<Object *> objects;
extern bool shadowOn, reflectionOn, refractionOn;

bool isZero(double d) { return std::abs(d) < eps; }
Object *getNearest(Ray ray, double nearLimit = 0, double farLimit = inf, bool shadow = false);

struct Coeffs {
    double amb, dif, spc, rfl, rfr;
};
std::istream &operator>>(std::istream &in, Coeffs &k) {
    /* in >> k.amb >> k.dif >> k.spc >> k.rfl >> k.rfr; */
    in >> k.amb >> k.dif >> k.spc >> k.rfl;
    k.rfr = 0.0;
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
    virtual double getKrfr(Point __unused iPoint) const { return k.rfr; }

    virtual Color trace(Ray ray, int depth) const {
        double t = intersect(ray);
        Point iPoint = ray.src + t * ray.dir;
        Vector normal = getNormal({iPoint, -ray.dir});

        Color iColor = getColor(iPoint);

        Color tColor = iColor * k.amb;

        for (auto l : lights) {
            Vector fromL = iPoint - l.pos;
            double lightDistance = fromL.norm();
            fromL /= lightDistance;

            Ray rayToLight = Ray(iPoint - ahead * fromL, -fromL);
            Object *nearest = nullptr;
            if (shadowOn) {
                nearest = getNearest(rayToLight, 0, lightDistance, true);
            }
            if (!nearest) {
                Vector toR = reflect(fromL, normal);

                double lambValue = std::max(0.0, dot(normal, -fromL));
                double phongValue = std::max(0.0, dot(toR, -ray.dir));

                tColor += l.color * k.dif * iColor * lambValue;
                tColor += l.color * k.spc * pow(phongValue, shine);
            }
        }

        if (depth <= 1)
            return tColor;

        if (reflectionOn) {
            Vector rflDir = reflect(ray.dir, normal);
            Ray rflRay(iPoint + ahead * rflDir, rflDir);

            Object *nearest = getNearest(rflRay);
            if (nearest) {
                Color rflColor = nearest->trace(rflRay, depth - 1);
                tColor += rflColor * k.rfl;
            }
        }

        if (refractionOn) {
            double iKrfr = getKrfr(ray.src);
            if (isZero(iKrfr))
                return tColor;

            Vector rfrDir = refract(ray.dir, normal, iKrfr);
            Ray rfrRay(iPoint + ahead * rfrDir, rfrDir);

            Object *nearest = getNearest(rfrRay);
            if (nearest) {
                Color rfrColor = nearest->trace(rfrRay, depth - 1);
                tColor += rfrColor;
            }
        }

        return tColor;
    };

    friend std::istream &operator>>(std::istream &in, Object &o);
};

std::istream &operator>>(std::istream &in, Object &o) {
    in >> o.color >> o.k >> o.shine;
    return in;
}

Object *getNearest(Ray ray, double nearLimit, double farLimit, bool shadow) {
    double tmin = farLimit;
    Object *nearest = nullptr;
    for (auto o : objects) {
        double t = o->intersect(ray);
        if (t > nearLimit && t < tmin) {
            if (shadow)
                return o;
            tmin = t;
            nearest = o;
        }
    }
    return nearest;
}

#endif // OBJECT_H
