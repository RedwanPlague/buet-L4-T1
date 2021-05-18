#include "Matrix.hpp"
#include "Shapes.hpp"
#include "Vector.hpp"
#include "bitmap_image.hpp"
#include <algorithm>
#include <cassert>
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

const path sub_dir = path("3");
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

void make_fit(int &x, int l /*inclusive*/, int r /*exclusive*/) { x = (x < l) ? l : ((x < r) ? x : r - 1); }

void stage4() {
    ifstream confin(in_dir / path("config.txt"));

    int screen_width, screen_height;
    double left, bottom, z_front, z_rear;

    confin >> screen_width >> screen_height;
    confin >> left >> bottom >> z_front >> z_rear;

    double right = -left, top = -bottom;

    confin.close();

    double **z_buff = new double *[screen_width];
    for (int i = 0; i < screen_width; i++) {
        z_buff[i] = new double[screen_height];
        fill(z_buff[i], z_buff[i] + screen_height, z_rear + 1e-7);
    }

    double dx = (right - left) / (screen_width - 1);
    double dy = (top - bottom) / (screen_height - 1);

    bitmap_image image(screen_width, screen_height);

    ifstream fin(out_dir / path("stage3.txt"));
    Triangle triangle;
    while (fin >> triangle) {
        RandomColor color;
        int top_scan = round((top - triangle.maxy()) / dy);
        int bottom_scan = round((top - triangle.miny()) / dy);
        make_fit(top_scan, 0, screen_height);
        make_fit(bottom_scan, 0, screen_height);
        double ys = top - top_scan * dy;
        for (int yi = top_scan; yi <= bottom_scan; yi++, ys -= dy) {
            auto cuty = triangle.cut_at_y(ys);
            Point a = cuty.first, b = cuty.second;
            if (a.x > b.x) {
                swap(a, b);
            }
            double coeff = (a.z - b.z) / (a.x - b.x);
            int left_scan = round((a.x - left) / dx);
            int right_scan = round((b.x - left) / dx);
            make_fit(left_scan, 0, screen_width);
            make_fit(right_scan, 0, screen_width);
            double xp = left + left_scan * dx;
            for (int xi = left_scan; xi <= right_scan; xi++, xp += dx) {
                double zp = (xp - a.x) * coeff;
                if (zp < z_buff[xi][yi]) {
                    z_buff[xi][yi] = zp;
                    image.set_pixel(xi, yi, color.r, color.g, color.b);
                }
            }
        }
    }
    fin.close();

    image.save_image(out_dir / path("out.bmp"));

    /* ofstream fout(out_dir / path("z_buffer.txt")); */
    /* fout << setprecision(precision) << fixed; */
    // output z_buff here
    /* fout.close(); */

    for (int i = 0; i < screen_width; i++) {
        delete[] z_buff[i];
    }
    delete[] z_buff;
}

int main() {
    if (!(fs::is_directory(out_dir) && fs::exists(out_dir))) {
        fs::create_directories(out_dir);
    }

    clock_t start = clock();
    /* stage1(); */
    /* stage2(); */
    /* stage3(); */
    stage4();
    clock_t end = clock();
    cout << "time needed: " << (end - start + 0.0) / CLOCKS_PER_SEC << " seconds" << endl;

    return 0;
}
