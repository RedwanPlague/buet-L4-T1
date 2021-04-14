#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>
#include <istream>
#include <ostream>

struct Vector {
    double x, y, z;

  public:
    Vector() = default;

    Vector(double x, double y, double z) : x(x), y(y), z(z) {}

    Vector(const Vector &v) : x(v.x), y(v.y), z(v.z) {}

    Vector operator+(const Vector &v) const { return {x + v.x, y + v.y, z + v.z}; }
    Vector operator-(const Vector &v) const { return {x - v.x, y - v.y, z - v.z}; }
    Vector operator*(double m) { return {m * x, m * y, m * z}; }
    Vector operator/(double m) { return {m / x, m / y, m / z}; }
    friend Vector operator*(double m, const Vector &v);
    friend Vector operator/(double m, const Vector &v);
    Vector operator-() const { return {-x, -y, -z}; }

    double norm() const { return sqrt(x * x + y * y + z * z); }
    double dot(const Vector &v) const { return x * v.x + y * v.y + z * v.z; }
    Vector cross(const Vector &v) const { return {y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x}; }

    friend std::istream &operator>>(std::istream &in, Vector &v);
    friend std::ostream &operator<<(std::ostream &out, const Vector &v);
};

Vector operator/(double m, const Vector &v) { return {m / v.x, m / v.y, m / v.z}; }
Vector operator*(double m, const Vector &v) { return {m * v.x, m * v.y, m * v.z}; }

std::istream &operator>>(std::istream &in, Vector &v) {
    in >> v.x >> v.y >> v.z;
    return in;
}

std::ostream &operator<<(std::ostream &out, const Vector &v) {
    out << '(' << v.x << ", " << v.y << ", " << v.z << ')';
    return out;
}

typedef Vector Point;

#endif // VECTOR_H
