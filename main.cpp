#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include "boilerplateClass.h"	//include our own class

//Globals
float camPos[] = {0, 0, 3.42f};	//where the camera is
float position = 0;
float angle = 0;

//OpenGL functions
/* display function - GLUT display callback function
 *		clears the screen, sets the camera position, draws the ground plane and movable box
 */
void display(void){
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camPos[0], camPos[1], camPos[2], 0, 0, 0, 0,1,0);
	glColor3f(1,1,1);
	glRotatef(angle, 1, 1, 0);
	angle++;
	////draw the teapot
	glutSolidTeapot(1);
	glutSwapBuffers();
	//swap because using 2 buffers

	//flush out to single buffer
	//glFlush();
}

void keyboard(unsigned char key, int xIn, int yIn){
	switch (key)
	{
		case 'q':
		case 27:
			exit(0);
			break;
	}
}

void special(int x, int y, int z){
	
}

void mouse(int btn, int sytate, int x, int y){
	if(btn == GLUT_LEFT_BUTTON){
		printf("left button, %i, %i\n", x, y);
	}
}

void motion(int x, int y){
	printf("left button, %i, %i\n", x, y);
}

void passive(int x, int y){
	printf("passive drag, %i, %i\n", x, y);
}

void reshape(int w, int h){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, ((w+0.0f)/h), 1, 100);

	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, w, h);
}

void FPS(int val){
	glutPostRedisplay();
	glutTimerFunc(17, FPS, 0);
}

void init(void){
	glClearColor(0, 0, 0, 0);
	glColor3f(1, 1, 1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, 100);
}

void callBackInit(){
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(passive);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, FPS, 0);
}

/* main function - program entry point */
int main(int argc, char** argv){
	glutInit(&argc, argv);		//starts up GLUT
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

	glutInitWindowSize(400, 400);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("3GC3 Boilerplate");	//creates the window

	callBackInit();

	init();

	glutMainLoop();				//starts the event glutMainLoop
	return(0);					//return may not be necessary on all compilers
}