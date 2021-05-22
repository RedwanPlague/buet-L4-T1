#include "Matrix.hpp"
#include "Shapes.hpp"
#include "Vector.hpp"
#include "bitmap_image.hpp"
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stack>
#include <string>

using namespace std;
namespace fs = filesystem;
typedef fs::path path;

const int precision = 7;

const path sub_dir = path("5");
const path in_dir = path("test-cases") / sub_dir;
const path out_dir = path("output") / sub_dir;

Point eye;
Vector look, up;
double fovY, aspectRatio, near, far;

void stage1() {
    ifstream fin(in_dir / path("scene.txt"));
    ofstream fout(out_dir / path("stage1.txt"));
    fout << setprecision(precision) << fixed;

    fin >> eye >> look >> up;
    fin >> fovY >> aspectRatio >> near >> far;

    stack<Matrix> stk;
    Matrix cur = Matrix::I;

    string command;
    while (fin >> command) {
        if (command == "triangle") {
            Triangle triangle;
            fin >> triangle;
            triangle.transform(cur);
            fout << triangle << endl;
        }
        else if (command == "translate") {
            Vector shift;
            fin >> shift;
            cur = cur * Matrix::translator(shift);
        }
        else if (command == "scale") {
            Point scale;
            fin >> scale;
            cur = cur * Matrix::scalar(scale);
        }
        else if (command == "rotate") {
            double angle;
            Vector axis;
            fin >> angle >> axis;
            cur = cur * Matrix::rotator(angle, axis);
        }
        else if (command == "push") {
            stk.push(cur);
        }
        else if (command == "pop") {
            cur = stk.top();
            stk.pop();
        }
        else if (command == "end") {
            break;
        }
    }

    fin.close();
    fout.close();
}

void stage2() {
    ifstream fin(out_dir / path("stage1.txt"));
    ofstream fout(out_dir / path("stage2.txt"));
    fout << setprecision(precision) << fixed;

    Matrix V = Matrix::viewer(eye, look, up);

    Triangle triangle;
    while (fin >> triangle) {
        triangle.transform(V);
        fout << triangle << endl;
    }

    fin.close();
    fout.close();
}

void stage3() {
    ifstream fin(out_dir / path("stage2.txt"));
    ofstream fout(out_dir / path("stage3.txt"));
    fout << setprecision(precision) << fixed;

    Matrix P = Matrix::projector(fovY, aspectRatio, near, far);

    Triangle triangle;
    while (fin >> triangle) {
        triangle.transform(P);
        fout << triangle << endl;
    }

    fin.close();
    fout.close();
}

double **make_buffer(int W, int H, double mx) {
    double **buff = new double *[W];
    for (int i = 0; i < W; i++) {
        buff[i] = new double[H];
        fill(buff[i], buff[i] + H, mx + eps);
    }
    return buff;
}

/* void write_buffer(double **buff, int W, int H, double mx) { */
/*     ofstream fout(out_dir / path("z_buffer.txt")); */
/*     fout << setprecision(precision) << fixed; */
/*     // output z_buff here */
/*     fout.close(); */
/* } */

void clear_buffer(double **buff, int W) {
    for (int i = 0; i < W; i++) {
        delete[] buff[i];
    }
    delete[] buff;
}

// returns if polygon should be drawn
bool make_fit(int &a, int &b, int l /*inclusive*/, int r /*exclusive*/) {
    if (a >= r || b < l) { // polygon completely outside of viewing area
        return false;
    }
    a = (a < l) ? l : ((a < r) ? a : r - 1);
    b = (b < l) ? l : ((b < r) ? b : r - 1);
    return true;
}

void stage4() {
    ifstream confin(in_dir / path("config.txt"));

    int screen_width, screen_height;
    double l, b, z_front, z_rear;

    confin >> screen_width >> screen_height;
    confin >> l >> b >> z_front >> z_rear;

    double r = -l, t = -b;

    confin.close();

    double dx = (r - l) / screen_width;
    double dy = (t - b) / screen_height;
    double left = l + dx / 2;
    double top = t - dy / 2;

    bitmap_image image(screen_width, screen_height);
    double **z_buff = make_buffer(screen_width, screen_height, z_rear);

    ifstream fin(out_dir / path("stage3.txt"));
    Triangle triangle;

    while (fin >> triangle) {
        RandomColor color;

        int top_cell = round((top - triangle.maxy()) / dy);
        int bottom_cell = round((top - triangle.miny()) / dy);
        if (!make_fit(top_cell, bottom_cell, 0, screen_height)) {
            continue;
        }

        double ys = top - top_cell * dy;
        for (int yi = top_cell; yi <= bottom_cell; yi++, ys -= dy) {

            auto cuty = triangle.cut_at_y(ys);
            Point a = cuty.first, b = cuty.second;

            int left_cell = round((a.x - left) / dx);
            int right_cell = round((b.x - left) / dx);
            if (!make_fit(left_cell, right_cell, 0, screen_width)) {
                continue;
            }

            double coeff = (a.z - b.z) / (a.x - b.x);
            double xs = left + left_cell * dx;
            for (int xi = left_cell; xi <= right_cell; xi++, xs += dx) {

                double zs = a.z + (xs - a.x) * coeff;
                if (z_front < zs + eps && zs < z_buff[xi][yi]) {
                    z_buff[xi][yi] = zs;
                    image.set_pixel(xi, yi, color.r, color.g, color.b);
                }
            }
        }
    }
    fin.close();

    image.save_image(out_dir / path("out.bmp"));
    /* write_buffer(z_buff, screen_width, screen_height, z_rear); */
    clear_buffer(z_buff, screen_width);
}

int main() {
    if (!(fs::is_directory(out_dir) && fs::exists(out_dir))) {
        fs::create_directories(out_dir);
    }

    /* srand(time(0)); */

    clock_t start = clock();
    stage1();
    stage2();
    stage3();
    stage4();
    clock_t end = clock();
    cout << "time needed: " << (end - start + 0.0) / CLOCKS_PER_SEC << " seconds" << endl;

    return 0;
}
