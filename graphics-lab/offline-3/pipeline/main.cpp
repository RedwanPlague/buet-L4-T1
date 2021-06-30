#include "Floor.h"
#include "General.h"
#include "Light.h"
#include "Ray.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Vector.h"
#include "opengl.h"

#include "bitmap_image.hpp"

#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
namespace fs = filesystem;
typedef fs::path path;

int maxDepth;
vector<Object *> objects;
vector<Light> lights;

bool shadowOn, reflectionOn, refractionOn;

int imgWidth, imgHeight;
const path dir("io-files");

void loadData() {
    ifstream fin(dir / path("scene.txt"));

    fin >> maxDepth >> imgWidth;
    imgHeight = imgWidth;

    objects.push_back(new Floor(1000, 20));

    int no;
    fin >> no;
    for (int i = 0; i < no; i++) {
        string type;
        fin >> type;
        if (type == "sphere") {
            Sphere *s = new Sphere();
            fin >> *s;
            objects.push_back(s);
        }
        else if (type == "triangle") {
            Triangle *t = new Triangle();
            fin >> *t;
            objects.push_back(t);
        }
        else if (type == "general") {
            General *g = new General();
            fin >> *g;
            objects.push_back(g);
        }
    }

    int lo;
    fin >> lo;
    for (int i = 0; i < lo; i++) {
        Light l;
        fin >> l;
        lights.push_back(l);
    }

    fin.close();
}

void unloadData() {
    for (auto o : objects) {
        delete o;
    }
}

void capture() {
    cout << "generating image: ";
    cout.flush();
    clock_t stime = clock();

    double planeDist = (windowHeight / 2.0) / tan(pi / 180 * viewAngle / 2.0);
    double dx = (double)windowWidth / imgWidth;
    double dy = (double)windowHeight / imgHeight;

    Point topLeft = eye + lv * planeDist - rv * (windowWidth - dx) / 2 + uv * (windowHeight - dy) / 2;

    bitmap_image image(imgWidth, imgHeight);
    image.clear();

    Point leftStart = topLeft;
    for (int j = 0; j < imgHeight; j++, leftStart -= dy * uv) {
        Point mid = leftStart;
        for (int i = 0; i < imgWidth; i++, mid += dx * rv) {
            Ray ray(eye, mid - eye);
            Object *nearest = getNearest(ray);
            if (nearest) {
                Color color = nearest->trace(ray, maxDepth).transform();
                image.set_pixel(i, j, color.r, color.g, color.b);
            }
        }
    }

    image.save_image(dir / path("out.bmp"));

    clock_t etime = clock();
    cout << fixed << setprecision(2) << (etime - stime + 0.0) / CLOCKS_PER_SEC << "s taken" << endl;
}

int main(int argc, char **argv) {
    loadData();
    atexit(unloadData);
    opengl(&argc, argv);
    return 0;
}
