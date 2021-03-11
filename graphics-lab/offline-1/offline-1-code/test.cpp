#include <GL/glut.h>
#include <cmath>

#define __unused __attribute__((unused))

const double pi = acos(-1);

const int rangeX = 250;
const int rangeY = 250;
const int rangeZ = 100;

const int unit = 10;
const int unitsX = rangeX / unit;
const int unitsY = rangeY / unit;

bool shouldDrawGrid;
bool shouldDrawAxes;
double angleUse, speed;
double cameraAngleInc;
double uvInc, lvInc, rvInc;

struct Point {
    double x, y, z;

    Point() = default;
    Point(double x, double y, double z) : x(x), y(y), z(z) {}

    Point operator + (const Point &p) const {
        return {x + p.x, y + p.y, z + p.z};
    }
    Point operator - (const Point &p) const {
        return {x - p.x, y - p.y, z - p.z};
    }
    Point operator - () const {
        return {-x, -y, -z};
    }
    Point operator * (const double m) const {
        return {x * m, y * m, z * m};
    }
    Point operator / (const double m) const {
        return {x / m, y / m, z / m};
    }
};

typedef Point Vector;

Point pos;
Vector uv, lv, rv; // up vector, look vector, right vector

Vector orthogonalRotate (Vector whom, Vector direction, double angle) {
    return whom * cos(angle) + direction * sin(angle);
}

void drawAxes() {
    if (shouldDrawAxes) {
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINES);
        {
            glVertex3f(rangeX, 0, 0);
            glVertex3f(-rangeX, 0, 0);

            glVertex3f(0, -rangeY, 0);
            glVertex3f(0, rangeY, 0);

            glVertex3f(0, 0, rangeZ);
            glVertex3f(0, 0, -rangeZ);
        }
        glEnd();
    }
}

void drawGrid() {
    if (shouldDrawGrid) {
        glColor3f(0.3, 0.3, 0.3); // grey
        glBegin(GL_LINES);
        {
            for (int i = -unitsX+5; i <= unitsX-5; i++) {
                // lines parallel to Y-axis
                glVertex3f(i * 10, -rangeY, 0);
                glVertex3f(i * 10, rangeY, 0);
            }
            for (int i = -unitsY; i <= unitsY; i++) {
                // lines parallel to X-axis
                glVertex3f(-rangeX, i * 10, 0);
                glVertex3f(rangeX, i * 10, 0);
            }
        }
        glEnd();
    }
}

void drawSquare(double a, bool filled = false) {
    glBegin(filled ? GL_QUADS : GL_LINE_LOOP);
    {
        glVertex3f(a, a, 0);
        glVertex3f(a, -a, 0);
        glVertex3f(-a, -a, 0);
        glVertex3f(-a, a, 0);
    }
    glEnd();
}

void drawCircle(double radius, int segments, bool filled = false) {
    glBegin(filled ? GL_POLYGON : GL_LINE_LOOP);
    {
        for (int i=0; i<segments; i++) {
            double theta = (double) i / segments * 2 * pi;
            double x = radius * cos(theta);
            double y = radius * sin(theta);
            glVertex3f(x, y, 0);
        }
    }
    glEnd();
}

void drawCone(double radius, double height, int segments) {
    Point prev(radius, 0, 0);

    // draw triangles to build cone
    for (int i = 1; i <= segments; i++) {
        // create shading effect
        double fraction = (double) i / segments;
        double shade = (2*i < segments ? 2 * fraction : 2 * (1 - fraction));
        glColor3f(shade, shade, shade);

        double theta = fraction * 2 * pi;
        Point cur(radius * cos(theta), radius * sin(theta), 0);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0, 0, height);
            glVertex3f(prev.x, prev.y, prev.z);
            glVertex3f(cur.x, cur.y, cur.z);
        }
        glEnd();

        prev = cur;
    }
}

void drawSphere(double radius, int slices, int stacks) {
    struct Point points[100][100];
    int i, j;
    double h, r;
    // generate points
    for (i = 0; i <= stacks; i++) {
        h = radius * sin(((double) i / (double) stacks) * (pi / 2));
        r = radius * cos(((double) i / (double) stacks) * (pi / 2));
        for (j = 0; j <= slices; j++) {
            points[i][j].x = r * cos(((double) j / (double) slices) * 2 * pi);
            points[i][j].y = r * sin(((double) j / (double) slices) * 2 * pi);
            points[i][j].z = h;
        }
    }
    // draw quads using generated points
    for (i = 0; i < stacks; i++) {
        glColor3f((double) i / (double) stacks, (double) i / (double) stacks, (double) i / (double) stacks);
        for (j = 0; j < slices; j++) {
            glBegin(GL_QUADS);
            {
                // upper hemisphere
                glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
                glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
                glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
                glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
                // lower hemisphere
                glVertex3f(points[i][j].x, points[i][j].y, -points[i][j].z);
                glVertex3f(points[i][j + 1].x, points[i][j + 1].y, -points[i][j + 1].z);
                glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, -points[i + 1][j + 1].z);
                glVertex3f(points[i + 1][j].x, points[i + 1][j].y, -points[i + 1][j].z);
            }
            glEnd();
        }
    }
}

void drawSS() {
    glColor3f(1, 0, 0);
    drawSquare(20);

    glRotatef(angleUse, 0, 0, 1);
    glTranslatef(110, 0, 0);
    glRotatef(2 * angleUse, 0, 0, 1);
    glColor3f(0, 1, 0);
    drawSquare(15);

    glPushMatrix();
    {
        glRotatef(angleUse, 0, 0, 1);
        glTranslatef(60, 0, 0);
        glRotatef(2 * angleUse, 0, 0, 1);
        glColor3f(0, 0, 1);
        drawSquare(10);
    }
    glPopMatrix();

    glRotatef(3 * angleUse, 0, 0, 1);
    glTranslatef(40, 0, 0);
    glRotatef(4 * angleUse, 0, 0, 1);
    glColor3f(1, 1, 0);
    drawSquare(5);
}

void keyboardListener(unsigned char key, __unused int x, __unused int y) {
    Vector newLV = lv, newRV = rv, newUV = uv;
    switch (key) {
        case '1': // look left
            newLV = orthogonalRotate(lv, -rv, cameraAngleInc);
            newRV = orthogonalRotate(rv, lv, cameraAngleInc);
            break;
        case '2': // look right
            newLV = orthogonalRotate(lv, rv, cameraAngleInc);
            newRV = orthogonalRotate(rv, -lv, cameraAngleInc);
            break;
        case '3': // look up
            newUV = orthogonalRotate(uv, -lv, cameraAngleInc);
            newLV = orthogonalRotate(lv, uv, cameraAngleInc);
            break;
        case '4': // look down
            newUV = orthogonalRotate(uv, lv, cameraAngleInc);
            newLV = orthogonalRotate(lv, -uv, cameraAngleInc);
            break;
        case '5': // tilt clockwise
            newRV = orthogonalRotate(rv, -uv, cameraAngleInc);
            newUV = orthogonalRotate(uv, rv, cameraAngleInc);
            break;
        case '6': // tilt anticlockwise
            newRV = orthogonalRotate(rv, uv, cameraAngleInc);
            newUV = orthogonalRotate(uv, -rv, cameraAngleInc);
            break;
        default:
            break;
    }
    lv = newLV;
    rv = newRV;
    uv = newUV;
}

void specialKeyListener(int key, __unused int x, __unused int y) {
    switch (key) {
        case GLUT_KEY_UP:
            pos = pos + lv * lvInc;
            break;
        case GLUT_KEY_DOWN:
            pos = pos - lv * lvInc;
            break;
        case GLUT_KEY_RIGHT:
            pos = pos + rv * rvInc;
            break;
        case GLUT_KEY_LEFT:
            pos = pos - rv * rvInc;
            break;
        case GLUT_KEY_PAGE_UP:
            pos = pos + uv * uvInc;
            break;
        case GLUT_KEY_PAGE_DOWN:
            pos = pos - uv * uvInc;
            break;
        default:
            break;
    }
}

void mouseListener(int button, int state, int x, int y) {
    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN) {
                speed += 0.01;
            }
            break;
        case GLUT_RIGHT_BUTTON:
            if (state == GLUT_DOWN) {
                speed -= 0.01;
            }
            break;
        default:
            break;
    }
}

void display() {

    // clear the display
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glClearColor(0, 0, 0, 0);    // color black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /********************
    / set-up camera here
    ********************/
    // load the correct matrix -- MODEL-VIEW matrix
    glMatrixMode(GL_MODELVIEW);

    // initialize the matrix
    glLoadIdentity();

    // now give three info
    // 1. where is the camera (viewer)?
    // 2. where is the camera looking?
    // 3. Which direction is the camera's UP direction?

    // gluLookAt(100,100,100,	0,0,0,	0,0,1);
    // gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
    gluLookAt(pos.x, pos.y, pos.z, pos.x + lv.x, pos.y + lv.y, pos.z + lv.z, uv.x, uv.y, uv.z);


    // again select MODEL-VIEW
    glMatrixMode(GL_MODELVIEW);


    /****************************
    / Add your objects from here
    ****************************/
    // add objects

    drawAxes();
    drawGrid();

    // drawSS();

    drawCone(20, 50, 50);
    glTranslatef(100, 0, 0);
    drawCone(20, 50, 50);
    glTranslatef(-200, 0, 0);
    drawCone(20, 50, 50);

    // ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
    glutSwapBuffers();
}

void animate() {
    angleUse += speed;
    // codes for any changes in Models, Camera
    glutPostRedisplay();
}

void init() {
    // codes for initialization
    shouldDrawGrid = true;
    shouldDrawAxes = true;
    angleUse = 0;

    pos = Point(0, 0, 200);
    uv = Vector(0, 1, 0);
    lv = Vector(0, 0, -1);
    rv = Vector(1, 0, 0);

    cameraAngleInc = 0.1;
    uvInc = lvInc = rvInc = 1;

    // clear the screen
    glClearColor(0, 0, 0, 0);

    /************************
    / set-up projection here
    ************************/
    // load the PROJECTION matrix
    glMatrixMode(GL_PROJECTION);

    // initialize the matrix
    glLoadIdentity();

    // give PERSPECTIVE parameters
    gluPerspective(80, 1, 1, 1000.0);
    // field of view in the Y (vertically)
    // aspect ratio that determines the field of view in the X direction (horizontally)
    // near distance
    // far distance
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);    // Depth, Double buffer, RGB color

    glutCreateWindow("My OpenGL Program");

    init();

    glEnable(GL_DEPTH_TEST);    // enable Depth Testing

    glutDisplayFunc(display);    // display callback function
    glutIdleFunc(animate);        // what you want to do in the idle time (when no drawing is occurring)

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMouseFunc(mouseListener);

    glutMainLoop();        // The main loop of OpenGL

    return 0;
}
