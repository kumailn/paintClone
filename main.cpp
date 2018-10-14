#include <stdlib.h>
#include <gl/glut.h>
#include <gl/gl.h>
#include <stdio.h>
int x = 0;
int y = 0;
int r = 0;

#define RED 1;

enum State {Line, Point, Square, Circle};
enum State currentState; 

int mousePressed = 0;
int windowWidth=800,windowHeight=800;
int mouseX1=0,mouseY1=0,mouseX2,mouseY2;

//Structs for each type of shape
struct Pointt {
   double x, y; 
};

struct Line {
   double x1, y1, x2, y2; 
};

struct Square {
	double x1, y1, x2, y2, x3, y3, x4, y4;
}

//Arrays to hold all shape positions, along with a counter for their length
Pointt points[100];
int numberOfPoints = 0;

Line lines[100];
int numberOfLines = 0;

Square squares[100];
int numberOfSquares = 0;





void createLine(){
	glLineWidth(2.5); 
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(3.0, 0.0, 0.0);
	glVertex3f(15, 3, 0);
	glEnd();
}

void axis(int size){
	glBegin(GL_LINES);
		glColor3f(1, 0, 0);
		glVertex2f(-size, 0);
		glVertex2f(size, 0);
		glColor3f(0, 1, 0);
		glVertex2f(0, -size);
		glVertex2f(0, size);
		//glColor3f(0, 1, 1);
		//glVertex2f(3, 6);
		for(int i = -size; i < size; i++){
			glVertex2f(i, 0.5);
			glVertex2f(i, -0.5);
		};
		for(int i = -size; i < size; i++){
			glVertex2f(-0.5, i);
			glVertex2f(0.5, i);
		};
	glEnd();
	r++;
}	

void drawPoint(int x, int y){
	printf("drawing point %i %i \n", x, y);
	glPointSize(5);
	glBegin(GL_POINTS);
	glVertex2i(x , y);
	glEnd();
	glutSwapBuffers();
}

void drawPoints(){
	for(int i = 0; i < numberOfPoints; i++ ){
		glPointSize(5);
		glBegin(GL_POINTS);
		glVertex2i(points[i].x , points[i].y);
		glEnd();
	}
	glutSwapBuffers();
}

void drawLine(int x1,int x2, int y1, int y2){
	if (x2 < x1) {  // Line needs to be drawn in reverse 
		printf("Reverse");
		return drawLine(x2, y2, x1, y1); 
	}
	int dx = x2 - x1;
	int dy = y2 - y1;
	int d = 2*dy - dx;
	float incrE = dy*2;
	float incrNE = 2*dy - 2*dx;
	int x = x1;
	int y = y1;
	drawPoint(x, y);
	while (x < x2) {
		if (d <= 0) {  
			d += incrE;
			x++;
			if (y2 <= y1) y--;
		} else {
			d += incrNE;
			x++;
			y++; 
		} 
		drawPoint(x, y);	
	}
}

/* display function - GLUT display callback function
 *		clears the screen, draws a square, and displays it
 */
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Draw axis
		//glOrtho(0.0f, 800, 800, 0.0f, 0.0f, 1.0f);

	//axis(100);
	drawPoint(200, 200);
	drawPoints();
	glPointSize(2);
	glBegin(GL_POINTS);
	glVertex2f(0 , 0);
	//drawLine(1, 1, 30, 30);
	glEnd();
	glutSwapBuffers();

	// Translate the Square	

/* 	glRotatef(40, 0, 0, 1);
	glScalef(5, 5, 1);
	glBegin(GL_POLYGON);
		glVertex2f(-0.5, -0.5);
		glVertex2f(-0.5, 0.5);
		glVertex2f(0.5, 0.5);
		glVertex2f(0.5, -0.5);	
	glEnd();
	glFlush(); */
}


void mouseClick(int btn, int state, int x, int y){
	//float mouseX = (x / 150) - 0.5f;
	//float mouseY = (y / 150) - 0.5f;
	static int moveX1=0,moveY1=0;
	if(btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		printf("Dragging %i, %i \n", x, y);
		mousePressed = 1;

	}
	else if(btn == GLUT_LEFT_BUTTON){
		printf("left button, %i, %i\n", x, y);
		if(currentState == Square){
			printf("sup");
		}
		else if(currentState == Circle){
			printf("circl");
		}
		else if(currentState == Point){
			points[numberOfPoints].x = x;
			points[numberOfPoints].y = y;
			numberOfPoints++;
			drawPoints();
			//drawPoint(x, y);
			glutSwapBuffers();
		}
	}
	else if(btn == GLUT_LEFT_BUTTON && state == GLUT_UP){
		mousePressed = 0;
	}
}

void handleMenuClicks(int option) {
	switch (option) {
		case 0:
			printf("Clicked point \n");
			currentState = Point;
			break;
		case 1:
			printf("Clicked line \n");
			currentState = Line;
			break;
		case 2:
			printf("Clicked square \n");
			currentState = Square;
			printf("%d", currentState);
			break;
		case 3:
			printf("Clicked rectangle \n");
			//currentState = Rectangle;
			break;
		case 4:
			printf("Clicked circle \n");
			currentState = Circle;
			break;
	}
}

void createGLUTMenus() {

	int menu;

	// create the menu and
	// tell glut that "processMenuEvents" will
	// handle the events
	menu = glutCreateMenu(handleMenuClicks);

	//add entries to our menu
	glutAddMenuEntry("Line",1);
	glutAddMenuEntry("Square",2);
	glutAddMenuEntry("Point",0);
	glutAddMenuEntry("Rectangle",3);
	glutAddMenuEntry("Circle",4);
	// attach the menu to the right button
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void idle(void){
	for(int i = 0; i < 99999999; i++){};
	printf("re-rendering \n");
	display();
};

void FPS(int val){
	glutPostRedisplay();
	glutTimerFunc(17, FPS, 0);
}

void reshape(int w, int h)
{
	glutReshapeWindow( 300, 300);
}

void mouseMotion(int x, int y){
	printf("2Dragging %i, %i \n", x, y);
}


/* main function - program entry point */
int main(int argc, char** argv)
{
	glutInit(&argc, argv);		//starts up GLUT
	glutCreateWindow("Paint Clone");	//creates the window
	glutDisplayFunc(display);	//registers "display" as the display callback function
	glutInitDisplayMode(GLUT_DOUBLE);
	glutReshapeWindow(800, 800);
	createGLUTMenus();
	glutMotionFunc(mouseMotion);
	glutMouseFunc(mouseClick);
	//glutTimerFunc(0, FPS, 0);
	//glutIdleFunc(idle);
	glMatrixMode(GL_PROJECTION);
	//glutReshapeFunc(reshape);
	glOrtho(0.0f, 800, 800, 0.0f, 0.0f, 1.0f);
	//gluOrtho2D(-10, 10, -10, 10);
	glutMainLoop();				//starts the event loop
	return(0);					//return may not be necessary on all compilers
}

