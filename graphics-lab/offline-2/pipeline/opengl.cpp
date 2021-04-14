#include <GL/gl.h>
#include <GL/glut.h>
#include <cstdio>

void print() {
    GLfloat model[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, model);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%3.2f ", model[4 * j + i]);
        }
        putchar('\n');
    }
    putchar('\n');
}

void drawAxes() {
    const int grain = 10;
    for (int u = -grain; u < grain; u++) {
        glColor3f(1, u != 0, u != 0);
        glBegin(GL_LINES);
        {
            glVertex3f(u / (double)grain, -1, 0);
            glVertex3f(u / (double)grain, 1, 0);
        }
        glEnd();
        glBegin(GL_LINES);
        {
            glVertex3f(-1, u / (double)grain, 0);
            glVertex3f(1, u / (double)grain, 0);
        }
        glEnd();
    }
}

void displayMe(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    drawAxes();
    print();
    glScalef(2, 0.5, 1);
    glTranslatef(-0.1, -0.2, 0);
    glScalef(1, 2, 1);
    glTranslatef(0.1, 0.2, 0);
    print();
    glColor3f(1, 1, 1);
    glBegin(GL_TRIANGLES);
    {
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 0.1, 0.0);
        glVertex3f(0.1, 0.0, 0.0);
    }
    glEnd();

    glFlush();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Hello world!");
    glutDisplayFunc(displayMe);
    glutMainLoop();
    return 0;
}
