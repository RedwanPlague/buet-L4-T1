#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Object.h"
#include "Vector.h"
#include <GL/glut.h>
#include <istream>

double determinant(double a1, double a2, double a3, double b1, double b2, double b3, double c1, double c2, double c3) {
    return a1 * (b2 * c3 - b3 * c2) + b1 * (c2 * a3 - c3 * a2) + c1 * (a2 * b3 - a3 * b2);
}

class Triangle : public Object {
    Point a, b, c;

  public:
    Triangle() = default;

    void draw() const {
        glPushMatrix();
        glColor3f(color.r, color.g, color.b);
        glBegin(GL_TRIANGLES);
        {
            glVertex3f(a.x, a.y, a.z);
            glVertex3f(b.x, b.y, b.z);
            glVertex3f(c.x, c.y, c.z);
        }
        glEnd();
        glPopMatrix();
    }

    double intersect(Ray ray) const {
        Point &R0 = ray.src, &Rd = ray.dir;
        double D = determinant(a.x - b.x, a.y - b.y, a.z - b.z, a.x - c.x, a.y - c.y, a.z - c.z, Rd.x, Rd.y, Rd.z);
        if (D == 0)
            return -1;

        double Db = determinant(a.x - R0.x, a.y - R0.y, a.z - R0.z, a.x - c.x, a.y - c.y, a.z - c.z, Rd.x, Rd.y, Rd.z);
        double Dg = determinant(a.x - b.x, a.y - b.y, a.z - b.z, a.x - R0.x, a.y - R0.y, a.z - R0.z, Rd.x, Rd.y, Rd.z);
        double beta = Db / D;
        double gamma = Dg / D;

        if (beta < 0 || gamma < 0 || beta + gamma > 1)
            return -1;

        double Dt = determinant(a.x - b.x, a.y - b.y, a.z - b.z, a.x - c.x, a.y - c.y, a.z - c.z, a.x - R0.x,
                                a.y - R0.y, a.z - R0.z);
        double t = Dt / D;
        return t;
    }

    Vector getNormal(Ray ray) const {
        Vector normal = cross(b - a, c - a);
        return fixNormal(normal, ray);
    }

    friend std::istream &operator>>(std::istream &in, Triangle &t);
};

std::istream &operator>>(std::istream &in, Triangle &t) {
    in >> t.a >> t.b >> t.c >> (Object &)t;
    return in;
}

#endif // TRIANGLE_H
