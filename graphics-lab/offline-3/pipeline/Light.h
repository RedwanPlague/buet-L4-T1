#ifndef LIGHT_H
#define LIGHT_H

#include "Color.h"
#include "Vector.h"
#include <GL/glut.h>
#include <istream>

struct Light {
    Point pos;
    Color color;

    Light() = default;

    void draw() const {
        const double a = 0.5, x = pos.x, y = pos.y, z = pos.z;
        glPushMatrix();
        glColor3f(color.r, color.g, color.b);
        glBegin(GL_QUADS);
        {
            glVertex3f(x + a, y + a, z + a);
            glVertex3f(x + a, y + a, z - a);
            glVertex3f(x + a, y - a, z - a);
            glVertex3f(x + a, y - a, z + a);

            glVertex3f(x - a, y + a, z + a);
            glVertex3f(x - a, y + a, z - a);
            glVertex3f(x - a, y - a, z - a);
            glVertex3f(x - a, y - a, z + a);

            glVertex3f(x + a, y + a, z + a);
            glVertex3f(x + a, y + a, z - a);
            glVertex3f(x - a, y + a, z - a);
            glVertex3f(x - a, y + a, z + a);

            glVertex3f(x + a, y - a, z + a);
            glVertex3f(x + a, y - a, z - a);
            glVertex3f(x - a, y - a, z - a);
            glVertex3f(x - a, y - a, z + a);

            glVertex3f(x + a, y + a, z + a);
            glVertex3f(x + a, y - a, z + a);
            glVertex3f(x - a, y - a, z + a);
            glVertex3f(x - a, y + a, z + a);

            glVertex3f(x + a, y + a, z - a);
            glVertex3f(x + a, y - a, z - a);
            glVertex3f(x - a, y - a, z - a);
            glVertex3f(x - a, y + a, z - a);
        }
        glEnd();
        glPopMatrix();
    }
};

std::istream &operator>>(std::istream &in, Light &l) {
    in >> l.pos >> l.color;
    return in;
}

#endif // LIGHT_H
