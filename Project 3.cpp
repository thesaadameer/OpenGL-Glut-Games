#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include "vec.h"

#define WINDOW_WIDTH  1200
#define WINDOW_HEIGHT 600

#define  TIMER_PERIOD  16 // Period for the timer.
#define  TIMER_ON     1     // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

bool isHit = false;
bool isFired = false;

int boxX = -450;
int boxY = -100;

//************************************************************
float A = 100, //amplitude
fq = 1,  //frequency
C = 0,   //horizontal phase shift
B = 0;   //vertical phase shift
float angle = 0;

/* User defined Types and variables for the application */
#define ENEMY_SPEED 2
#define PLAYER_SPEED 10

typedef struct {
	double x, y;
} point_t;

typedef struct {
	point_t posP;
	vec_t pos;
} player_t;

typedef struct {
	point_t posE;
	vec_t pos;
	vec_t vel;  //velocity
} enemy_t;

player_t P;
enemy_t  E;


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


// display text with variables.
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

void drawBackground()
{
	//clouds
	glColor3ub(197, 226, 247);
	circle(-510, 230, 20);
	circle(-490, 230, 20);
	circle(-500, 220, 20);
	circle(-480, 220, 20);

	circle(-350, 200, 20);
	circle(-330, 200, 20);
	circle(-340, 190, 20);
	circle(-320, 190, 20);

	circle(-210, 230, 20);
	circle(-190, 230, 20);
	circle(-200, 220, 20);
	circle(-180, 220, 20);

	circle(-50, 200, 20);
	circle(-30, 200, 20);
	circle(-40, 190, 20);
	circle(-20, 190, 20);

	circle(130, 230, 20);
	circle(110, 230, 20);
	circle(120, 220, 20);
	circle(140, 220, 20);

	circle(250, 200, 20);
	circle(230, 200, 20);
	circle(240, 190, 20);
	circle(260, 190, 20);

	circle(430, 230, 20);
	circle(410, 230, 20);
	circle(420, 220, 20);
	circle(440, 220, 20);

	// water
	glColor3ub(185, 242, 255);
	glRectf(-800, -1200, 800, -100);

	//waves
	circle(-670, -115, 30);
	circle(-630, -115, 30);
	circle(-590, -115, 30);
	circle(-550, -115, 30);
	circle(-510, -115, 30);
	circle(-470, -115, 30);
	circle(-430, -115, 30);
	circle(-390, -115, 30);
	circle(-350, -115, 30);
	circle(-310, -115, 30);
	circle(-270, -115, 30);
	circle(-240, -115, 30);
	circle(-200, -115, 30);
	circle(-160, -115, 30);
	circle(-120, -115, 30);
	circle(-80, -115, 30);
	circle(-40, -115, 30);
	circle(0, -115, 30);
	circle(40, -115, 30);
	circle(80, -115, 30);
	circle(120, -115, 30);
	circle(160, -115, 30);
	circle(200, -115, 30);
	circle(240, -115, 30);
	circle(280, -115, 30);
	circle(320, -115, 30);
	circle(360, -115, 30);
	circle(400, -115, 30);
	circle(440, -115, 30);
	circle(480, -115, 30);
	circle(520, -115, 30);
	circle(560, -115, 30);
	circle(600, -115, 30);
	circle(640, -115, 30);
	circle(680, -115, 30);

	//treasure box
	glColor3ub(186, 140, 99);
	glRectf(boxX, boxY - 20, boxX - 20, boxY);
	glColor3ub(129, 84, 56);
	glRectf(boxX - 15, boxY - 15, boxX - 5, boxY - 5);
}

void drawPlayer(player_t p) //UFO Rotaing in a circle
{
	float r = 150; //amplitude

	p.pos.x = (r * cos(angle * D2R));
	p.pos.y = r * sin(angle * D2R) + 25;

	//circle
	glColor3ub(179, 218, 101);
	circle(p.pos.x, p.pos.y, 15);

	//rectangle
	glColor3ub(105, 105, 105);
	glBegin(GL_POLYGON);
	glVertex2f(p.pos.x - 13, p.pos.y); // top left	
	glVertex2f(p.pos.x - 28, p.pos.y - 18); // bottom left
	glVertex2f(p.pos.x + 28, p.pos.y - 18); // bottom right
	glVertex2f(p.pos.x + 13, p.pos.y); // top right	
	glEnd();
}

void drawEnemy(enemy_t e) //Spaceship docking in UFO
{
	//top 	
	glColor3ub(105, 105, 105);
	glBegin(GL_POLYGON);
	glVertex2f(e.pos.x-2, e.pos.y + 5); // top left
	glVertex2f(e.pos.x - 6, e.pos.y - 3); // bottom left
	glVertex2f(e.pos.x + 6, e.pos.y - 3); // bottom right
	glVertex2f(e.pos.x+2, e.pos.y + 5); // top left
	glEnd();

	//bottom rectangle
	glColor3ub(56, 56, 56);
	glBegin(GL_POLYGON);
	glVertex2f(e.pos.x - 6, e.pos.y - 3); // top left	
	glVertex2f(e.pos.x - 6, e.pos.y - 10); // bottom left
	glVertex2f(e.pos.x + 6, e.pos.y - 10); // bottom right
	glVertex2f(e.pos.x + 6, e.pos.y - 3); // top right	
	glEnd();

}

void display()
{
	// clear window to black
	glClearColor(1, 1, 1, 0.5);
	glClear(GL_COLOR_BUFFER_BIT);

	drawBackground();
	if (isFired == true)
		drawEnemy(E);
	drawPlayer(P);

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
	case GLUT_KEY_UP: up = true; break;
	case GLUT_KEY_DOWN: down = true; break;
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
	x = x - winWidth / 2;
	y = winHeight / 2 - y;
	// Write your codes here.
	if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN) {
		if (isFired == false)
		{
			isFired = true;
			E.pos.x = x;
			E.pos.y = y;
			E.posE.x = x;
			E.posE.y = y;
		}
	}
	// to refresh the window it calls display() function
	glutPostRedisplay();
}

// distance between two points.
double dist(point_t p1, point_t p2) {
	return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
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

void onMove(int x, int y) {
	// Write your codes here.



	 // to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.

	//brown box
	if (boxY > -290)
		boxY--;

	angle += 0.7;

	float r = 150;
	P.pos.x = (r * cos(angle * D2R));
	P.pos.y = r * sin(angle * D2R) + 25;
	P.posP.x = (r * cos(angle * D2R));
	P.posP.y = r * sin(angle * D2R) + 25;;

	// flee or chase
	// velocity vector from Enemy to Player
	if (isFired == true)
	{
		E.vel = mulV(ENEMY_SPEED, unitV(subV(P.pos, E.pos)));
		E.pos = addV(E.pos, E.vel); // chase
		E.posE.x = E.pos.x;
		E.posE.y = E.pos.y;
	}

	double d = dist(P.posP, E.posE);
	if (d < 10) {
		isFired = false;
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


void main(int argc, char* argv[])
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Docking Spaceship in UFO");

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