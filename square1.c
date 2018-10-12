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

void createLine(){
	glLineWidth(2.5); 
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(3.0, 0.0, 0.0);
	glVertex3f(15, 3, 0);
	glEnd();
}

void drawPoint(int x, int y){
	printf("pointing");
	glPointSize(2);
	glBegin(GL_POINTS);
	glVertex2f(50 , 22);
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

/* display function - GLUT display callback function
 *		clears the screen, draws a square, and displays it
 */
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	// Draw axis
	axis(10);
	glPointSize(2);
	glBegin(GL_POINTS);
	glVertex2f(0 , 0);
	glEnd();
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
			drawPoint(x, y);
			display();
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
	createGLUTMenus();

	glutMotionFunc(mouseMotion);
	glutMouseFunc(mouseClick);
	//glutTimerFunc(0, FPS, 0);
	//glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	gluOrtho2D(-10, 10, -10, 10);
	glutMainLoop();				//starts the event loop
	return(0);					//return may not be necessary on all compilers
}

