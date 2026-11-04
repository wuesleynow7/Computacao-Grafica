// gcc bilhar.c -o bilhar -lGL -lGLU -lglut -lm

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <stdbool.h>

 const double ball_radius = 0.7; // usar estaticas para evitar visibilidade externa
 double ball_pos[3] = { -5.0, 0.0, 0.0 };
 double ball_vel[3] = { 3.0, 5.0, 0.0 }; // modifique para incluir varias bolas

 const double wall_x = 6.0;   
 const double wall_left_x = -6.0;
 const double wall_up_y = 6.0;
 const double wall_down_y = -6.0;

 const double cyl_radius = 3.0;
 const double dt = 0.05;

 GLUquadric *quad;

int window_width  = 800;
int window_height = 600;

GLfloat light0_position[] = { 50.0f, 50.0f, 50.0f, 1.0f };
GLfloat light0_ambient[]  = { 0.15f, 0.15f, 0.15f, 1.0f };
GLfloat light0_diffuse[]  = { 0.80f, 0.80f, 0.80f, 1.0f };
GLfloat light0_specular[] = { 1.00f, 1.00f, 1.00f, 1.0f };
GLfloat global_ambient[]  = { 0.05f, 0.05f, 0.05f, 1.0f };

GLfloat mat_ambient[]   = { 0.1f, 0.1f, 0.2f, 1.0f };
GLfloat mat_diffuse[]   = { 0.2f, 0.3f, 0.8f, 1.0f };
GLfloat mat_specular[]  = { 0.9f, 0.9f, 0.9f, 1.0f };
GLfloat mat_shininess[] = { 64.0f };


float theta=0.0;

void meuinit(void)
{
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
    glEnable(GL_COLOR_MATERIAL);
}


void paralelepipedo(double cx, double cy, double cz, double sx, double sy, double sz, float r, float g, float b) {
    glPushMatrix();
    glTranslatef((float)cx, (float)cy, (float)cz);
    glScalef((float)sx, (float)sy, (float)sz);
    glColor3f(r,g,b);
    glutSolidCube(1.0);
    glPopMatrix();
}

void desenhaCaixa() {
      paralelepipedo(0.0, 0.0, -0.7, 12.0, 12.0, 0.2, 0.0f, 0.6f, 0.0f);
    paralelepipedo(6.0, 0.0, 0.0, 0.2, 12.0, 4.0, 0.0f, 0.6f, 0.0f);   
    paralelepipedo(-6.0, 0.0, 0.0, 0.2, 12.0, 4.0, 0.0f, 0.6f, 0.0f);  
    paralelepipedo(0.0, 6.0, 0.0, 12.0, 0.2, 4.0, 0.0f, 0.6f, 0.0f);   
    paralelepipedo(0.0, -6.0, 0.0, 12.0, 0.2, 4.0, 0.0f, 0.6f, 0.0f);  
}

void desenhaCilindro() {
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
//    glRotatef(-90.0f, 1, 0, 0); 
    gluCylinder(quad, cyl_radius, cyl_radius, 3.0, 48, 1);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 3.0f);
    gluDisk(quad, 0.0, cyl_radius, 48, 1);
    glPopMatrix();
    glPushMatrix();
    gluDisk(quad, 0.0, cyl_radius, 48, 1);
    glPopMatrix();

    glPopMatrix();
}

void desenhaBola() {
    glPushMatrix();
    glTranslatef((float)ball_pos[0], (float)ball_pos[1], (float)ball_pos[2]);
    glColor3f(1.0f, 0.0f, 0.0f);
    glutSolidSphere((float)ball_radius, 36, 36);
    glPopMatrix();
}

void anima() {
    double prev_x = ball_pos[0];
    double prev_y = ball_pos[1];

    ball_pos[0] += ball_vel[0] * dt;
    ball_pos[1] += ball_vel[1] * dt;
    ball_pos[2] += ball_vel[2] * dt;

        if (ball_pos[0] > wall_x - ball_radius) {
        ball_pos[0] = wall_x - ball_radius;
        ball_vel[0] = -ball_vel[0];
    }
    if (ball_pos[0] < wall_left_x + ball_radius) {
        ball_pos[0] = wall_left_x + ball_radius;
        ball_vel[0] = -ball_vel[0];
    }
    if (ball_pos[1] > wall_up_y - ball_radius) {
        ball_pos[1] = wall_up_y - ball_radius;
        ball_vel[1] = -ball_vel[1];
    }
    if (ball_pos[1] < wall_down_y + ball_radius) {
        ball_pos[1] = wall_down_y + ball_radius;
        ball_vel[1] = -ball_vel[1];
    }

     double r = sqrt(ball_pos[0]*ball_pos[0] + ball_pos[1]*ball_pos[1]);
    double v = sqrt(ball_vel[0]*ball_vel[0] + ball_vel[1]*ball_vel[1]);

    if (r <= (cyl_radius + ball_radius/2.0)) {
        double dx = prev_x;
        double dy = prev_y;
        double teta = atan2(dy, dx);
        double tetav = atan2(ball_vel[1], ball_vel[0]);
        double phi = tetav - teta;

        double radial = -v * cos(phi);
        double tangential = v * sin(phi);

        double vx = radial * cos(-teta) + tangential * sin(-teta);
        double vy = -radial * sin(-teta) + tangential * cos(-teta);

        ball_vel[0] = vx;
        ball_vel[1] = vy;

         double push = (cyl_radius + ball_radius/2.0) - r + 1e-6;
        if (push > 0.0) {
            double nx = ball_pos[0] / (r + 1e-9);
            double ny = ball_pos[1] / (r + 1e-9);
            ball_pos[0] += nx * push;
            ball_pos[1] += ny * push;
        }

        ball_pos[0] += ball_vel[0] * dt;
        ball_pos[1] += ball_vel[1] * dt;
    }
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

   
    gluLookAt(0.0, -18.0, 18.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);   

    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);


    desenhaCaixa();
    desenhaCilindro();
    desenhaBola();

    glutSwapBuffers();
}

void idle(void) {

    anima();
    glutPostRedisplay();

}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w/(double)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {
    quad = gluNewQuadric();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(900, 700);
    glutCreateWindow("Bilhar de Sinai com uma única bola");

    glClearColor(0.8f, 0.9f, 1.0f, 1.0f);
    meuinit();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

    glutMainLoop();

    gluDeleteQuadric(quad);
    return 0;
}