#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#define color4 GLfloat
using namespace std;

GLfloat light0_ambient[] =
{0.2, 0.2, 0.2, 1.0};
GLfloat light0_diffuse[] =
{0.0, 0.0, 0.0, 1.0};
GLfloat light1_diffuse[] =
{1.0, 1.0, 1.0, 1.0};
GLfloat light1_position[] =
{0.0, 0.0, -10.0, 1.0};
GLfloat light2_diffuse[] =
{1.0, 1.0, 1.0, 1.0};
GLfloat light2_position[] =
{0.0, 50.0, 0.0, 1.0};
GLfloat light2_spot[] =
{0.0, 0.0, 0.0, 1.0};
GLfloat globalAmbient[] = {0.0, 0.0, 0.0, 0.0};

float s = 0.0;
GLfloat theta = 0.0;
float alpha=0.0;
float tr_x=0.0;
float tr_y=0.0;
float tr_z=0.0;


clock_t counter1, counter2;

GLuint texture[6]; //the array for our texture

int i;

GLuint LoadTexture2( const char * filename, int width, int height )
{

    /* FUNCIONA APENAS COM PPM ASCII GERADO SEM COMENTARIOS, COMO NO CASO DO IMAGEMAGICK */

    GLuint texture;
    unsigned char * data;
    FILE * file;
    char linha2[255];
    int r,g,b;

    file = fopen( filename, "rb" );
    if ( file == NULL ) return 0;
    data = (unsigned char *)malloc( sizeof(char)*width * height * 3 );

    fgets(linha2,sizeof linha2, file);  // ignora P3
    fgets(linha2,sizeof linha2, file);  // ignora dimensoes
    fgets(linha2,sizeof linha2, file);  // ignora colordepth


    for (i=0; i<(width*height*3); i+=3)
    {
        fscanf(file,"%i %i %i",&r,&g,&b);
        data[i]=(unsigned char)r;
        data[i+1]=(unsigned char)g;
        data[i+2]=(unsigned char)b;
    }

    fclose( file );

    glGenTextures( 1, &texture ); //gera a textura
    glBindTexture( GL_TEXTURE_2D, texture ); // faz o binding da texture com o array

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE , GL_MODULATE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

    free( data );
    return texture;
}


void FreeTexture( GLuint texture )
{
    glDeleteTextures( 1, &texture );
}

static void idle(void)
{
    GLfloat seconds = glutGet(GLUT_ELAPSED_TIME)/1000.0;

    glutPostRedisplay();
}

void timer(int value)
{
    theta= (GLfloat) fmod(theta+ 2.0, 360.0);
    glutPostRedisplay();
    glutTimerFunc(10, timer, 0);
    counter2=clock()/CLOCKS_PER_SEC;
    if (counter2>10)tr_x=theta/10;
    if (counter2>20)tr_y=theta/10;
    if (counter2>40)tr_z=theta/10;
}

void cubo (float x, float y, float z,float l)
{

    glBindTexture( GL_TEXTURE_2D, texture[0] );

    glBegin (GL_POLYGON);
    glNormal3f(0.0,0.0,-1.0);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-l+x, -l+y, -l+z);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-l+x, l+y, -l+z);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(l+x, l+y, -l+z);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(l+x, -l+y, -l+z);
    glEnd();

    glBindTexture( GL_TEXTURE_2D, texture[1] );

    glBegin (GL_POLYGON);
    glNormal3f(0.0,0.0,1.0);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-l+x,-l+y, l+z);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(l+x, -l+y, l+z);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(l+x, l+y, l+z);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(-l+x, l+y, l+z);
    glEnd();


    glBindTexture( GL_TEXTURE_2D, texture[2] );
    glBegin (GL_POLYGON);
    glNormal3f(1.0,0.0,0.0);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(l+x,-l+y, -l+z);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(l+x, l+y, -l+z);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(l+x, l+y, l+z);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(l+x,-l+y, l+z);
    glEnd();

    glBindTexture( GL_TEXTURE_2D, texture[3] );
    glBegin (GL_POLYGON);
    glNormal3f(-1.0,0.0,0.0);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-l+x,-l+y, -l+z);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-l+x, -l+y, l+z);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(-l+x, l+y, l+z);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(-l+x, l+y, -l+z);

    glEnd();

    glBindTexture( GL_TEXTURE_2D, texture[4] );
    glBegin (GL_POLYGON);
    glNormal3f(0.0,-1.0,0.0);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-l+x, -l+y, -l+z);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(l+x, -l+y, -l+z);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(l+x, -l+y, l+z);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(-l+x, -l+y, l+z);
    glEnd();

    glBindTexture( GL_TEXTURE_2D, texture[5] );

    glBegin (GL_POLYGON);
    glNormal3f(0.0,1.0,0.0);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-l+x, l+y, -l+z);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-l+x, l+y, l+z);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(l+x, l+y, l+z);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(l+x,l+y, -l+z);
    glEnd();

}

void display(void)
{

    GLfloat rot_x=0.0, rot_y=0.0;

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glPushMatrix();
    glEnable(GL_LIGHT2);

    glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
    glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light2_spot);

    glEnable( GL_TEXTURE_2D );

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


    gluLookAt (0.0, 0.0, 25.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    //glTranslated(0,0,0);
    alpha=(theta/180.0)*3.14159265;

    s=alpha*2;

    glBindTexture( GL_TEXTURE_2D, texture[5] );
    glBegin (GL_POLYGON);
    glNormal3f(0.0,0.0,1.0);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-40.0, -20.0, -80.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-40.0, -20.0, 0.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(40.0, -20.0, 0.0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(40.0,-20.0, -80.0);
    glEnd();

    glRotatef(theta, 0.0f, 1.0f, 1.0f);
    glScalef (0.5, 0.5, 0.5);

    for (float th=0.0; th<6.28; th+=0.2)
        for (float ph=0.0; ph<3.14; ph+=0.2)
        {
            float scale=(20.0+10.0*sin(3.1415*theta/180.0));
            float x=scale*sin(th)*cos(ph);
            float y=scale*sin(th)*sin(ph);
            float z=scale*cos(th);
            cubo(x+tr_x,y+tr_y,z+tr_z,0.7);
        }

    glPushMatrix();

    glRotatef(-theta, 1.0f, 0.0f, 1.0f);
    cubo(0,0,0,8.0);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    glFlush();
    glutSwapBuffers();
}


void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-10,10,-10,10,-10,10);
    gluPerspective(45.0, float(w)/float(h), -10, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}


void myinit()
{
    glShadeModel(GL_FLAT);
    glClearDepth(100.0f);  ///*******
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);  ///*******
    glEnable (GL_TEXTURE_2D );
	
    glFrontFace(GL_CW);    //
    glCullFace(GL_FRONT);    //  Estas tres fazem o culling funcionar
    glEnable(GL_CULL_FACE); //

    glDepthFunc(GL_LEQUAL);  ///*******
    glDepthRange(50,-50);    ///*******
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glDepthMask(GL_TRUE);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glColor3f(0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);  ///******* NA DISPLAY

}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode( GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH );

    glutInitWindowSize(1280, 1024);
    glutCreateWindow("cubos");

    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);

    counter1 = clock ()/CLOCKS_PER_SEC;

    texture[5]=LoadTexture2("unipampa.ppm",500,400);
    texture[4]=LoadTexture2("gr.ppm",250,254);
    texture[3]=LoadTexture2("fray.ppm",256,256);
    texture[2]=LoadTexture2("mgp.ppm",256,256);
    texture[1]=LoadTexture2("et.ppm",400,400);
    texture[0]=LoadTexture2("MJ.ppm",400,316);
 // glutFullScreen();
    myinit();
    glutTimerFunc(10, timer, 0);

    glutMainLoop();
    return 0;
}

/*
tarefas
1-carregar outras texturas diferentes
2-modifique o codigo  para apresentar arquivo obj com as mesmas texturas para todas as faces
3-carregar um arquivo obj com coordenadas de textura e apresentar  o objeto com a sua textura
*/