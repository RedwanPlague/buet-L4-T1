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
        memset(a, 0, sizeof(a));
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
        axis = axis / axis.norm();
        Vector c0 = rodrigues({1, 0, 0}, axis, angle);
        Vector c1 = rodrigues({0, 1, 0}, axis, angle);
        Vector c2 = rodrigues({0, 0, 1}, axis, angle);
        ret.a[0][0] = c0.x;
        ret.a[1][0] = c0.y;
        ret.a[2][0] = c0.z;
        ret.a[0][1] = c1.x;
        ret.a[1][1] = c1.y;
        ret.a[2][1] = c1.z;
        ret.a[0][2] = c2.x;
        ret.a[1][2] = c2.y;
        ret.a[2][2] = c2.z;
        return ret;
    }

    static Matrix viewer(Point eye, Vector look, Vector up) {
        Vector l = look - eye;
        l = l / l.norm();
        Vector r = cross(l, up);
        r = r / r.norm();
        Vector u = cross(r, l);

        Matrix T = translator(-eye);

        Matrix R = I;
        R.a[0][0] = r.x;
        R.a[0][1] = r.y;
        R.a[0][2] = r.z;
        R.a[1][0] = u.x;
        R.a[1][1] = u.y;
        R.a[1][2] = u.z;
        R.a[2][0] = -l.x;
        R.a[2][1] = -l.y;
        R.a[2][2] = -l.z;

        return R * T;
    }

    static Matrix projector(double fovY, double aspectRatio, double near, double far) {
        fovY *= pi / 180;
        double fovX = fovY * aspectRatio;

        Matrix ret(0);
        ret.a[0][0] = 1 / tan(fovX / 2);
        ret.a[1][1] = 1 / tan(fovY / 2);
        ret.a[2][2] = -(far + near) / (far - near);
        ret.a[2][3] = -(2 * far * near) / (far - near);
        ret.a[3][2] = -1;

        return ret;
    }

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
