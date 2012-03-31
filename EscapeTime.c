/*
 * EscapeTime.c
 *
 * Author - Robert Horswell
 *
 * A program running an escape time algorithm to draw Julia sets of functions.
 *
 * Last rewritten 31/03/2012
 * 
 */

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif // OS

#include <stdlib.h>
#include <stdio.h>
#include "complex.h"
#include "colourdataf.h"

#define INIT_WIN_WIDTH				640
#define INIT_WIN_HEIGHT				480
#define MAX_ITERATIONS				250
#define INIT_COMPLEX_DOMAIN_WIDTH	3.3
#define INIT_COMPLEX_DOMAIN_HEIGHT	2.8
#define ESCAPE_RADIUS				100

int mode;
int n1, n2;
int iters, dir, cimax;
double zoom;
int *fracData = NULL;
size_t fracData_len;
int animToggle;
Complex JULIA;
Complex trans;
const float *cmap;

inline void screenToComplex (int sx, int sy, Complex *z) {

	double complexHeight = INIT_COMPLEX_DOMAIN_WIDTH * (double)n2 / (double)n1;
	Complex sub = newComplex (INIT_COMPLEX_DOMAIN_WIDTH / 2, complexHeight / 2);
	*z = newComplex (INIT_COMPLEX_DOMAIN_WIDTH * (double)(sx) / (double)(n1), complexHeight * (double)(sy) / (double)(n2));
	*z = minusComplex (z, &sub);
	*z = rmultComplex (zoom, z);
	*z = addComplex (z, &trans);
}

void allocData () {

	fracData_len = n1 * n2;
	if (fracData != NULL) free (fracData);
	fracData = (int *)malloc (fracData_len * sizeof (int));
	if (fracData == NULL) exit (-1);
}

void generateData () {

	int k;
	int sx;
	int sy;
	Complex z, w, *c;
	if (fracData == NULL) allocData ();
	for (sx = 0; sx < n1; ++sx) {
		for (sy = 0; sy < n2; ++sy) {
			screenToComplex (sx, sy, &w);
			z = w;
			if (mode== 1) c = &w;
			else c = &JULIA;
			for (k = 0; k < MAX_ITERATIONS; ++k) {
				z = cmultComplex (&z, &z);
				z = cmultComplex (&z, &z);
				z = addComplex (&z, c);
//				z = sinComplex (&z);
//				z = cmultComplex (c, &z);
//				z.real = c->imaginary + z.real;
//				z.imaginary = c->real - z.imaginary;
//				z = cmultComplex (c, &z);
				if (z.real * z.real + z.imaginary * z.imaginary > ESCAPE_RADIUS) break;
			}
			fracData[(sx * n2) + sy] = k;
		}
	}
}

void idle (void) {
	iters += dir;
	if (iters >= cimax) iters = 0;
	else if (iters < 0) iters = cimax - 1;
	glutPostRedisplay (); 
}

inline void getColour (int k) {

	if (k == MAX_ITERATIONS) glColor3f (0.0f, 0.0f, 0.0f);
	else {
	 	int ci;
		float cr;
		float cg;
		float cb;
		ci = k + iters;
		ci %= cimax;
		cr = *(cmap + (3 * ci));
		cg = *(cmap + (3 * ci) + 1);
		cb = *(cmap + (3 * ci) + 2);
		glColor3f (cr, cg, cb);
	}
}

void display (void) {

 	int k;
 	int sx;
 	int sy;
	glClear (GL_COLOR_BUFFER_BIT);
	if (fracData == NULL) return;
	glBegin (GL_POINTS);
	for (sx = 0; sx < n1; ++sx) {
		for (sy = 0; sy < n2; ++sy) {
			k = fracData[(sx * n2) + sy];
			getColour (k);
			glVertex2i (sx, sy);
		}
	}
	glEnd ();
	glutSwapBuffers ();
}

void reshape (int width, int height) {

	if (height <= 0) height = 1;
	if (width <= 0) width = 1;	
	n1 = width;
	n2 = height;
	if ((n1 * n2) > fracData_len) allocData ();
	generateData ();
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D (0, width, 0, height);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	glViewport (0, 0, (GLsizei)width, (GLsizei)height);
}

void mouse (int button, int state, int x, int y) {

	static int clickx;
	static int clicky;
	switch (button) {
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN) {
				Complex c;
				screenToComplex (x, (n2 - y - 1), &c);
				if (glutGetModifiers () == GLUT_ACTIVE_SHIFT) {
					if (mode== 1) {
						mode= 0;
						JULIA = c;
					}
					else {
						mode= 1;
					}
					trans = newComplex (0, 0);
					zoom = 1;
				}
				else {
					trans = c;
					if (glutGetModifiers () == GLUT_ACTIVE_CTRL) zoom /= 0.7;
					else zoom *= 0.7;
				}
				generateData ();
				glutPostRedisplay ();
			}
			break;
		case GLUT_RIGHT_BUTTON:
			if (state == GLUT_DOWN) {
				clickx = x;
				clicky = y;
			}
			else {
				Complex c;
				Complex r;
				screenToComplex (clickx, (n2 - clicky - 1), &c);
				screenToComplex (x, (n2 - y - 1), &r);
				trans = addComplex (&trans, &c);
				trans = minusComplex (&trans, &r);
				generateData ();
				glutPostRedisplay ();
			}
		default:
			break;
	}
}

void keyboard (unsigned char key, int x, int y) {

	switch (key) {
		case 27:
			exit(0);
			break;
		case 'P':
		case 'p':
			if (animToggle == 1) glutIdleFunc (idle);
			else glutIdleFunc (NULL);
			animToggle = -animToggle;
			break;
		case 'R':
		case 'r':
			trans = newComplex (0, 0);
			zoom = 2;
			generateData ();
			glutPostRedisplay ();
			break;
		case 'A':
		case 'a':
			dir = -dir;
			break;
		case '1':
			cmap = goodega;
			cimax = goodega_len;
			glutPostRedisplay ();
			break;
		case '2':
			cmap = chroma;
			cimax = chroma_len;
			glutPostRedisplay ();
			break;
		case '3':
			cmap = blues;
			cimax = blues_len;
			glutPostRedisplay ();
			break;
		case '4':
			cmap = volcano;
			cimax = volcano_len;
			glutPostRedisplay ();
			break;
		case '5':
			cmap = fractintdefault;
			cimax = fractintdefault_len;
			glutPostRedisplay ();
			break;
		case 'V':
		case 'v':
			if (mode== 1) printf("Parameter space\n");
			else printf("c = %f + %fi\n", JULIA.real, JULIA.imaginary);
			printf ("Zoom: %f\n", zoom);
			printf ("Centre: %f + %fi\n", trans.real, trans.imaginary);
			printf ("Size: %d x %d\n\n", n1, n2);
		default:
			break;
	}
}

void special (int key, int x, int y) {
	
	switch (key) {
		case GLUT_KEY_UP:
			trans.imaginary += (zoom * 0.1);
			break;
		case GLUT_KEY_DOWN:
			trans.imaginary -= (zoom * 0.1);
			break;
		case GLUT_KEY_LEFT:
			trans.real -= (zoom * 0.1);
			break;
		case GLUT_KEY_RIGHT:
			trans.real += (zoom * 0.1);
			break;
		case GLUT_KEY_PAGE_UP:
			zoom *= 0.7;
			break;
		case GLUT_KEY_PAGE_DOWN:
			zoom /= 0.7;
			break;
		default:
			return;
	}
	generateData ();
	glutPostRedisplay ();
}

void initialise (void) {

	glClearColor (0.0, 0.0, 0.0, 0.0);
	trans = newComplex (0, 0);
	zoom = 2.0;
	iters = 0;
	dir = 1;
	animToggle = 1;
	mode= 1;
	cimax = goodega_len;
	cmap = goodega;
}

int main (int argc, char **argv) {

	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize (INIT_WIN_WIDTH, INIT_WIN_HEIGHT);
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("Escape Time");
	glutDisplayFunc (display);
	glutReshapeFunc (reshape);
	glutMouseFunc (mouse);
	glutKeyboardFunc (keyboard);
	glutSpecialFunc (special);
 	initialise ();
	glutMainLoop ();
	return 0;
}
