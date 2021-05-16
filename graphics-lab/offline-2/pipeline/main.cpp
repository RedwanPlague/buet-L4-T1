#include "Matrix.hpp"
#include "Shapes.hpp"
#include "Vector.hpp"
#include <fstream>
#include <iostream>
#include <stack>
#include <string>

using namespace std;

const string in_dir = "test-cases/1/";
const string out_dir = "output/1/";

Point eye;
Vector look, up;
double fovY, aspectRatio, nearDist, farDist;

void stage1() {
    ifstream fin(in_dir + "scene.txt");
    ofstream fout(out_dir + "stage1");

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
}

int main() {

    Vector a({1, 2, 3});
    cout << a << endl;

    return 0;
}
