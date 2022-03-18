#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH  1200
#define WINDOW_HEIGHT 600

#define  TIMER_PERIOD  16 // Period for the timer.
#define  TIMER_ON     1   // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* GLOBAL VARIABLES and CONSTANTS */
#define  OFF 0
#define  ON 1
#define  PAUSE  2

bool up = false;
bool down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height
bool activeTimer = false;
int stateOfTV = OFF;
bool offButton = false;
bool pauseButton = false;
int sx = -410;
int sy = -150;

// this is function signature (prototype)
// this is called "function declaration"
void onTimer(int v);
void vprint(int x, int y, void* font, const char* string, ...);


//
// to draw circle, center at (x,y)
//  radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}
void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}
void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}

void themeBackground() {

    //background patterns
    glColor3ub(212, 242, 160);
    glRectf(-700, -400, -650, 350);
    glRectf(-600, -400, -550, 350);
    glRectf(-500, -400, -450, 350);
    glRectf(-400, -400, -350, 350);
    glRectf(-300, -400, -250, 350);
    glRectf(-200, -400, -150, 350);
    glRectf(-100, -400, -50, 350);
    glRectf(0, -400, 50, 350);
    glRectf(100, -400, 150, 350);
    glRectf(200, -400, 250, 350);
    glRectf(300, -400, 350, 350);
    glRectf(400, -400, 450, 350);
    glRectf(500, -400, 550, 350);
    glRectf(600, -400, 650, 350);
    glRectf(700, -400, 750, 350);

    //buttons layer
    glColor3ub(164, 116, 73);
    glRectf(-470, -270, 450, 270);

    //brown layer
    glColor3ub(183, 119, 41);
    glRectf(-450, -250, 250, 250);

    //grey layer
    glColor3ub(223, 215, 200);
    glRectf(-430, -230, 230, 230);

    //night on tv
    glColor3f(0, 0, 0);
    glRectf(-410, -210, 210, 210);

    //button circles
    glColor3ub(116, 107, 90);
    circle(370, 170, 50);
    glColor3f(1, 1, 1);
    vprint2(350, 165, 0.1, "ON/OFF");

    glColor3ub(116, 107, 90);
    circle(370, 15, 50);
    glColor3f(1, 1, 1);
    vprint2(350, 20, 0.1, "PAUSE/");
    vprint2(350, 0, 0.1, "RESTART");


    //name, id
    //glColor3f(0, 0, 0);
    //vprint(-570, 290, GLUT_BITMAP_8_BY_13, "Muhammad Saad Ameer");
    //vprint(-570, 275, GLUT_BITMAP_8_BY_13, "21901118");
    //vprint(-570, 260, GLUT_BITMAP_9_BY_15, "The Sailor");
}


void tvOn() {

    //night on tv
    glColor3ub(76, 70, 70);
    glRectf(-410, -210, 210, 210);

    //mountains
    glColor3ub(85, 65, 36);
    glBegin(GL_POLYGON);
    glVertex2f(-410, -50);
    glVertex2f(-410, 100);
    glVertex2f(-290, -50);
    glVertex2f(-246, 80);
    glVertex2f(-160, -50);
    //glVertex2f(-155, -57);
    glEnd();

    //water
    glColor3ub(46, 163, 242);
    glRectf(-410, -210, 210, -50);

    //waves
    glColor3ub(46, 163, 242);
    circle(-390, -65, 30);
    circle(-350, -65, 30);
    circle(-310, -65, 30);
    circle(-270, -65, 30);
    circle(-240, -65, 30);
    circle(-200, -65, 30);
    circle(-160, -65, 30);
    circle(-120, -65, 30);
    circle(-80, -65, 30);
    circle(-40, -65, 30);
    circle(0, -65, 30);
    circle(40, -65, 30);
    circle(80, -65, 30);
    circle(120, -65, 30);
    circle(160, -65, 30);
    circle(200, -65, 30);


    //ship
    glColor3f(0, 0.5, 1);
    glBegin(GL_POLYGON);
    glVertex2f(sx, sy + 50); // top left
    glVertex2f(sx + 30, sy); // bottom left
    glVertex2f(sx + 110, sy); // bottom right
    glVertex2f(sx + 140, sy + 50); // top right
    glEnd();

    glColor3f(0, 0.1, 0.1);
    glBegin(GL_POLYGON);
    glVertex2f(sx + 23, sy + 10); // top left
    glVertex2f(sx + 30, sy); // bottom left
    glVertex2f(sx + 110, sy); // bottom right
    glVertex2f(sx + 117, sy + 10); // top right
    glEnd();

    glColor3f(0, 0.5, 1);
    glRectf(sx + 30, sy + 50, sx + 110, sy + 70);

    glColor3f(0, 0.5, 1);
    glRectf(sx + 45, sy + 70, sx + 95, sy + 80);

    //flag
    glLineWidth(3);
    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
    glVertex2f(sx + 60, sy + 80);
    glVertex2f(sx + 60, sy + 120);
    glEnd();
    glLineWidth(1);

    glColor3f(1, 0, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(sx + 60, sy + 90);
    glVertex2f(sx + 60, sy + 110);
    glVertex2f(sx + 40, sy + 100);
    glEnd();

    glColor3f(1, 1, 1);
    glRectf(sx + 35, sy + 20, sx + 45, sy + 30);
    glRectf(sx + 55, sy + 20, sx + 65, sy + 30);
    glRectf(sx + 75, sy + 20, sx + 85, sy + 30);
    glRectf(sx + 95, sy + 20, sx + 105, sy + 30);

    glRectf(sx + 40, sy + 55, sx + 100, sy + 60);

    //moon
    glColor3ub(204, 194, 183);
    circle(140, 100, 50);
    //moon spots
    glColor3ub(174, 164, 153);
    circle(160, 130, 10);
    circle(120, 70, 5);
    circle(165, 80, 8);
    circle(115, 105, 7);

    //hiding ship
    //buttons layer
    glColor3ub(164, 116, 73);
    glRectf(250, -200, 400, -50);
    glRectf(250, -200, 320, 0);
    glRectf(-470, -200, -450, 210);

    //brown layer
    glColor3ub(183, 119, 41);
    glRectf(230, -200, 250, 210);
    glRectf(-450, -200, -430, 210);

    //grey layer
    glColor3ub(223, 215, 200);
    glRectf(210, -200, 230, 210);
    glRectf(-430, -200, -410, 210);

    //green layer
    glColor3ub(212, 242, 160);
    glRectf(-500, -210, -470, 210);

}


//
// To display onto window using OpenGL commands
//
void display()
{
    glClearColor(1, 1, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    // same background theme in all states
    themeBackground();

    switch (stateOfTV) {
    case ON:
        tvOn();
        break;
    case PAUSE:
        tvOn();
        break;
    }

    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}
//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_DOWN: down = true; if (sy >= -190)
        sy = sy - 5; break;
    case GLUT_KEY_UP: up = true; if (sy <= -135)
        sy = sy + 5; break;
    case GLUT_KEY_RIGHT: right = true; break;

    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}


//
// Special Key like GLUT_KEY_F1, F2, F3,...
//// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;

    }
    // to refresh the window it calls display() function
    glutPostRedisplay();
}


//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.
    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN) {
        if (stateOfTV == OFF && offButton == true) {
            stateOfTV = ON;
            activeTimer = true;
            glutTimerFunc(TIMER_PERIOD, onTimer, 0);
        }
        else if (stateOfTV == ON && pauseButton == true)
            stateOfTV = PAUSE;

        else if (stateOfTV == PAUSE && offButton == true)
        {
            stateOfTV = OFF;
            activeTimer = false;
        }
        else if (stateOfTV == PAUSE && pauseButton == true)
        {
            stateOfTV = ON;
            activeTimer = true;
            glutTimerFunc(TIMER_PERIOD, onTimer, 0);
        }

        else if (stateOfTV == ON && offButton == true)
        {
            stateOfTV = OFF;
            activeTimer = true;
            glutTimerFunc(TIMER_PERIOD, onTimer, 0);
        }

    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}
//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



     // to refresh the window it calls display() function   
    glutPostRedisplay();
}


// (px, py) : coordinate of a point, p
// (cx, cy) : center point (centroid) of the circle
// R : radius of the circle
// Result: true (point is withing the circle), otherwise false.
bool buttonChecker(float px, float py, float cx, float cy, float R) {
    float dx = px - cx;
    float dy = py - cy;
    float d = sqrt(dx * dx + dy * dy);
    return d <= R;
}

// When you move mouse pointer, this event handler is called automatically
void onMove(int x, int y) {
    // Write your codes here.
    int mx = x - winWidth / 2;
    int my = winHeight / 2 - y;

    // modify the flag that shows if the mouse is on the button.
    // start button circle = (0, 0 ) with radius 70
    offButton = buttonChecker(mx, my, 370, 170, 50);
    pauseButton = buttonChecker(mx, my, 370, 0, 50);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {
    // Write your codes here.
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    if (activeTimer) {
        if (stateOfTV == OFF || stateOfTV == ON)
            if (sx < 210)
                sx = sx + 1;
            else
                sx = -500;

        // to refresh the window it calls display() function
        glutPostRedisplay(); // display()

    }
}
#endif

void Init() {

}


void main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);


    Init();
    srand(time(0));
    glutMainLoop();

}