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

#define TIMER_PERIOD  16 // Period for the timer.
#define TIMER_ON       1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

#define welcomeScreen 0
#define game 1
#define endScreen 2

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

int cannonX = -540; //X and Y 
int cannonY = 0;    //Coordinates of the shooting object - spaceship

bool initialAnimation = true;

int stateOfTV = welcomeScreen;
bool startButton = false;
bool activeTimer = false;

int missed = 0; //to check if the bullet missed the object
int counter = 0; //to check the bullets used
int score = 0; //to check how many bullets hit the incoming objects

//--------------------------------------------------------------------------

//
// to draw circle, center at (x,y)
// radius r
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

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
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

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
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

//creating structure for bullets-----------------------------------------------------------
typedef struct
{
    int x;
    int y;
}coordinates_t;

typedef struct
{
    coordinates_t coordinates;
    int radius;
    bool fireActive;
}bullet_t;

typedef struct {
    int r, g, b;
}colors_t;

typedef struct {
    coordinates_t coordinates;
    colors_t color;
}incoming_t;

bullet_t bullet = { {cannonX, cannonY}, 5, false };
incoming_t incoming;
incoming_t incoming2;
incoming_t incoming3;
incoming_t incoming4;
//---------------------------------------------------
void welcomeScreenn()
{
    glColor3f(0.53, 0.53, 0.53);
    glRectf(-300, -10, 300, 100);

    glColor3ub(0, 200, 200);
    vprint(-201, 71, GLUT_BITMAP_9_BY_15, "Hit as many spaceships as you can");
    vprint(-201, 41, GLUT_BITMAP_9_BY_15, "Missing more than 6 targets will end the game");
    vprint(-201, 11, GLUT_BITMAP_9_BY_15, "Press M to start the game");
}

void drawCannon() {
    glColor3ub(149, 125, 136);
    glBegin(GL_TRIANGLES);
    glVertex2f(-29, 120);//s1
    glVertex2f(-9, 120);
    glVertex2f(-19, 100);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(-29, 105);
    glVertex2f(-9, 105);
    glVertex2f(-19, 125);
    glEnd();

    int b = 29;

    glBegin(GL_TRIANGLES);
    glVertex2f(-29 - b, 120 - b);
    glVertex2f(-9 - b, 120 - b);
    glVertex2f(-19 - b, 100 - b);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(-29 - b, 105 - b);
    glVertex2f(-9 - b, 105 - b);
    glVertex2f(-19 - b, 125 - b);
    glEnd();

    b += 39;


    glBegin(GL_TRIANGLES);
    glVertex2f(-29 + b, 120 + b);
    glVertex2f(-9 + b, 120 + b);
    glVertex2f(-19 + b, 100 + b);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(-29 + b, 105 + b);
    glVertex2f(-9 + b, 105 + b);
    glVertex2f(-19 + b, 125 + b);
    glEnd();

    b += 24;

    glBegin(GL_TRIANGLES);
    glVertex2f(-29 - b, 120);
    glVertex2f(-9 - b, 120);
    glVertex2f(-19 - b, 100);//s4
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(-29 - b, 105);
    glVertex2f(-9 - b, 105);
    glVertex2f(-19 - b, 125);
    glEnd();

    b + 14;

    glBegin(GL_TRIANGLES);
    glVertex2f(-29 + b, 120 - b);
    glVertex2f(-9 + b, 120 - b);
    glVertex2f(-19 + b, 100 - b);//s5
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(-29 + b, 105 - b);
    glVertex2f(-9 + b, 105 - b);
    glVertex2f(-19 + b, 125 - b);
    glEnd();


    b += 179;


    glBegin(GL_TRIANGLES);
    glVertex2f(-29 - b, 120);//s7
    glVertex2f(-9 - b, 120);
    glVertex2f(-19 - b, 100);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(-29 - b, 105);
    glVertex2f(-9 - b, 105);
    glVertex2f(-19 - b, 125);
    glEnd();

    b = -349;

    glBegin(GL_TRIANGLES);
    glVertex2f(-29 + b, 120);
    glVertex2f(-9 + b, 120);
    glVertex2f(-19 + b, 100);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(-29 + b, 105);
    glVertex2f(-9 + b, 105);
    glVertex2f(-19 + b, 125);
    glEnd();

    b = 349;

    glBegin(GL_TRIANGLES);
    glVertex2f(-29 + b, 120);
    glVertex2f(-9 + b, 120);
    glVertex2f(-19 + b, 100);//s8
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(-29 + b, 105);
    glVertex2f(-9 + b, 105);
    glVertex2f(-19 + b, 125);
    glEnd();

    b = 499;
    int y = -79;

    glBegin(GL_TRIANGLES);
    glVertex2f(-14 + b, 105 + y);
    glVertex2f(4 + b, 105 + y);
    glVertex2f(-6 + b, 85 + y);//s9
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(-14 + b, 90 + y);
    glVertex2f(4 + b, 90 + y);
    glVertex2f(-6 + b, 110 + y);
    glEnd();

    //shooting spaceship
    glColor3ub(136, 57, 239);
    glBegin(GL_POLYGON);
    glVertex2f(cannonX, cannonY);
    glVertex2f(cannonX - 40, cannonY + 20);
    glVertex2f(cannonX - 40, cannonY - 20);
    glEnd();

    //name tag
    glColor3f(0.53, 0.53, 0.53);
    glRectf(-530, 130, -340, 270);

    glColor3ub(0, 200, 200);
    //vprint(-520, 250, GLUT_BITMAP_9_BY_15, "Muhammad Saad Ameer");
    //vprint(-520, 230, GLUT_BITMAP_9_BY_15, "HW-2");
    vprint(-520, 210, GLUT_BITMAP_9_BY_15, "Shooting Spaceships");
    vprint(-520, 190, GLUT_BITMAP_9_BY_15, "Missed bullets : %d", missed);
    vprint(-520, 170, GLUT_BITMAP_9_BY_15, "Bullets fired : %d", counter);
}

void drawBullet(bullet_t bullet)
{
    if (bullet.fireActive) {
        glColor3f(0, 0, 1);
        circle(bullet.coordinates.x, bullet.coordinates.y - 2, bullet.radius);
    }
}

void drawIncoming(incoming_t incoming) {
    glBegin(GL_POLYGON);
    glColor3ub(incoming.color.r, incoming.color.g, incoming.color.b);
    glVertex2f(incoming.coordinates.x + 600, incoming.coordinates.y + 40);
    glVertex2f(incoming.coordinates.x + 640, incoming.coordinates.y + 60);
    glVertex2f(incoming.coordinates.x + 640, incoming.coordinates.y + 20);
    glEnd();
}

void resetIncoming() {
    float yPos = (rand() % (0 - (-300) + 1) + (-300));
    incoming.coordinates.y = yPos;
    incoming.coordinates.x = 600;
    incoming.color.r = rand() % 256;
    incoming.color.g = rand() % 256;
    incoming.color.b = rand() % 256;
}

void drawIncoming2(incoming_t incoming2) {
    glBegin(GL_POLYGON);
    glColor3ub(incoming2.color.r, incoming2.color.g, incoming2.color.b);
    glVertex2f(incoming2.coordinates.x + 700, incoming2.coordinates.y);
    glVertex2f(incoming2.coordinates.x + 740, incoming2.coordinates.y + 20);
    glVertex2f(incoming2.coordinates.x + 740, incoming2.coordinates.y - 20);
    glEnd();
}

void resetIncoming2() {
    float yPos = (rand() % (0 - (-300) + 1) + (-300));
    incoming2.coordinates.y = yPos;
    incoming2.coordinates.x = 600;
    incoming2.color.r = rand() % 256;
    incoming2.color.g = rand() % 256;
    incoming2.color.b = rand() % 256;
}

void drawIncoming3(incoming_t incoming3) {
    glBegin(GL_POLYGON);
    glColor3ub(incoming3.color.r, incoming3.color.g, incoming3.color.b);
    glVertex2f(incoming3.coordinates.x + 800, incoming3.coordinates.y - 40);
    glVertex2f(incoming3.coordinates.x + 840, incoming3.coordinates.y - 60);
    glVertex2f(incoming3.coordinates.x + 840, incoming3.coordinates.y - 20);
    glEnd();
}

void resetIncoming3() {
    float yPos = (rand() % (0 - (-300) + 1) + (-300));
    incoming3.coordinates.y = yPos;
    incoming3.coordinates.x = 600;
    incoming3.color.r = rand() % 256;
    incoming3.color.g = rand() % 256;
    incoming3.color.b = rand() % 256;
}

void drawIncoming4(incoming_t incoming4) {
    glBegin(GL_POLYGON);
    glColor3ub(incoming4.color.r, incoming4.color.g, incoming4.color.b);
    glVertex2f(incoming4.coordinates.x + 900, incoming4.coordinates.y + 120);
    glVertex2f(incoming4.coordinates.x + 940, incoming4.coordinates.y + 140);
    glVertex2f(incoming4.coordinates.x + 940, incoming4.coordinates.y + 100);
    glEnd();
}

void resetIncoming4() {
    float yPos = (rand() % (0 - (-300) + 1) + (-300));
    incoming4.coordinates.y = yPos;
    incoming4.coordinates.x = 600;
    incoming4.color.r = rand() % 256;
    incoming4.color.g = rand() % 256;
    incoming4.color.b = rand() % 256;
}

void endScreenWindow() {
    glColor3f(0.53, 0.53, 0.53);
    glRectf(-600, -300, 600, 300);

    glColor3ub(0, 200, 200);
    vprint(-200, 70, GLUT_BITMAP_9_BY_15, "          Game Over!");
    vprint(-200, 40, GLUT_BITMAP_9_BY_15, "You missed more than 6 targets");
    vprint(-200, 10, GLUT_BITMAP_9_BY_15, "     Total score is : %d", score);
}

//----------------------------------------------------

//
// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //
    glClearColor(1, 1, 1, 0.25);
    glClear(GL_COLOR_BUFFER_BIT);

    switch (stateOfTV)
    {
    case welcomeScreen:
        welcomeScreenn();
        break;
    case game:
        //drawStars();
        drawCannon();
        drawBullet(bullet);
        drawIncoming(incoming);
        drawIncoming2(incoming2);
        drawIncoming3(incoming3);
        drawIncoming4(incoming4);
        break;
    case endScreen:
        endScreenWindow();
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

    //if the key is space character.
    if (key == ' ')
    {
        bullet.fireActive = true;
    }
    if (key == 77 || key == 109)
    {
        stateOfTV = game;
        activeTimer = true;
    }
    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
    {
        exit(0);
    }
    if (key == ' ')
    {

    }

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
    case GLUT_KEY_UP:
        cannonY += 18;
        if (bullet.coordinates.x == cannonX) {
            bullet.coordinates.y += 18;
        }
        break;
    case GLUT_KEY_DOWN:
        cannonY -= 18;
        if (bullet.coordinates.x == cannonX) {
            bullet.coordinates.y -= 18;
        }
        break;
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_RIGHT: right = true; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
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

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.
    //printf("OnMove");
    //vprint2(30, 30, 0.35, "00:%02d", 100000000);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

bool hitObject(incoming_t incoming, incoming_t incoming2, incoming_t incoming3, incoming_t incoming4, bullet_t bullet) {
    if ((bullet.coordinates.x - 550 >= incoming.coordinates.x + 40)
        && (bullet.coordinates.y > incoming.coordinates.y + 20)
        && (bullet.coordinates.y < incoming.coordinates.y + 60))
    {
        resetIncoming();
        return true;
    }

    if ((bullet.coordinates.x - 700 >= incoming2.coordinates.x)
        && (bullet.coordinates.y > incoming2.coordinates.y - 20)
        && (bullet.coordinates.y < incoming2.coordinates.y + 20))
    {
        resetIncoming2();
        return true;
    }

    if ((bullet.coordinates.x - 820 >= incoming3.coordinates.x - 40)
        && (bullet.coordinates.y > incoming3.coordinates.y - 60)
        && (bullet.coordinates.y < incoming3.coordinates.y - 20))
    {
        resetIncoming3();
        return true;
    }

    if ((bullet.coordinates.x - 800 >= incoming4.coordinates.x + 120)
        && (bullet.coordinates.y > incoming4.coordinates.y + 100)
        && (bullet.coordinates.y < incoming4.coordinates.y + 140))
    {
        resetIncoming4();
        return true;
    }

    return false;
}

#if TIMER_ON == 1
void onTimer(int v)
{
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);

    // Write your codes here.

    incoming.coordinates.x -= 6;
    if (incoming.coordinates.x < -1100)
        resetIncoming();

    incoming2.coordinates.x -= 5;
    if (incoming2.coordinates.x < -1200)
        resetIncoming2();

    incoming3.coordinates.x -= 4;
    if (incoming3.coordinates.x < -1300)
        resetIncoming3();

    incoming4.coordinates.x -= 4;
    if (incoming4.coordinates.x < -1400)
        resetIncoming4();

    if (activeTimer == true)
    {
        //bullet Animation
        if (bullet.coordinates.x > 1000)
        {
            bullet.fireActive = false;
            missed++;
            counter++;
            bullet.coordinates = { -540, cannonY };
        }

        if (hitObject(incoming, incoming2, incoming3, incoming4, bullet))
        {
            bullet.fireActive = false;
            bullet.coordinates.x = -540;
            score++;
            counter++;
        }
    }
    if (bullet.fireActive == true)
        bullet.coordinates.x += 13;

    if (missed == 6) {
        stateOfTV = endScreen;
        activeTimer = false;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
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

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}