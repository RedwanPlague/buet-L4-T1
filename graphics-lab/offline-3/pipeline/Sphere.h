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
    Sphere(Point c, double r) : center(c), radius(r) {}

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

        double r2 = radius * radius;
        double R0dotR0 = dot(R0, R0);
        double tp = dot(-R0, Rd);

        if (R0dotR0 > r2 && tp < 0)
            return -1;

        double d2 = R0dotR0 - tp * tp;

        if (d2 > r2)
            return -1;

        double td = sqrt(r2 - d2);

        return (R0dotR0 < r2) ? tp + td : tp - td;
    }

    Vector getNormal(Ray ray) const { return fixNormal(ray.src - center, ray); }

    friend std::istream &operator>>(std::istream &in, Sphere &s);
};

std::istream &operator>>(std::istream &in, Sphere &s) {
    in >> s.center >> s.radius >> (Object &)s;
    return in;
}

#endif // SPHERE_H
