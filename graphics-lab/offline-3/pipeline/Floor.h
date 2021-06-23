#ifndef FLOOR_H
#define FLOOR_H

#include "Object.h"
#include "Vector.h"
#include <GL/gl.h>
#include <GL/glut.h>
#include <algorithm>
#include <istream>

class Floor : public Object {
    double floorWidth, tileWidth;

  public:
    Floor(double fw, double tw) : floorWidth(fw), tileWidth(tw) {
        k.amb = k.dif = k.spc = k.rfl = 1;
        k.rfr = 0;
        shine = 10;
    }

    void draw() {
        double mid = floorWidth / 2;
        for (double x = -mid, start = 0; x < mid; x += tileWidth, start = 1 - start) {
            for (double y = -mid, shade = start; y < mid; y += tileWidth, shade = 1 - shade) {
                glColor3f(shade, shade, shade);

                double xw = std::min(x + tileWidth, mid);
                double yw = std::min(y + tileWidth, mid);
                glBegin(GL_QUADS);
                {
                    glVertex3f(x, y, 0);
                    glVertex3f(xw, y, 0);
                    glVertex3f(xw, yw, 0);
                    glVertex3f(x, yw, 0);
                }
                glEnd();
            }
        }
    }

    virtual double intersect(Ray ray, Color &color, int depth) { return -1; };
};

#endif // FLOOR_H