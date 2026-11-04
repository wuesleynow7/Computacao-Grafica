#include <GL/glut.h>
void display(void) {
glClear(GL_COLOR_BUFFER_BIT);
glColor3f(1.0, 0.0, 0.0); // Vermelho
glBegin(GL_POLYGON);
glVertex2f(-0.5, -0.5);
glVertex2f(0.5, -0.5);
glVertex2f(0.5, 0.5);
glVertex2f(-0.5, 0.5);
glEnd();
glFlush();
}

void myinit(void)
{
GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };

glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
glLightfv(GL_LIGHT0, GL_POSITION, light_position);

glEnable(GL_LIGHTING);
glEnable(GL_LIGHT0);
glEnable(GL_DEPTH_TEST);
glShadeModel (GL_SMOOTH);
}

int main(int argc, char** argv) {
glutInit(&argc, argv);
glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
glutInitWindowSize(400, 400);
glutCreateWindow("Exemplo OpenGL");
glClearColor(0.0, 0.0, 0.0, 0.0);
myinit();
glutDisplayFunc(display);
glutMainLoop();
return 0;
}