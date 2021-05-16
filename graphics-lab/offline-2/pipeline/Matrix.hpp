#ifndef MATRIX_H
#define MATRIX_H

#include "Vector.hpp"
#include <cstring>
#include <iomanip>
#include <ostream>

#ifndef D
#define D 4
#endif

class Matrix {
    double a[D][D];

  public:
    Matrix() = default;

    Matrix(double val) {
        for (int i = 0; i < D; i++) {
            a[i][i] = val;
        }
    }

    Matrix(const Matrix &m) {
        for (int i = 0; i < D; i++) {
            for (int j = 0; j < D; j++) {
                a[i][j] = m.a[i][j];
            }
        }
    }

    Matrix operator+(const Matrix &m) const {
        Matrix ret;
        for (int i = 0; i < D; i++) {
            for (int j = 0; j < D; j++) {
                ret.a[i][j] = a[i][j] + m.a[i][j];
            }
        }
        return ret;
    }

    Matrix operator-(const Matrix &m) const {
        Matrix ret;
        for (int i = 0; i < D; i++) {
            for (int j = 0; j < D; j++) {
                ret.a[i][j] = a[i][j] - m.a[i][j];
            }
        }
        return ret;
    }

    Matrix operator*(const Matrix &m) const {
        Matrix ret;
        for (int i = 0; i < D; i++) {
            for (int j = 0; j < D; j++) {
                ret.a[i][j] = 0;
                for (int k = 0; k < D; k++) {
                    ret.a[i][j] += a[i][k] * m.a[k][j];
                }
            }
        }
        return ret;
    }

    Vector operator*(const Vector &v) const {
        double p[4];
        for (int i = 0; i < D; i++) {
            p[i] = a[i][0] * v.x + a[i][1] * v.y + a[i][2] * v.z + a[i][3];
        }
        return {p[0] / p[3], p[1] / p[3], p[2] / p[3]};
    }

    static Matrix I;

    static Matrix translator(Vector shift) {
        Matrix ret = I;
        ret.a[0][3] = shift.x;
        ret.a[1][3] = shift.y;
        ret.a[2][3] = shift.z;
        return ret;
    }

    static Matrix scalar(Vector scale) {
        Matrix ret = I;
        ret.a[0][0] = scale.x;
        ret.a[1][1] = scale.y;
        ret.a[2][2] = scale.z;
        return ret;
    }

    static Matrix rotator(double angle, Vector axis) {
        Matrix ret = I;
        Vector c1 = rodrigues({1, 0, 0}, axis, angle);
        Vector c2 = rodrigues({0, 1, 0}, axis, angle);
        Vector c3 = rodrigues({0, 0, 1}, axis, angle);
        ret.a[0][0] = c1.x;
        ret.a[1][0] = c1.y;
        ret.a[2][0] = c1.z;
        ret.a[0][1] = c2.x;
        ret.a[1][1] = c2.y;
        ret.a[2][1] = c2.z;
        ret.a[0][2] = c3.x;
        ret.a[1][2] = c3.y;
        ret.a[2][2] = c3.z;
        return ret;
    }

    static Matrix projector() { return I; }

    friend std::ostream &operator<<(std::ostream &out, const Matrix &m);
};

std::ostream &operator<<(std::ostream &out, const Matrix &m) {
    for (int i = 0; i < D; i++) {
        for (int j = 0; j < D; j++) {
            out << std::setw(5) << std::setprecision(2) << std::fixed << m.a[i][j] << ' ';
        }
        out << '\n';
    }
    return out;
}

Matrix Matrix::I = Matrix(1);

#endif // MATRIX_H
