#include <GL/glut.h>
#include <cmath>
#include <cstdio>

#define __unused __attribute__((unused))

const double pi = acos(-1);

const double wholeRight = 30;
const double wholeLeft = -30;
const double halfUp = 30;
const double halfDown = -30;
const double rodUp = 30;
const double rodDown = -30;
const double rotateRight = 60;
const double rotateLeft = -60;

const int sliceLimit = 100;

struct Point {
    double x, y, z;

    Point() = default;

    Point(double x, double y, double z) : x(x), y(y), z(z) {}

    Point operator+(const Point &p) const {
        return {x + p.x, y + p.y, z + p.z};
    }

    Point operator-(const Point &p) const {
        return {x - p.x, y - p.y, z - p.z};
    }

    Point operator-() const {
        return {-x, -y, -z};
    }

    Point operator*(const double m) const {
        return {x * m, y * m, z * m};
    }

    Point operator/(const double m) const {
        return {x / m, y / m, z / m};
    }
};
typedef Point Vector;

Vector cross(const Vector &a, const Vector &b) {
    return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

Point pos;
Vector uv, lv, rv; // up vector, look vector, right vector

bool shouldDrawAxes;
double cameraAngleInc, gunAngleInc;
double wholeAngle, halfAngle, rodAngle, rotateAngle;
double uvInc, lvInc, rvInc;

double bigRadius, smallRadius, rodLength, outRadius;
int sliceCount;
double targetSize, targetDist, hitSize;

Point hits[1000];
int hitCount;

void initialize() {
    pos = Point(100, 0, 200);
    uv = Vector(0, 1, 0);
    lv = Vector(0, 0, -1);
    rv = Vector(1, 0, 0);

    shouldDrawAxes = true;
    cameraAngleInc = 1;
    gunAngleInc = 1;
    wholeAngle = halfAngle = rodAngle = rotateAngle = 0;
    uvInc = lvInc = rvInc = 1;
    hitCount = 0;

    bigRadius = 50;
    smallRadius = 25;
    rodLength = 150;
    outRadius = 50;
    sliceCount = 50;

    targetSize = 200;
    targetDist = 600;
    hitSize = 2;
}

// rotate Vector 'whom' around Vector 'axis', anticlockwise
// 'whom' and 'axis' are orthogonal unit Vectors
Vector orthogonalRotate(Vector whom, Vector axis, double angle) {
    angle *= pi / 180;
    Vector direction = cross(axis, whom);
    return whom * cos(angle) + direction * sin(angle);
}

// rotate Point 'whom' around Vector 'p2 - p1'
// 'p2 - p1' is a unit Vector
__unused Point pointRotate(Point whom, Point p1, Point p2, double angle) {
    angle *= pi / 180;
    Vector axis = p2 - p1;
    double d = hypot(axis.y, axis.z);
    double b_d = axis.y / d;
    double c_d = axis.z / d;
    double cos_a = cos(angle);
    double sin_a = sin(angle);

    whom = whom - p1;
    whom = Point(whom.x, c_d * whom.y - b_d * whom.z, c_d * whom.z + b_d * whom.y);
    whom = Point(d * whom.x - axis.x * whom.z, whom.y, d * whom.z + axis.x * whom.x);
    whom = Point(whom.x * cos_a - whom.y * sin_a, whom.x * sin_a + whom.y * cos_a, whom.z);
    whom = Point(d * whom.x + axis.x * whom.z, whom.y, d * whom.z - axis.x * whom.x);
    whom = Point(whom.x, c_d * whom.y + b_d * whom.z, c_d * whom.z - b_d * whom.y);
    whom = whom + p1;

    return whom;
}

void increase(double &val, double inc, double limit) {
    if (val + inc <= limit) {
        val += inc;
    }
}

void decrease(double &val, double dec, double limit) {
    if (val - dec >= limit) {
        val -= dec;
    }
}

void drawAxes() {
    if (shouldDrawAxes) {
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINES);
        {
            glVertex3f(250, 0, 0);
            glVertex3f(-250, 0, 0);

            glVertex3f(0, -250, 0);
            glVertex3f(0, 250, 0);

            glVertex3f(0, 0, 250);
            glVertex3f(0, 0, -250);
        }
        glEnd();
    }
}

void drawSquare(double x, double y, double z, double a) {
    glBegin(GL_QUADS);
    {
        glVertex3f(x, y + a, z + a);
        glVertex3f(x, y - a, z + a);
        glVertex3f(x, y - a, z - a);
        glVertex3f(x, y + a, z - a);
    }
    glEnd();
}

void drawHalfSphere(double radius, int slices, int stacks) {
    Point points[2][sliceLimit];
    int prev = 0, cur = 1;
    Point pole(radius, 0, 0);
    for (int j = 0; j <= slices; j++) {
        points[prev][j] = pole;
    }

    for (int i = 1; i <= stacks; i++, prev ^= 1, cur ^= 1) {
        double angleStack = (double) i / stacks * (pi / 2);
        double h = radius * cos(angleStack);
        double r = fabs(radius) * sin(angleStack);
        for (int j = 0; j <= slices; j++) {
            double angleSlice = ((double) j / (double) slices) * 2 * pi;
            points[cur][j].x = h;
            points[cur][j].y = r * sin(angleSlice);
            points[cur][j].z = - r * cos(angleSlice);
        }
        for (int j = 0, shade = 0; j < slices; j++, shade ^= 1) {
            glColor3f(shade, shade, shade);
            glBegin(GL_QUADS);
            {
                glVertex3f(points[cur][j].x, points[cur][j].y, points[cur][j].z);
                glVertex3f(points[cur][j + 1].x, points[cur][j + 1].y, points[cur][j + 1].z);
                glVertex3f(points[prev][j + 1].x, points[prev][j + 1].y, points[prev][j + 1].z);
                glVertex3f(points[prev][j].x, points[prev][j].y, points[prev][j].z);
            }
            glEnd();
        }
    }
}

void drawCylinder(double radius, double height, int slices) {
    double prevY = 0, prevZ = radius;
    for (int i = 1, shade = 0; i <= slices; i++, shade ^= 1) {
        double angleSlice = ((double) i / slices) * 2 * pi;
        double curY = radius * sin(angleSlice);
        double curZ = - radius * cos(angleSlice);
        glColor3f(shade, shade, shade);
        glBegin(GL_QUADS);
        {
            glVertex3f(0, prevY, prevZ);
            glVertex3f(height, prevY, prevZ);
            glVertex3f(height, curY, curZ);
            glVertex3f(0, curY, curZ);
        }
        glEnd();
        prevY = curY;
        prevZ = curZ;
    }
}

void drawSkirt(double innerRadius, double outerRadius, int slices, int stacks) {
    double spreadRadius = outerRadius - innerRadius;

    Point points[2][sliceLimit];
    int prev = 0, cur = 1;
    for (int j = 0; j <= slices; j++) {
        double angleSlice = ((double) j / slices) * 2 * pi;
        double y = innerRadius * sin(angleSlice);
        double z = - innerRadius * cos(angleSlice);
        points[prev][j] = Point(0, y, z);
    }

    for (int i = 1; i <= stacks; i++, prev ^= 1, cur ^= 1) {
        double angleStack = (double) i / stacks * (pi / 2);
        double h = spreadRadius * sin(angleStack);
        double r = innerRadius + spreadRadius * (1 - cos(angleStack));
        for (int j = 0; j <= slices; j++) {
            double angleSlice = ((double) j / (double) slices) * 2 * pi;
            points[cur][j].x = h;
            points[cur][j].y = r * sin(angleSlice);
            points[cur][j].z = - r * cos(angleSlice);
        }
        for (int j = 0, shade = 0; j < slices; j++, shade ^= 1) {
            glColor3f(shade, shade, shade);
            glBegin(GL_QUADS);
            {
                glVertex3f(points[cur][j].x, points[cur][j].y, points[cur][j].z);
                glVertex3f(points[cur][j + 1].x, points[cur][j + 1].y, points[cur][j + 1].z);
                glVertex3f(points[prev][j + 1].x, points[prev][j + 1].y, points[prev][j + 1].z);
                glVertex3f(points[prev][j].x, points[prev][j].y, points[prev][j].z);
            }
            glEnd();
        }
    }
}

void drawGun() {
    glPushMatrix();
    {
        glRotatef(wholeAngle, 0, 1, 0);
        drawHalfSphere(-bigRadius, sliceCount, 50);

        glRotatef(halfAngle, 0, 0, 1);
        drawHalfSphere(bigRadius, sliceCount, 50);

        glTranslatef(bigRadius, 0, 0);

        glRotatef(rodAngle, 0, 0, 1);
        glRotatef(rotateAngle, 1, 0, 0);

        glTranslatef(smallRadius, 0, 0);
        drawHalfSphere(-smallRadius, sliceCount, 50);
        drawCylinder(smallRadius, rodLength, sliceCount);

        glTranslatef(rodLength, 0, 0);
        drawSkirt(smallRadius, outRadius, sliceCount, 50);
    }
    glPopMatrix();
}

void drawTarget() {
    glColor3f(0, 0.7, 0);

    // +0.1 -> draw the target a little behind to avoid overshadowing the hits
    drawSquare(targetDist + 0.1, 0, 0, targetSize);

    glColor3f(1, 0, 0);
    for (int i = 0; i < hitCount; i++) {
        drawSquare(targetDist, hits[i].y, hits[i].z, hits[i].x);
    }
}

void drawSS() {
    drawAxes();
    drawGun();
    drawTarget();
}

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
        case 'q':
            increase(wholeAngle, gunAngleInc, wholeRight);
            break;
        case 'w':
            decrease(wholeAngle, gunAngleInc, wholeLeft);
            break;
        case 'e':
            increase(halfAngle, gunAngleInc, halfUp);
            break;
        case 'r':
            decrease(halfAngle, gunAngleInc, halfDown);
            break;
        case 'a':
            increase(rodAngle, gunAngleInc, rodUp);
            break;
        case 's':
            decrease(rodAngle, gunAngleInc, rodDown);
            break;
        case 'd':
            increase(rotateAngle, 2 * gunAngleInc, rotateRight);
            break;
        case 'f':
            decrease(rotateAngle, 2 * gunAngleInc, rotateLeft);
            break;
        case 'u': targetSize++; break;
        case 'U': targetSize--; break;
        case 'i': targetDist++; break;
        case 'I': targetDist--; break;
        case 'o':
            hitSize++;
            printf("hitSize = %f\n", hitSize);
            break;
        case 'O':
            hitSize--;
            printf("hitSize = %f\n", hitSize);
            break;
        case 'h': bigRadius++; break;
        case 'H': bigRadius--; break;
        case 'j': smallRadius++; break;
        case 'J': smallRadius--; break;
        case 'k': rodLength++; break;
        case 'K': rodLength--; break;
        case 'l': outRadius++; break;
        case 'L': outRadius--; break;
        case ';':
            if (sliceCount+1 < sliceLimit)
                sliceCount++;
            break;
        case ':':
            if (sliceCount > 6)
                sliceCount--;
            break;
        default:
            break;
    }
    glutPostRedisplay();
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
    glutPostRedisplay();
}

void mouseListener(int button, int state, __unused int x, __unused int y) {
    switch (button) {
        case GLUT_LEFT_BUTTON: // fire gun
            if (state == GLUT_DOWN) {
                Vector gunU(0, 1, 0), gunL(1, 0, 0);
                gunL = orthogonalRotate(gunL, gunU, wholeAngle);
                Vector gunR = cross(gunL, gunU);
                gunL = orthogonalRotate(gunL, gunR, halfAngle);

                Point rodRoot = gunL * bigRadius;

                gunL = orthogonalRotate(gunL, gunR, rodAngle);

                Point rodEnd = rodRoot + /* (smallRadius + rodLength) * */ gunL;

                double xVal = (targetDist - rodEnd.x) / (rodEnd.x - rodRoot.x);
                double ySect = xVal * (rodEnd.y - rodRoot.y) + rodEnd.y;
                double zSect = xVal * (rodEnd.z - rodRoot.z) + rodEnd.z;

                if (ySect > -targetSize && ySect < targetSize && zSect > -targetSize && zSect < targetSize) {
                    hits[hitCount].x = hitSize; // x is being used to store size of hit
                    hits[hitCount].y = ySect;
                    hits[hitCount].z = zSect;
                    hitCount++;
                }
            }
            break;
        case GLUT_RIGHT_BUTTON: // toggle axis
            if (state == GLUT_DOWN) {
                shouldDrawAxes = !shouldDrawAxes;
            }
            break;
        default:
            break;
    }
    glutPostRedisplay();
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

    drawSS();

    // ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
    glutSwapBuffers();
}

// void animate() {
//     glutPostRedisplay();
// }

void init() {
    // codes for initialization
    initialize();

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
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);    // Depth, Double buffer, RGB color

    glutCreateWindow("Problem-1: Gun");

    init();

    glEnable(GL_DEPTH_TEST);    // enable Depth Testing

    glutDisplayFunc(display);    // display callback function
    // glutIdleFunc(animate);        // what you want to do in the idle time (when no drawing is occurring)

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMouseFunc(mouseListener);

    glutMainLoop();        // The main loop of OpenGL

    return 0;
}
