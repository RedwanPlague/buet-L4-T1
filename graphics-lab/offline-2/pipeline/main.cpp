#include "Matrix.hpp"
#include "Vector.hpp"
#include <iostream>

using namespace std;

int main() {
    cout << Matrix::I << endl;
    Vector a(0, 0, 0);
    cout << a << endl;
    Matrix t = Matrix::translator({0.1, 0.2, 0});
    Matrix it = Matrix::translator({-0.1, -0.2, 0});
    /* cout << t << endl; */
    /* cout << t * a << endl; */
    Matrix s = Matrix::scalar({1, 2, 1});
    Matrix is = Matrix::scalar({2, 0.5, 1});
    /* cout << t * s << endl; */
    /* cout << t * s * a << endl; */
    cout << is * it * s * t * a << endl;
    /* cout << t * s * a << endl; */
    return 0;
}
