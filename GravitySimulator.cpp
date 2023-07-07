#include "pch.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <GL\glut.h>
#include <GL\gl.h>

#define PI acos(-1)

const int oo = 1000000; // as infinity mass
int Mx, My; // mouse positions
bool leftClicked = false;
bool rightClicked = false;
bool middleClicked = false;
bool onHomePage = true;

struct particle {
    float x;
    float y;
    float r;
    float vx;
    float vy;
    float m; // mass
    float color[3];
};

std::vector<particle> particles;

// Function declarations
void display();
void renderHomePage();
void renderProjectPage();
void addParticle(float, float, float = 0, float = 0);
void removeParticles();
void mouse(int, int, int, int);
void mouseMotion(int, int);
void timer(int = 0);
void keyboard(unsigned char, int, int);
void switchToProjectPage();

void init(void) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-250, 250, 250, -250.0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (onHomePage) {
        renderHomePage();
    }
    else {
        renderProjectPage();
    }

    glutSwapBuffers();
}

void renderHomePage() {
    // Render the home page text
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2f(-130.5, 0.0); // Adjust the X-coordinate to -0.5 for a leftward position
    const char* homeText = "Welcome to the Gravity Simulator!";
    for (int i = 0; homeText[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, homeText[i]);
    }

    // Render the project text
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2f(-130, 210);
    const char* projectText = "Project Done by G V Monish and Gaurav R";
    for (int i = 0; projectText[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, projectText[i]);
    }
    

    // Render the button
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_QUADS);
    glVertex2f(-0.1, -0.4);
    glVertex2f(0.1, -0.4);
    glVertex2f(0.1, -0.2);
    glVertex2f(-0.1, -0.2);
    glEnd();

    // Render the button text
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2f(-75, 70.3);
    const char* buttonText = "Press Space to Proceed";
    for (int i = 0; buttonText[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, buttonText[i]);
    }
}

void renderProjectPage() {
    // Show all the particles from the particles vector as circles
    for (int i = 0; i < int(particles.size()); ++i) {
        particle& p = particles[i];
        glColor3f(p.color[0], p.color[1], p.color[2]);
        glBegin(GL_POLYGON);
        // Drawing the particle as a circle
        for (float t = 0.0; t < 2 * PI; t += 0.2) {
            glVertex2f(p.r * cos(t) + p.x, p.r * sin(t) + p.y);
        }
        glEnd();
    }
}

void addParticle(float m, float r, float vx, float vy) {
    particle p;
    p.x = Mx;
    p.y = My;
    p.r = r;
    p.vx = vx / 3;
    p.vy = vy / 3;
    p.m = m;
    // Getting random color
    p.color[0] = rand() % 200 / 200.0;
    p.color[1] = rand() % 200 / 200.0;
    p.color[2] = rand() % 200 / 200.0;

    // Add the particle to the particles vector
    particles.push_back(p);
}

void removeParticles() {
    particles.clear();
}

void mouse(int button, int state, int x, int y) {
    Mx = x - 250;
    My = y - 250;

    if (onHomePage && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (Mx >= -50 && Mx <= 50 && My >= 200 && My <= 240) {
            switchToProjectPage();
        }
    }

    if (button == GLUT_LEFT_BUTTON)
        leftClicked = state == GLUT_DOWN;
    if (button == GLUT_RIGHT_BUTTON)
        rightClicked = state == GLUT_DOWN;
    else if (button == GLUT_MIDDLE_BUTTON)
        middleClicked = state == GLUT_DOWN;
}

void mouseMotion(int x, int y) {
    Mx = x - 250;
    My = y - 250;
}

void timer(int) {
    display();

    if (!onHomePage) {
        if (leftClicked) {
            addParticle(10, 8);
            leftClicked = false;
        }
        if (rightClicked) {
            addParticle(oo, 15);
            rightClicked = false;
        }
        if (middleClicked)
            removeParticles(); // Remove all particles

        for (int i = 0; i < int(particles.size()); i++) {
            particle& p = particles[i];
            bool notFall = true;
            for (int j = 0; j < int(particles.size()); j++) {
                if (j == i || p.m >= oo)
                    continue;
                const particle& p1 = particles[j];
                float d = sqrt((p1.x - p.x) * (p1.x - p.x) + (p1.y - p.y) * (p1.y - p.y));
                if (d > p1.r) {
                    p.vx += 0.0003 * p1.m / (d * d) * (p1.x - p.x) / d;
                    p.vy += 0.0003 * p1.m / (d * d) * (p1.y - p.y) / d;
                }
                else
                    notFall = false;
            }
            if (notFall) {
                p.x += p.vx;
                p.y += p.vy;
            }
            else
                particles.erase(particles.begin() + i);
        }
    }

    glutTimerFunc(1, timer, 0);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case ' ':
        if (onHomePage)
            switchToProjectPage();
        break;
    case 27:
        removeParticles();
        exit(0);
        break;
    }
}

void switchToProjectPage() {
    onHomePage = false;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Gravity Simulator");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(mouseMotion);
    glutKeyboardFunc(keyboard);
    timer();
    glutMainLoop();
    return 0;
}
