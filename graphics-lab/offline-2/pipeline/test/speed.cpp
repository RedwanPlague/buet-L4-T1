#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

using namespace std;

void with_array(int W, int H, double val, int loop) {
    clock_t start = clock();

    double **a = new double *[W];
    for (int i = 0; i < W; i++) {
        a[i] = new double[H];
        fill(a[i], a[i] + H, val);
    }

    for (int k = 0; k < loop; k++) {
        for (int i = 0; i < W; i++) {
            for (int j = 0; j < H; j++) {
                a[i][j] = a[i][j] + (i + 0.0) / j;
            }
        }
    }

    for (int i = 0; i < W; i++) {
        delete[] a[i];
    }
    delete[] a;

    clock_t end = clock();
    cout << (end - start + 0.0) / CLOCKS_PER_SEC * 1000 << endl;
}

void with_vector(int W, int H, double val, int loop) {
    clock_t start = clock();

    vector<vector<double>> a(W, vector<double>(H, val));

    for (int k = 0; k < loop; k++) {
        for (int i = 0; i < W; i++) {
            for (int j = 0; j < H; j++) {
                a[i][j] = a[i][j] + (i + 0.0) / j;
            }
        }
    }

    clock_t end = clock();
    cout << (end - start + 0.0) / CLOCKS_PER_SEC * 1000 << endl;
}

int main() {
    int W = 500, H = 500, loop = 100;
    double val = 5.33;

    with_array(W, H, val, loop);
    with_vector(W, H, val, loop);

    return 0;
}
