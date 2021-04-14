#include "Matrix.hpp"
#include "Vector.hpp"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

ifstream fin;
ofstream fout;

struct View {
    Point eye;
    Vector look, up;
    double fovY, aspectRatio, nearDist, farDist;
} view;

int main() {
    fin.open("test-cases/1/scene.txt");

    return 0;
}
