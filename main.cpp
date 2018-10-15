#include <stdlib.h>
#include <gl/glut.h>
#include <gl/gl.h>
#include <stdio.h>
#include <math.h>      
int x = 0;
int y = 0;
int r = 0;

#define RED 1;

enum State {Line, Point, Square, Circle, Radial};
int currentState = Point; 

int CURRENT_COLOR = 0;
int BRUSH_SIZE = 1;
int mousePressed = 0;
int windowWidth = 800, windowHeight = 800;
int mouseX1=0,mouseY1=0,mouseX2,mouseY2;
int windowID;

int oneClick = 0;

//Structs for each type of shape
struct PointShape {
   double x, y;
   int color; 
};

struct LineShape {
   double x1, y1, x2, y2; 
};

struct SquareShape {
	double x1, y1, x2, y2, x3, y3, x4, y4;
};

struct CircleShape {
	double x, y, r;
};

struct ColorRGB {
	float r, g, b;
};

ColorRGB randomColor;
ColorRGB CURRENT_RBG_COLOR;

PointShape lineInitial;
PointShape lineEnd;

//Arrays to hold all shape positions, along with a counter for their length
PointShape points[10000];
int numberOfPoints = 0;

PointShape linePoints[10000];
int numberOfLinePoints = 0;

PointShape circlePoints[10000];
int numberOfCirclePoints = 0;

LineShape lines[100];
int numberOfLines = 0;

SquareShape squares[100];
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

float randomfloat(float a, float b)
{
    return ((b - a) * ((float)rand() / RAND_MAX)) + a;
}

void drawPoint(int x, int y, int col){
	//printf("drawing point %i %i \n", x, y);
	glPointSize(BRUSH_SIZE);
	if (col == 0) {CURRENT_RBG_COLOR.r = 1.0; CURRENT_RBG_COLOR.g = 1.0; CURRENT_RBG_COLOR.b = 1.0;}
	if (col == 1) {CURRENT_RBG_COLOR.r = 1.0; CURRENT_RBG_COLOR.g = 0.0; CURRENT_RBG_COLOR.b = 0.0;}
	if (col == 2) {CURRENT_RBG_COLOR.r = 1.0; CURRENT_RBG_COLOR.g = 1.0; CURRENT_RBG_COLOR.b = 0.0;}
	if (col == 3) {CURRENT_RBG_COLOR.r = 1.0; CURRENT_RBG_COLOR.g = 0.0; CURRENT_RBG_COLOR.b = 1.0;}
	if (col == 4) {CURRENT_RBG_COLOR.r = 1.0; CURRENT_RBG_COLOR.g = 1.0; CURRENT_RBG_COLOR.b = 1.0;}
	if (col == 5) {CURRENT_RBG_COLOR.r = 0.0; CURRENT_RBG_COLOR.g = 0.0; CURRENT_RBG_COLOR.b = 1.0;}
	if (col == 6) {CURRENT_RBG_COLOR.r = randomColor.r; CURRENT_RBG_COLOR.g = randomColor.g; CURRENT_RBG_COLOR.b = randomColor.b;}
	if (col == 7) {CURRENT_RBG_COLOR.r = randomfloat(0.0, 1.0); CURRENT_RBG_COLOR.g = randomfloat(0.0, 1.0); CURRENT_RBG_COLOR.b = randomfloat(0.0, 1.0);}
	glColor3f(CURRENT_RBG_COLOR.r, CURRENT_RBG_COLOR.g, CURRENT_RBG_COLOR.b);
	glBegin(GL_POINTS);
	glVertex2i(x , y);
	glEnd();
	//glutSwapBuffers();
}

void drawPoints(){
	for(int i = 0; i < numberOfPoints; i++ ){
		drawPoint(points[i].x, points[i].y, points[i].color);
	}
	//glutSwapBuffers();
}
void drawLine(int x1, int y1, int x2, int y2)
{
  bool up = (fabs(y2 - y1) > fabs(x2 - x1));
  if(up)
  {
	int temp = x1;
	x1 = y1;
	y1 = temp;

	int temp2 = x2;
	x2 = y2;
	y2 = temp2;
  }
 
  if(x1 > x2)
  {
	int temp = x1;
	x1 = x2;
	x2 = temp;

	int temp2 = y1;
	y1 = y2;
	y2 = temp2;
  }
 
  int dx = x2 - x1;
  int dy = fabs(y2 - y1);
  int e = dx / 2.0f;
  int yjump = (y1 < y2) ? 1 : -1;
  int y = (int)y1;
  int maxX = (int) x2;
  for(int x = (int) x1; x < maxX; x++) {
    if(up) {
	linePoints[numberOfLinePoints].x = y;
	linePoints[numberOfLinePoints].y = x;
	numberOfLinePoints++;    }
    else {
	linePoints[numberOfLinePoints].x = x;
	linePoints[numberOfLinePoints].y = y;
	numberOfLinePoints++;    }
    e -= dy;
    if(e < 0)
    {
        y += yjump;
        e += dx;
    }
  }
}
  
void drawLines(){
	//printf("Drawing all lines...%i %i \n", numberOfLines, numberOfLinePoints);
	for(int i = 0; i < numberOfLines; i ++){
		drawLine(lines[i].x1, lines[i].y1, lines[i].x2, lines[i].y2);
	}
	for(int i = 0; i < numberOfLinePoints; i ++){
		drawPoint(linePoints[i].x, linePoints[i].y, linePoints[i].color);
	}

}

void circleHelper(int xc, int yc, int x, int y) 
{ 
	circlePoints[numberOfCirclePoints].x = xc+x;
	circlePoints[numberOfCirclePoints].y = yc+y;
	circlePoints[numberOfCirclePoints].color = CURRENT_COLOR;
	numberOfCirclePoints++;

	circlePoints[numberOfCirclePoints].x = xc-x;
	circlePoints[numberOfCirclePoints].y = yc+y;
	circlePoints[numberOfCirclePoints].color = CURRENT_COLOR;
	numberOfCirclePoints++;

	circlePoints[numberOfCirclePoints].x = xc+x;
	circlePoints[numberOfCirclePoints].y = yc-y;
	circlePoints[numberOfCirclePoints].color = CURRENT_COLOR;
	numberOfCirclePoints++;

	circlePoints[numberOfCirclePoints].x = xc-x;
	circlePoints[numberOfCirclePoints].y = yc-y;
	circlePoints[numberOfCirclePoints].color = CURRENT_COLOR;
	numberOfCirclePoints++;

	circlePoints[numberOfCirclePoints].x = xc+y;
	circlePoints[numberOfCirclePoints].y = yc+x;
	circlePoints[numberOfCirclePoints].color = CURRENT_COLOR;
	numberOfCirclePoints++;

	circlePoints[numberOfCirclePoints].x = xc-y;
	circlePoints[numberOfCirclePoints].y = yc+x;
	circlePoints[numberOfCirclePoints].color = CURRENT_COLOR;
	numberOfCirclePoints++;

	circlePoints[numberOfCirclePoints].x = xc+y;
	circlePoints[numberOfCirclePoints].y = yc-x;
	circlePoints[numberOfCirclePoints].color = CURRENT_COLOR;
	numberOfCirclePoints++;

	circlePoints[numberOfCirclePoints].x = xc-y;
	circlePoints[numberOfCirclePoints].y = yc-x;
	circlePoints[numberOfCirclePoints].color = CURRENT_COLOR;
	numberOfCirclePoints++;
} 
  
  

void drawCircles(int xcenter, int yccenter, int r) { 
    int x = 0, y = r; 
    int d = 3 - 2 * r; 
    while (y >= x) 
    { 
        circleHelper(xcenter, yccenter, x, y); 
        x++; 
        if (d > 0) 
        { 
            y--;  
            d = d + 4 * (x - y) + 10; 
        } 
        else d = d + 4 * x + 6; 
        circleHelper(xcenter, yccenter, x, y); 
    } 
} 

void drawAllCircles(){
	for(int i = 0; i < numberOfCirclePoints; i++){
		drawPoint(circlePoints[i].x, circlePoints[i].y, circlePoints[i].color);
	}
}

void drawAllShapes(){
	drawPoints();
	drawLines();
	drawAllCircles();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawPoints();
	glPointSize(2);
	glBegin(GL_POINTS);
	glVertex2f(0 , 0);
	drawAllShapes();
	glEnd();
	glutSwapBuffers();
}

//Handle drawing shpaes on mouse clicks
void mouseClick(int btn, int state, int x, int y){
	static int moveX1=0,moveY1=0;
	//printf("Mouse click# %i \n", mousePressed);
	if (mousePressed == 2){
		if (currentState == Line) {
			//printf("Second Click! drawing line");
			mousePressed = 0;
			lines[numberOfLines].x1 = lineInitial.x;
			lines[numberOfLines].y1 = lineInitial.y;
			lines[numberOfLines].x2 = x;
			lines[numberOfLines].y2 = y;
			numberOfLines++;
			display();
			glutSwapBuffers();
		} else if (currentState == Circle) {
			//printf("Drawing circle..............\n");
			double radius = sqrt(pow(x - lineInitial.x, 2.0) + pow(y - lineInitial.y, 2.0));
			mousePressed = 0;
			drawCircles(lineInitial.x, lineInitial.y, radius);
			display();
		} 
		else if (currentState == Square) {
			mousePressed = 0;
			double distance = sqrt(pow(x - lineInitial.x, 2.0) + pow(y - lineInitial.y, 2.0));
			drawLine(lineInitial.x, lineInitial.y, lineInitial.x + distance, lineInitial.y);
			drawLine(lineInitial.x, lineInitial.y, lineInitial.x, lineInitial.y + distance);
			drawLine(lineInitial.x + distance, lineInitial.y + distance, lineInitial.x, lineInitial.y + distance);
			drawLine(lineInitial.x + distance, lineInitial.y, lineInitial.x + distance, lineInitial.y + distance);

/* 			lines[numberOfLines].x1 = lineInitial.x;
			lines[numberOfLines].y1 = lineInitial.y;
			lines[numberOfLines].x2 = lineInitial.x + distance;
			lines[numberOfLines].y2 = lineInitial.y;
			numberOfLines++;

			lines[numberOfLines].x1 = lineInitial.x;
			lines[numberOfLines].y1 = lineInitial.y;
			lines[numberOfLines].x2 = lineInitial.x;
			lines[numberOfLines].y2 = lineInitial.y + distance;
			numberOfLines++;

			lines[numberOfLines].x1 = lineInitial.x + distance;
			lines[numberOfLines].y1 = lineInitial.y + distance;
			lines[numberOfLines].x2 = lineInitial.x ;
			lines[numberOfLines].y2 = lineInitial.y + distance;
			numberOfLines++;

			lines[numberOfLines].x1 = lineInitial.x + distance;
			lines[numberOfLines].y1 = lineInitial.y;
			lines[numberOfLines].x2 = lineInitial.x + distance;
			lines[numberOfLines].y2 = lineInitial.y + distance;
			numberOfLines++; */
			//drawSquares(lineInitial.x, lineInitial.y, radius);
			glutPostRedisplay();
		} 
	}
	else if(btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		//printf("init click %i, %i \n", x, y);
		mousePressed = 1;
		lineInitial.x = x;
		lineInitial.y = y;
	}
	else if(btn == GLUT_LEFT_BUTTON && mousePressed == 1){
		//printf("Mouse released, %i, %i\n", x, y);
		if(currentState == Square){
			//printf("sup");
			mousePressed = 2;
		}
		else if(currentState == Circle){
			//printf("circl");
			mousePressed = 2;
		}
		else if(currentState == Line){
			//printf("line2321\n");
			mousePressed = 2;
		}
		else if(currentState == Point){
			points[numberOfPoints].x = x;
			points[numberOfPoints].y = y;
			points[numberOfPoints].color = CURRENT_COLOR;
			numberOfPoints++;
			display();
			//drawPoints();
			//drawPoint(x, y);
			//glutSwapBuffers();
			mousePressed = 0;
		}
	}
	else if(btn == GLUT_LEFT_BUTTON && state == GLUT_UP){
		if (currentState == Line){
		}
	}
}

void handleMenuClicks(int option) {
	switch (option) {
		case 0:
			//printf("Clicked point \n");
			currentState = Point;
			break;
		case 1:
			//printf("Clicked line \n");
			currentState = Line;
			break;
		case 2:
			//printf("Clicked square \n");
			currentState = Square;
			printf("%d", currentState);
			break;
		case 3:
			//printf("Clicked rectangle \n");
			//currentState = Rectangle;
			break;
		case 4:
			//printf("Clicked circle \n");
			currentState = Circle;
			break;
		case 7:
			//printf("Clicked Radial \n");
			currentState = Radial;
			break;
		case 5:
			//printf("Clear All \n");
			//currentState = Circle;
			memset(points, 0, sizeof(points));
			numberOfPoints = 0;
			memset(lines, 0, sizeof(lines));
			numberOfLines = 0;
			memset(linePoints, 0, sizeof(linePoints));
			numberOfLinePoints = 0;
			memset(squares, 0, sizeof(squares));
			numberOfSquares = 0;
			display();
			break;
		case 6:
			glutDestroyWindow(windowID);
			break;
	}
}

void mouseMotion(int x, int y){
	if (currentState != Point && currentState != Radial) return;
	if (currentState == Radial){
		printf("RADDD");

		if (true){
			int d = x + y;
			int nx = 2 * d - x;
			int ny = 2 * d - y;

			int e = - x + y;
			int mx = 2 * e - x;
			int my = 2 * e - y;

			int f = x;
			int ox = 2 * f - x;
			int oy = 2 * f - y;

			points[numberOfPoints].y = y;
			points[numberOfPoints].x = x;
			points[numberOfPoints].color = CURRENT_COLOR;
			numberOfPoints++;

			points[numberOfPoints].x = ny;
			points[numberOfPoints].y = nx;
			points[numberOfPoints].color = CURRENT_COLOR;
			numberOfPoints++;

			points[numberOfPoints].x = mx;
			points[numberOfPoints].y = my;
			points[numberOfPoints].color = CURRENT_COLOR;
			numberOfPoints++;

			points[numberOfPoints].x = ox;
			points[numberOfPoints].y = oy;
			points[numberOfPoints].color = CURRENT_COLOR;
			numberOfPoints++;
		}
		display();
	}
	else{
		//printf("2Dragging %i, %i \n", x, y);
		points[numberOfPoints].x = x;
		points[numberOfPoints].y = y;
		points[numberOfPoints].color = CURRENT_COLOR;
		numberOfPoints++;
		display();
	}
}

void handleColors(int option){
	if(option == 6){
		randomColor.r = randomfloat(0.0, 1.0);
		randomColor.g = randomfloat(0.0, 1.0);
		randomColor.b = randomfloat(0.0, 1.0);
	}
	CURRENT_COLOR = option;
}

void handleBrush(int option){
	BRUSH_SIZE = option;
}

void createGLUTMenus() {

	int menu;

	// create the menu and
	// tell glut that "processMenuEvents" will
	// handle the events

	int colorMenu = glutCreateMenu(handleColors);
	glutAddMenuEntry("Red", 1);
	glutAddMenuEntry("Yellow", 2);
	glutAddMenuEntry("Purple", 3);
	glutAddMenuEntry("Blue", 5);
	glutAddMenuEntry("White", 4);
	glutAddMenuEntry("Random", 6);
	glutAddMenuEntry("Rainbow", 7);

	int brushMenu = glutCreateMenu(handleBrush);
	glutAddMenuEntry("1", 1);
	glutAddMenuEntry("2", 2);
	glutAddMenuEntry("3", 3);
	glutAddMenuEntry("4", 4);
	glutAddMenuEntry("5", 5);
	glutAddMenuEntry("6", 6);

	menu = glutCreateMenu(handleMenuClicks);
	//add entries to our menu
	glutAddMenuEntry("Line",1);
	glutAddMenuEntry("Square",2);
	glutAddMenuEntry("Point",0);
	glutAddMenuEntry("Circle",4);
	glutAddMenuEntry("Radial",7);
	glutAddSubMenu("Color", colorMenu);
	glutAddSubMenu("Brush", brushMenu);
	glutAddMenuEntry("Clear All",5);
	glutAddMenuEntry("Quit",6);

	// attach the menu to the right button
	glutAttachMenu(GLUT_RIGHT_BUTTON);

}

void reshape(int w, int h)
{
	glutReshapeWindow( 300, 300);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);		//starts up GLUT
	windowID = glutCreateWindow("Assignment 1: Paint Program");	//creates the window
	glutDisplayFunc(display);	//registers "display" as the display callback function
	glutInitDisplayMode(GLUT_DOUBLE);
	glutReshapeWindow(800, 800);
	createGLUTMenus();
	CURRENT_RBG_COLOR.r = 1.0;
	CURRENT_RBG_COLOR.g = 1.0;
	CURRENT_RBG_COLOR.b = 1.0;
	glutMotionFunc(mouseMotion);
	glutMouseFunc(mouseClick);
	glMatrixMode(GL_PROJECTION);
	glOrtho(0.0f, 800, 800, 0.0f, 0.0f, 1.0f);
	printf("**********WELCOME TO PAINT**********\n");
	printf("All commands are located within the right click menu.\n");
	printf("Note: If a shape doesn't appear, try making another one.\n");
	glutMainLoop();				//starts the event loop
	return(0);					//return may not be necessary on all compilers
}

