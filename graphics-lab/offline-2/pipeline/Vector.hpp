#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm>
#include <cmath>
#include <istream>
#include <ostream>
#include <utility>

const double pi = acos(-1);
const double eps = 1e-7;

struct Vector {
    double x, y, z;

  public:
    Vector() = default;

    Vector(double x, double y, double z) : x(x), y(y), z(z) {}

    Vector(const Vector &v) : x(v.x), y(v.y), z(v.z) {}

    Vector operator+(const Vector &v) const { return {x + v.x, y + v.y, z + v.z}; }
    Vector operator-(const Vector &v) const { return {x - v.x, y - v.y, z - v.z}; }
    Vector operator*(double m) { return {x * m, y * m, z * m}; }
    Vector operator/(double m) { return {x / m, y / m, z / m}; }
    friend Vector operator*(double m, const Vector &v);
    friend Vector operator/(double m, const Vector &v);
    Vector operator-() const { return {-x, -y, -z}; }

    double norm() const { return sqrt(x * x + y * y + z * z); }
};

Vector operator*(double m, const Vector &v) { return {m * v.x, m * v.y, m * v.z}; }

double dot(const Vector &a, const Vector &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
Vector cross(const Vector &a, const Vector &b) {
    return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

std::istream &operator>>(std::istream &in, Vector &v) {
    in >> v.x >> v.y >> v.z;
    return in;
}

std::ostream &operator<<(std::ostream &out, const Vector &v) {
    /* out << '(' << v.x << ", " << v.y << ", " << v.z << ')'; */
    out << v.x << ' ' << v.y << ' ' << v.z;
    return out;
}

typedef Vector Point;

Point rodrigues(Point x, Vector a /*axis (normalized)*/, double angle) {
    angle *= pi / 180;
    double cost = cos(angle);
    double sint = sin(angle);

    return cost * x + (1 - cost) * dot(a, x) * a + sint * cross(a, x);
}

// finds the Point between a and b that has y = y
std::pair<bool, Point> intersect_at_y(Point a, Point b, double y) {
    double coeff = (y - a.y) / (a.y - b.y);
    double x = a.x + (a.x - b.x) * coeff;
    double z = a.z + (a.z - b.z) * coeff;
    Point p(x, y, z);

    bool valid = x + eps > std::min(a.x, b.x) && x < std::max(a.x, b.x) + eps;
    return std::make_pair(valid, p);
}

#endif // VECTOR_H
