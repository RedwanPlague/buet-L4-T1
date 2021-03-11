#include <GL/glut.h>
#include <cmath>
#include <random>
#include <chrono>

#define __unused __attribute__((unused))

const double pi = acos(-1);

const double frameSize = 150;
const double arenaRadius = 125;
const double ballRadius = 15;
const double initShift = 0.01;
const double shiftInc = 0.001;
const double shiftUp = 0.1;
const double shiftDown = 0.001;
const int ballCount = 5;
int ballUse = 0;
double shift, shiftKeep;
bool separate[ballCount][ballCount];

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

    double norm() const {
        return sqrt(x * x + y * y + z * z);
    }
};
typedef Point Vector;

double dot(const Vector &a, const Vector &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

double distance(const Point &a, const Point &b) {
    double x = a.x - b.x;
    double y = a.y - b.y;
    double z = a.z - b.z;
    return sqrt(x * x + y * y + z * z);
}

void drawSquare(double a) {
    glBegin(GL_LINE_LOOP);
    {
        glVertex3f(a, a, 0);
        glVertex3f(-a, a, 0);
        glVertex3f(-a, -a, 0);
        glVertex3f(a, -a, 0);
    }
    glEnd();
}

void drawCircle(double radius, int segments) {
    glBegin(GL_LINE_LOOP);
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

struct Ball {
    Point center;
    double radius;
    Vector direction;
    bool inside;

    void move(double amount) {
        center = center + direction * amount;
    }

    void frameReflect() {
        double limit = frameSize - ballRadius;
        if (center.x < -limit || center.x > limit) {
            direction.x = -direction.x;
        }
        if (center.y < -limit || center.y > limit) {
            direction.y = -direction.y;
        }
    }

    void arenaReflect() {
        double dist = center.norm();
        if (inside) {
            if (dist >= arenaRadius - ballRadius) {
                Vector normal = - center / dist;
                direction = direction - normal * 2 * dot(direction, normal);
            }
        }
        else {
            inside = (dist < arenaRadius - ballRadius);
        }
    }

    bool separate(const Ball &b) const {
        return inside && b.inside && (distance(center, b.center) > radius + b.radius);
    }

    void ballReflect(const Ball &b) {
        if (inside && b.inside) {
            if (distance(center, b.center) <= radius + b.radius) {
                Vector normal = center - b.center;
                normal = normal / normal.norm();
                direction = direction - normal * 2 * dot(direction, normal);
            }
        }
    }

    void draw() const {
        glPushMatrix();
        {
            glTranslatef(center.x, center.y, center.z);
            drawCircle(radius, 50);
        }
        glPopMatrix();
    }
};

Ball balls[ballCount];

void drawSS() {
    glColor3f(0, 1, 0);
    drawSquare(frameSize);

    glColor3f(1, 0, 0);
    drawCircle(arenaRadius, 50);

    glColor3f(1, 0.4, 0);
    for (int i = 0; i < ballUse; i++) {
        balls[i].draw();
    }
}

void updateBalls() {
    for (int i = 0; i < ballUse; i++) {
        balls[i].move(shift);
        balls[i].frameReflect();
        balls[i].arenaReflect();
        for (int j = 0; j < ballUse; j++) {
            if (separate[i][j]) {
                balls[i].ballReflect(balls[j]);
            }
            else {
                separate[i][j] = balls[i].separate(balls[j]);
            }
        }
    }
}

double count = 0;

void animate() {
    if (ballUse < ballCount) {
        count += 0.0005;
        ballUse = floor(count);
    }
    updateBalls();
    // codes for any changes in Models, Camera
    glutPostRedisplay();
}

void keyboardListener(unsigned char key, __unused int x, __unused int y) {
    if (key == 'p') {
        if (shift > 0) {
            shiftKeep = shift;
            shift = 0;
        }
        else {
            shift = shiftKeep;
        }
    }
}

void specialKeyListener(int key, __unused int x, __unused int y) {
    switch (key) {
        case GLUT_KEY_UP:
            if (shift + shiftInc <= shiftUp) {
                shift += shiftInc;
            }
            break;
        case GLUT_KEY_DOWN:
            if (shift - shiftInc >= shiftDown) {
                shift -= shiftInc;
            }
            break;
        default:
            break;
    }
}

// void mouseListener(int button, int state, int x, int y) {}

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

    gluLookAt(0, 0, 200, 0, 0, 0, 0, 1, 0);

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

void init() {
    // codes for initialization
    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    for (int i = 0; i < ballCount; i++) {
        balls[i].center = Point(-frameSize + ballRadius, -frameSize + ballRadius, 0);
        balls[i].radius = ballRadius;
        double theta = std::uniform_real_distribution<double>(0, pi / 2)(rng);
        balls[i].direction = Vector(cos(theta), sin(theta), 0);
        balls[i].inside = false;

        for (int j = 0; j < ballCount; j++) {
            separate[i][j] = false;
        }
    }

    shift = initShift;

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
    // glutMouseFunc(mouseListener);

    glutMainLoop();        // The main loop of OpenGL

    return 0;
}
