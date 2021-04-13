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
            for (int j = 0; j < D; j++) {
                a[i][j] = val / (i + j);
            }
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

    Matrix operator*(const Vector &v) const {
        Vector ret;

        return ret;
    }

    friend std::ostream &operator<<(std::ostream &out, const Matrix &m);
};

std::ostream &operator<<(std::ostream &out, const Matrix &m) {
    for (int i = 0; i < D; i++) {
        for (int j = 0; j < D; j++) {
            out << std::fixed << std::setw(7) << std::setprecision(2) << m.a[i][j] << ' ';
        }
        out << '\n';
    }
    return out;
}

#endif // MATRIX_H
