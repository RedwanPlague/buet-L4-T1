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

const path in_base = path("test-cases");
const path out_base = path("output");
path in_dir, out_dir;

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

double **make_buffer(int H, int W, double mx) {
    double **buff = new double *[H];
    for (int i = 0; i < H; i++) {
        buff[i] = new double[W];
        fill(buff[i], buff[i] + W, mx + eps);
    }
    return buff;
}

void write_buffer(double **buff, int H, int W, double mx) {
    ofstream fout(out_dir / path("z_buffer.txt"));
    fout << setprecision(6) << fixed;
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            if (buff[i][j] < mx + eps) {
                fout << buff[i][j] << '\t';
            }
        }
        fout << endl;
    }
    fout.close();
}

void clear_buffer(double **buff, int H) {
    for (int i = 0; i < H; i++) {
        delete[] buff[i];
    }
    delete[] buff;
}

// returns if polygon should be drawn
bool make_fit(int &a, int &b, int l /*inclusive*/, int r /*exclusive*/) {
    if (a >= r || b < l) { // polygon completely outside of viewing area
        return false;
    }
    a = clamp(a, l, r - 1);
    b = clamp(b, l, r - 1);
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

    double **z_buff = make_buffer(screen_height, screen_width, z_rear);
    bitmap_image image(screen_width, screen_height);
    image.clear();

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
        for (int i = top_cell; i <= bottom_cell; i++, ys -= dy) {

            auto cuty = triangle.cut_at_y(ys);
            Point a = cuty.first, b = cuty.second;

            int left_cell = round((a.x - left) / dx);
            int right_cell = round((b.x - left) / dx);
            if (!make_fit(left_cell, right_cell, 0, screen_width)) {
                continue;
            }

            double coeff = (a.z - b.z) / (a.x - b.x);
            double xs = left + left_cell * dx;
            double zs = a.z + (xs - a.x) * coeff;
            double dz = dx * coeff;
            for (int j = left_cell; j <= right_cell; j++, zs += dz) {

                if (z_front < zs + eps && zs < z_buff[i][j]) {
                    z_buff[i][j] = zs;
                    image.set_pixel(j, i, color.r, color.g, color.b);
                }
            }
        }
    }
    fin.close();

    image.save_image(out_dir / path("out.bmp"));
    write_buffer(z_buff, screen_height, screen_width, z_rear);
    clear_buffer(z_buff, screen_height);
}

void fix_dir(string dir) {
    path sub_dir = path(dir);
    in_dir = in_base / sub_dir;
    out_dir = out_base / sub_dir;
    if (!(fs::is_directory(out_dir) && fs::exists(out_dir))) {
        fs::create_directories(out_dir);
    }
}

int main() {
    srand(time(0));

    string dirs[] = {"1", "2", "3", "4"};

    for (auto dir : dirs) {
        fix_dir(dir);
        clock_t start = clock();
        stage1();
        stage2();
        stage3();
        stage4();
        clock_t end = clock();
        cout << "/" << dir << "/ ";
        cout << "time needed: " << 1000.0 * (end - start + 0.0) / CLOCKS_PER_SEC << " ms" << endl;
    }

    return 0;
}
