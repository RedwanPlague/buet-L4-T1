#ifndef COLOR_H
#define COLOR_H

#include <algorithm>
#include <istream>

double clip(double a) { return std::min(a, 1.0); }

struct Color {
    double r, g, b;

    Color() = default;
    Color(double r, double g, double b) : r(r), g(g), b(b) {}

    Color operator+(const Color &c) const { return {clip(r + c.r), clip(g + c.g), clip(b + c.b)}; }
    Color operator*(const Color &c) const { return {r * c.r, g * c.g, b * c.b}; }
    Color operator*(double m) const { return {r * m, g * m, b * m}; }
    Color operator/(double m) const { return {r / m, g / m, b / m}; }

    Color &operator+=(const Color &c) { return *this = *this + c; }
};

Color operator*(double m, const Color &c) { return {m * c.r, m * c.g, m * c.b}; }

std::istream &operator>>(std::istream &in, Color &c) {
    in >> c.r >> c.g >> c.b;
    return in;
}

#endif // COLOR_H
