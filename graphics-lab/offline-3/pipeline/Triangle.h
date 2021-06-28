#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Object.h"
#include "Vector.h"
#include <GL/glut.h>
#include <istream>

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

    virtual Vector getNormal(Point iPoint) const { return Vector(); }

    double intersect(Ray ray) const { return -1; }
    Color trace(Ray ray, int depth) const { return Color(); }

    virtual Color trace(Ray ray, int depth) const { return Color(); }

    friend std::istream &operator>>(std::istream &in, Triangle &t);
};

std::istream &operator>>(std::istream &in, Triangle &t) {
    in >> t.a >> t.b >> t.c >> (Object &)t;
    return in;
}

#endif // TRIANGLE_H
