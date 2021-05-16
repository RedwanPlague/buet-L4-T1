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

const path sub_dir = path("3");
const path in_dir = path("test-cases") / sub_dir;
const path out_dir = path("output") / sub_dir;

Point eye;
Vector look, up;
double fovY, aspectRatio, nearDist, farDist;

void stage1() {
    ifstream fin(in_dir / path("scene.txt"));
    ofstream fout(out_dir / path("stage1.txt"));
    fout << setprecision(precision) << fixed;

    fin >> eye >> look >> up;
    fin >> fovY >> aspectRatio >> nearDist >> farDist;

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

int main() {
    if (!(fs::is_directory(out_dir) && fs::exists(out_dir))) {
        fs::create_directories(out_dir);
    }

    stage1();

    return 0;
}
