#ifndef SHAPES_H
#define SHAPES_H

#include "Matrix.hpp"
#include "Vector.hpp"
#include <algorithm>
#include <cassert>
#include <istream>

struct Triangle {
    Point a, b, c;
    friend std::istream &operator>>(std::istream &in, Triangle &t);

    void transform(Matrix m) {
        a = m * a;
        b = m * b;
        c = m * c;
    }

    inline double miny() { return std::min(a.y, std::min(b.y, c.y)); }
    inline double maxy() { return std::max(a.y, std::max(b.y, c.y)); }

    std::pair<Point, Point> cut_at_y(double y) {
        auto pab = intersect_at_y(a, b, y);
        auto pbc = intersect_at_y(b, c, y);
        auto pca = intersect_at_y(c, a, y);

        auto A = pca, B = pca;
        if (pab.first && pbc.first) {
            A = pab, B = pbc;
        }
        else if (pbc.first && pca.first) {
            A = pbc, B = pca;
        }
        else if (pca.first && pab.first) {
            A = pca, B = pab;
        }
        else if (pab.first) {
            A = B = pab;
        }
        else if (pbc.first) {
            A = B = pbc;
        }

        if (A.second.x > B.second.x) {
            swap(A, B);
        }
        return std::make_pair(A.second, B.second);
    }
};

std::istream &operator>>(std::istream &in, Triangle &t) {
    in >> t.a >> t.b >> t.c;
    return in;
}

std::ostream &operator<<(std::ostream &out, Triangle &t) {
    out << t.a << '\n';
    out << t.b << '\n';
    out << t.c << '\n';
    return out;
}

struct RandomColor {
    unsigned char r, g, b;

    RandomColor() {
        r = rand() % 256;
        g = rand() % 256;
        b = rand() % 256;
    }
};

#endif // SHAPES_H
