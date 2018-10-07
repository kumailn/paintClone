#include <stdlib.h>
#include <gl/glut.h>
#include <gl/gl.h>
#include <stdio.h>
int x = 0;
int y = 0;
int r = 0;
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
	// Translate the Square
	glRotatef(40, 0, 0, 1);
	glScalef(5, 5, 1);
	glBegin(GL_POLYGON);
		glVertex2f(-0.5, -0.5);
		glVertex2f(-0.5, 0.5);
		glVertex2f(0.5, 0.5);
		glVertex2f(0.5, -0.5);	
	glEnd();
	glFlush();
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

/* main function - program entry point */
int main(int argc, char** argv)
{
	glutInit(&argc, argv);		//starts up GLUT
	glutCreateWindow("square");	//creates the window
	glutDisplayFunc(display);	//registers "display" as the display callback function
	//glutTimerFunc(0, FPS, 0);
	//glutIdleFunc(idle);
	gluOrtho2D(-10, 10, -10, 10);
	glutMainLoop();				//starts the event loop
	return(0);					//return may not be necessary on all compilers
}

