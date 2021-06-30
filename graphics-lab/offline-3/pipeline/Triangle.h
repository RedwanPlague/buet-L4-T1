#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Object.h"
#include "Vector.h"
#include <GL/glut.h>
#include <istream>

class Triangle : public Object {
    Point a, b, c;
    Vector n;

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
        Point ab = a - b, ac = a - c;
        double D = determinant(ab, ac, Rd);
        if (D == 0)
            return -1;

        Point aR0 = a - R0;
        double Db = determinant(aR0, ac, Rd);
        double Dg = determinant(ab, aR0, Rd);
        double beta = Db / D;
        double gamma = Dg / D;

        if (beta < 0 || gamma < 0 || beta + gamma > 1)
            return -1;

        double Dt = determinant(ab, ac, aR0);
        double t = Dt / D;
        return t;
    }

    Vector getNormal(Ray ray) const { return fixNormal(n, ray); }

    friend std::istream &operator>>(std::istream &in, Triangle &t);
};

std::istream &operator>>(std::istream &in, Triangle &t) {
    in >> t.a >> t.b >> t.c >> (Object &)t;
    t.n = cross(t.b - t.a, t.c - t.a);
    return in;
}

#endif // TRIANGLE_H
