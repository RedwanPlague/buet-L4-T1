#ifndef CAMERA_H
#define CAMERA_H

#include "Light.h"
#include "Object.h"
#include "Vector.h"

#include <GL/glut.h>
#include <cstdlib>
#include <iostream>
#include <vector>

#define __unused __attribute__((unused))

extern std::vector<Object *> objects;
extern std::vector<Light> lights;

Point eye;         // camera position
Vector uv, lv, rv; // up vector, look vector, right vector

double cameraAngleInc, vInc;

const int windowWidth = 600, windowHeight = 600;
const double viewAngle = 80;

extern bool shadowOff, reflectionOff;

void initialize() {
    eye = Point(0, 0, 50);
    uv = Vector(0, 1, 0);
    lv = Vector(0, 0, -1);
    rv = Vector(1, 0, 0);

    cameraAngleInc = 5;
    vInc = 10;
    shadowOff = reflectionOff = false;
}

extern void capture();

void keyboardListener(unsigned char key, __unused int x, __unused int y) {
    switch (key) {
    case '1': // look left
        lv = orthogonalRotate(lv, uv, cameraAngleInc);
        rv = orthogonalRotate(rv, uv, cameraAngleInc);
        break;
    case '2': // look right
        lv = orthogonalRotate(lv, uv, -cameraAngleInc);
        rv = orthogonalRotate(rv, uv, -cameraAngleInc);
        break;
    case '3': // look up
        uv = orthogonalRotate(uv, rv, cameraAngleInc);
        lv = orthogonalRotate(lv, rv, cameraAngleInc);
        break;
    case '4': // look down
        uv = orthogonalRotate(uv, rv, -cameraAngleInc);
        lv = orthogonalRotate(lv, rv, -cameraAngleInc);
        break;
    case '5': // tilt clockwise
        rv = orthogonalRotate(rv, lv, cameraAngleInc);
        uv = orthogonalRotate(uv, lv, cameraAngleInc);
        break;
    case '6': // tilt anticlockwise
        rv = orthogonalRotate(rv, lv, -cameraAngleInc);
        uv = orthogonalRotate(uv, lv, -cameraAngleInc);
        break;
    case 'c': cameraAngleInc = 6 - cameraAngleInc; break;
    case 'v': vInc = 11 - vInc; break;
    case 's':
        shadowOff = !shadowOff;
        capture();
        break;
    case 'r':
        reflectionOff = !reflectionOff;
        capture();
        break;
    case '0': capture(); break;
    case 'q': exit(EXIT_SUCCESS); break;
    default: break;
    }
    glutPostRedisplay();
}

void specialKeyListener(int key, __unused int x, __unused int y) {
    switch (key) {
    case GLUT_KEY_UP: eye = eye + lv * vInc; break;
    case GLUT_KEY_DOWN: eye = eye - lv * vInc; break;
    case GLUT_KEY_RIGHT: eye = eye + rv * vInc; break;
    case GLUT_KEY_LEFT: eye = eye - rv * vInc; break;
    case GLUT_KEY_PAGE_UP: eye = eye + uv * vInc; break;
    case GLUT_KEY_PAGE_DOWN: eye = eye - uv * vInc; break;
    default: break;
    }
    glutPostRedisplay();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(eye.x, eye.y, eye.z, eye.x + lv.x, eye.y + lv.y, eye.z + lv.z, uv.x, uv.y, uv.z);
    glMatrixMode(GL_MODELVIEW);

    for (auto o : objects) {
        o->draw();
    }
    for (auto l : lights) {
        l.draw();
    }

    glutSwapBuffers();
}

void init() {
    initialize();

    glClearColor(0, 0, 0, 0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(80, 1, 0.1, 1000.0);
    // field of view in the Y (vertically)
    // aspect ratio that determines the field of view in the X direction (horizontally)
    // near distance
    // far distance
}

void opengl(int *argc, char **argv) {
    glutInit(argc, argv);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB); // Depth, Double buffer, RGB color

    glutCreateWindow("Offline 3");

    init();

    glEnable(GL_DEPTH_TEST); // enable Depth Testing

    glutDisplayFunc(display); // display callback function

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);

    glutMainLoop(); // The main loop of OpenGL
}

#endif // CAMERA_H
