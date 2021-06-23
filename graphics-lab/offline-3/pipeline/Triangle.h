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

    void draw() {
        glPushMatrix();
        glColor3f(color.x, color.y, color.z);
        glBegin(GL_TRIANGLES);
        {
            glVertex3f(a.x, a.y, a.z);
            glVertex3f(b.x, b.y, b.z);
            glVertex3f(c.x, c.y, c.z);
        }
        glEnd();
        glPopMatrix();
    }

    double intersect(Ray ray, Color &color, int depth) { return -1; }

    friend std::istream &operator>>(std::istream &in, Triangle &t);
};

std::istream &operator>>(std::istream &in, Triangle &t) {
    in >> t.a >> t.b >> t.c >> (Object &)t;
    return in;
}

#endif // TRIANGLE_H
