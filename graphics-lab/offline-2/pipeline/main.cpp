#include "Matrix.hpp"
#include "Shapes.hpp"
#include "Vector.hpp"
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

const path sub_dir = path("4");
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
    stk.push(Matrix::I);

    string command;
    while (fin >> command) {
        if (command == "triangle") {
            Triangle triangle;
            fin >> triangle;
            triangle.transform(stk.top());
            fout << triangle << endl;
        }
        else if (command == "translate") {
            Vector shift;
            fin >> shift;
            Matrix top = stk.top();
            stk.pop();
            stk.push(top * Matrix::translator(shift));
        }
        else if (command == "scale") {
            Point scale;
            fin >> scale;
            Matrix top = stk.top();
            stk.pop();
            stk.push(top * Matrix::scalar(scale));
        }
        else if (command == "rotate") {
            double angle;
            Vector axis;
            fin >> angle >> axis;
            Matrix top = stk.top();
            stk.pop();
            stk.push(top * Matrix::rotator(angle, axis));
        }
        else if (command == "push") {
            stk.push(stk.top());
        }
        else if (command == "pop") {
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

int main() {
    if (!(fs::is_directory(out_dir) && fs::exists(out_dir))) {
        fs::create_directories(out_dir);
    }

    stage1();
    stage2();
    stage3();

    return 0;
}
