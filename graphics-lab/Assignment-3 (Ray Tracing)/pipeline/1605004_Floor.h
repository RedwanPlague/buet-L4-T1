#ifndef FLOOR_H
#define FLOOR_H

#include "1605004_Object.h"
#include "1605004_Vector.h"
#include "1605004_bitmap_image.hpp"
#include <GL/glut.h>
#include <algorithm>
#include <istream>
#include <string>

extern bool textureOn;

class Floor : public Object {
    double floorWidth, tileWidth, mid;
    bitmap_image texture;
    double tx, ty;

  public:
    Floor(double fw, double tw, std::string imageName) : floorWidth(fw), tileWidth(tw) {
        texture = bitmap_image(imageName);
        tx = tileWidth / texture.width();
        ty = tileWidth / texture.width();
        mid = floorWidth / 2;
        k.amb = 0.4;
        k.dif = 0.4;
        k.spc = 0.3;
        k.rfl = 0.7;
        k.rfr = 0;
        shine = 10;
    }

    void draw() const {
        for (double x = -mid, start = 1; x < mid; x += tileWidth, start = 1 - start) {
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

    Vector getNormal(Ray ray) const {
        Vector normal(0, 0, 1);
        return fixNormal(normal, ray);
    }
    Color getColor(Point iPoint) const {
        int i = (iPoint.x + mid) / tileWidth;
        int j = (iPoint.y + mid) / tileWidth;
        if (textureOn) {
            if ((i + j) & 1) {
                double left = -mid + i * tileWidth;
                double up = -mid + j * tileWidth + tileWidth;
                int x = (iPoint.x - left) / tx;
                int y = (up - iPoint.y) / ty;
                unsigned char r, g, b;
                texture.get_pixel(x, y, r, g, b);
                return Color(r / 255.0, g / 255.0, b / 255.0);
            }
            else {
                return Color(1, 1, 1);
            }
        }
        else {
            return ((i + j) & 1) ? Color(0, 0, 0) : Color(1, 1, 1);
        }
    }

    double intersect(Ray ray) const {
        Point &R0 = ray.src, &Rd = ray.dir;
        double t = (Rd.z == 0) ? 0 : -R0.z / Rd.z;
        double x = R0.x + t * Rd.x;
        double y = R0.y + t * Rd.y;

        return (-mid < x && x < mid && -mid < y && y < mid) ? t : -1;
    }
};

#endif // FLOOR_H
