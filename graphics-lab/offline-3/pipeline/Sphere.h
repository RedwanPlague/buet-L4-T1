#ifndef SPHERE_H
#define SPHERE_H

#include "Object.h"
#include <GL/glut.h>
#include <cmath>
#include <istream>

class Sphere : public Object {
    Point center;
    double radius;

  public:
    Sphere() = default;

    void draw() const {
        const int slices = 50, stacks = 30;

        Point points[2][slices + 1];
        int prev = 0, cur = 1;

        Point pole(radius, 0, 0);
        for (int j = 0; j <= slices; j++) {
            points[prev][j] = pole;
        }

        glPushMatrix();

        glColor3f(color.r, color.g, color.b);
        glTranslatef(center.x, center.y, center.z);

        for (int i = 1; i <= stacks; i++, prev ^= 1, cur ^= 1) {
            double angleStack = (double)i / stacks * (pi / 2);
            double h = radius * cos(angleStack);
            double r = fabs(radius) * sin(angleStack);
            for (int j = 0; j <= slices; j++) {
                double angleSlice = ((double)j / (double)slices) * 2 * pi;
                points[cur][j].x = h;
                points[cur][j].y = r * sin(angleSlice);
                points[cur][j].z = -r * cos(angleSlice);
            }
            for (int j = 0; j < slices; j++) {
                glBegin(GL_QUADS);
                {
                    glVertex3f(points[cur][j].x, points[cur][j].y, points[cur][j].z);
                    glVertex3f(points[cur][j + 1].x, points[cur][j + 1].y, points[cur][j + 1].z);
                    glVertex3f(points[prev][j + 1].x, points[prev][j + 1].y, points[prev][j + 1].z);
                    glVertex3f(points[prev][j].x, points[prev][j].y, points[prev][j].z);
                }
                glBegin(GL_QUADS);
                {
                    glVertex3f(-points[cur][j].x, points[cur][j].y, points[cur][j].z);
                    glVertex3f(-points[cur][j + 1].x, points[cur][j + 1].y, points[cur][j + 1].z);
                    glVertex3f(-points[prev][j + 1].x, points[prev][j + 1].y, points[prev][j + 1].z);
                    glVertex3f(-points[prev][j].x, points[prev][j].y, points[prev][j].z);
                }
                glEnd();
            }
        }

        glPopMatrix();
    }

    double intersect(Ray ray) const {
        Vector R0 = ray.src - center;
        Vector Rd = ray.dir;

        double a = 1;
        double b = 2 * dot(Rd, R0);
        double c = dot(R0, R0) - radius * radius;
        double D = b * b - 4 * a * c;

        if (D < 0)
            return -1;

        D = sqrt(D);

        double t1 = (-b - D) / (2 * a);
        double t2 = (-b + D) / (2 * a);
        double t = (t1 >= 0) ? t1 : t2;

        return t;
    }

    Color trace(Ray ray, int depth) const {
        double t = intersect(ray);
        Point iPoint = ray.src + t * ray.dir;
        Vector normal = iPoint - center;
        normal /= normal.norm();

        Color iColor = color * k.amb;

        for (auto l : lights) {
            Vector toL = l.pos - iPoint;
            toL /= toL.norm();
            Vector toR = reflect(-toL, normal);

            double lambValue = dot(normal, toL);
            double phongValue = dot(toR, -ray.dir);

            iColor += l.color * k.dif * color * lambValue;
            iColor += l.color * k.spc * pow(phongValue, shine);
        }

        if (depth <= 1)
            return iColor;

        Vector rflDir = reflect(ray.dir, normal);
        Ray rflRay(iPoint + 0.1 * rflDir, rflDir);

        double tmin = inf;
        Object *nearest = nullptr;
        for (auto o : objects) {
            double t = o->intersect(ray);
            if (t > 0 && t < tmin) {
                tmin = t;
                nearest = o;
            }
        }
        if (nearest) {
            Color rflColor = nearest->trace(ray, depth - 1);
            iColor += rflColor * k.rfl;
        }

        return iColor;
    }

    friend std::istream &operator>>(std::istream &in, Sphere &s);
};

std::istream &operator>>(std::istream &in, Sphere &s) {
    in >> s.center >> s.radius >> (Object &)s;
    return in;
}

#endif // SPHERE_H
