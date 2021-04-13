#ifndef VECTOR_H
#define VECTOR_H

#ifndef D
#define D 4
#endif

class Vector {
    double x[D];

  public:
    Vector() = default;

    Vector(const double val) : x(val), y(val), z(val) {}

    Vector(const Vector &v) : x(v.x), y(v.y), z(v.z) {}

    Vec
};

typedef Vector Point;

#endif // VECTOR_H
