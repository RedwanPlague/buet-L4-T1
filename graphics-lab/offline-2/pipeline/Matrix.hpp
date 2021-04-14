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
        double x = a[0][0] * v.x + a[0][1] * v.y + a[0][2] * v.z + a[0][3];
        double y = a[1][0] * v.x + a[1][1] * v.y + a[1][2] * v.z + a[1][3];
        double z = a[2][0] * v.x + a[2][1] * v.y + a[2][2] * v.z + a[2][3];
        double d = a[3][0] * v.x + a[3][1] * v.y + a[3][2] * v.z + a[3][3];
        return {x / d, y / d, z / d};
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

    /* static Matrix rotator(double angle, Vector axis) { */
    /*     Matrix ret = I; */
    /*     ret.a[0][0] = scale.x; */
    /*     ret.a[1][1] = scale.y; */
    /*     ret.a[2][2] = scale.z; */
    /*     return ret; */
    /* } */

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
