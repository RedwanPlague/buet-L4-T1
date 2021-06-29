#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm>
#include <cmath>
#include <istream>
#include <ostream>

const double pi = acos(-1);
const double eps = 1e-7;
const double inf = 1e5;

struct Vector {
    double x, y, z;

    Vector() = default;
    Vector(double x, double y, double z) : x(x), y(y), z(z) {}

    Vector operator+(const Vector &v) const { return {x + v.x, y + v.y, z + v.z}; }
    Vector operator-(const Vector &v) const { return {x - v.x, y - v.y, z - v.z}; }
    Vector operator*(double m) const { return {x * m, y * m, z * m}; }
    Vector operator/(double m) const { return {x / m, y / m, z / m}; }
    Vector operator-() const { return {-x, -y, -z}; }

    Vector &operator+=(const Vector &v) { return *this = *this + v; }
    Vector &operator-=(const Vector &v) { return *this = *this - v; }
    Vector &operator*=(double m) { return *this = *this * m; }
    Vector &operator/=(double m) { return *this = *this / m; }

    bool operator==(const Vector &v) const {
        return std::abs(x - v.x) < eps && std::abs(y - v.y) < eps && std::abs(z - v.z) < eps;
    }

    [[nodiscard]] double norm() const { return sqrt(x * x + y * y + z * z); }
};

Vector operator*(double m, const Vector &v) { return {m * v.x, m * v.y, m * v.z}; }

double dot(const Vector &a, const Vector &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
Vector cross(const Vector &a, const Vector &b) {
    return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}
double determinant(Vector a, Vector b, Vector c) { return dot(a, cross(b, c)); }

std::istream &operator>>(std::istream &in, Vector &v) {
    in >> v.x >> v.y >> v.z;
    return in;
}

std::ostream &operator<<(std::ostream &out, const Vector &v) {
    out << v.x << ' ' << v.y << ' ' << v.z;
    return out;
}

typedef Vector Point;

// rotate Vector 'whom' around Vector 'axis', anticlockwise
// 'whom' and 'axis' are orthogonal unit Vectors
Vector orthogonalRotate(Vector whom, Vector axis, double angle) {
    angle *= pi / 180;
    Vector direction = cross(axis, whom);
    return whom * cos(angle) + direction * sin(angle);
}

Vector reflect(Vector v, Vector normal) { return v - 2 * dot(v, normal) * normal; }

#endif // VECTOR_H
