#ifndef SPHERE_H
#define SPHERE_H

#include "Object.h"
#include <GL/glut.h>
#include <cassert>
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

        double sqrtD = sqrt(D);

        double t1 = (-b - sqrtD) / (2 * a);
        double t2 = (-b + sqrtD) / (2 * a);
        double t = (t1 >= 0) ? t1 : t2;

        return t;
    }

    Vector getNormal(Point iPoint) const {
        Vector normal = iPoint - center;
        return normal / normal.norm();
    }

    friend std::istream &operator>>(std::istream &in, Sphere &s);
};

std::istream &operator>>(std::istream &in, Sphere &s) {
    in >> s.center >> s.radius >> (Object &)s;
    return in;
}

#endif // SPHERE_H
