#include <fstream>
#include <iostream>
//#include <windows.h>
// #include <string>
#include <vector>
#include <climits>
#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <cstring>
// #include <sstream>
// #include <algorithm>

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#define X 1.0
#define Y 1.0
#define Z 1.0


using namespace std;


struct face
{
    int I1, I2, I3;
    float zmedio;
    face(int a1, int a2, int a3)
    {
        I1=a1;
        I2=a2;
        I3=a3;
        zmedio=0.0;
    }
};

vector <float>v1;
vector <float>v2;
vector <float>v3;

vector <float> cx;
vector <float> cy;
vector <float> cz;

vector <float> Ni;
vector <float> Nj;
vector <float> Nk;

vector<face> faces;

int n_faces=0,n_vertices=0;
float minz=500.0, sc=1.0,x_0=0.0,y_0=0.0;
int flag=0;

// No Linux:
// g++ texturas.c -o texturas -lGL -lGLU -lglut -lm
// No Windows, use o script que estah no classroom.

GLfloat light0_ambient[] = {0.1, 0.1, 0.1, 1.0};
//{0.2, 0.2, 0.2, 1.0};
GLfloat light0_diffuse[] = {1.0, 1.0, 1.0, 0.0};
GLfloat light1_diffuse[] = {0.1, 0.1, 0.1, 1.0};
GLfloat light0_position[] = {0.0, 100.0, -100.0, 0.0};

GLfloat light1_position[] = {0.0, 0.0, 50.0, 1.0};
GLfloat light2_diffuse[] = {1.0, 1.0, 1.0, 1.0};
GLfloat light2_position[] = {0.0, -50.0, -50.0, 11.0};
GLfloat light2_lookat[] = {1.0, 0.0, 0.0, 0.0};
GLfloat specularLight[] = { 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat globalAmbient[] = {1.0, 1.0, 1.0, 1.0};
GLfloat light2_spot[] = {0.0, 0.0, 0.0, 1.0};
GLfloat cor_spec[] = {1.0,0.0,0.0};
GLfloat a[] = { 0.5, 0.0, 0.5, 00 };
GLfloat b[] = { 1.0, 1.0, 0.0, 00 };
float alpha,theta;

GLuint texture[6]; // array de texturas

int i;

GLuint LoadTexture2( const char * filename, int width, int height )
{

    /* FUNCIONA APENAS COM PPM ASCII GERADO SEM COMENTARIOS, COMO NO CASO DO IMAGEMAGICK */
    /* AINDA DEVE TER ALGUNS BUGS... */
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

void timer(int value)
{
    theta+=1.0;
    if (theta >= 360) theta=0.0;
    glutPostRedisplay();
    glutTimerFunc(10, timer, 0);
}

int carrega3d(string nome)
{
    float fx, fy, fz;
    float a1, a2, a3;

    char letra;

    n_vertices=0;
    n_faces=0;

    ifstream file;
    file.open("poly30x30.obj",ios::in);

    if (!file)
    {
        cout << "Problemas na leitura do arquivo" << endl;
        exit(430);
    }

    for (int i =0; i<0; i++)
        file.ignore(INT_MAX, '\n');


    while (!file.eof())
    {
        file >> letra;

        if (letra=='v')
        {
            file >> fx;
            file >> fy;
            file >> fz;
            cx.push_back(fx);
            cy.push_back(fy);
            cz.push_back(fz);
            if (fz<minz)minz=fz;
            n_vertices++;

        }

        if (letra=='f')
        {
            file >> a1;
            file >> a2;
            file >> a3;
            v1.push_back(a1-1);
            v2.push_back(a2-1);
            v3.push_back(a3-1);
            faces.push_back(face(a1-1,a2-1,a3-1));
            n_faces++;

        }
        else
        {
            file.ignore(INT_MAX, '\n');
        }
    }


    for (int i=0; i<n_faces; i++)
    {
        double Px=cx[faces[i].I2]-cx[faces[i].I1];
        double Py=cy[faces[i].I2]-cy[faces[i].I1];
        double Pz=cz[faces[i].I2]-cz[faces[i].I1];

        double Qx=cx[faces[i].I3]-cx[faces[i].I2];
        double Qy=cy[faces[i].I3]-cy[faces[i].I2];
        double Qz=cz[faces[i].I3]-cz[faces[i].I2];

        double Ci = Py*Qz - Pz*Qy;
        double Cj = Px*Qz - Pz*Qx;
        double Ck = Px*Qy - Py*Qx;

        double modulo = sqrt(Ci*Ci+Cj*Cj+Ck*Ck);

        Ni.push_back(Ci/modulo);
        Nj.push_back(Cj/modulo);
        Nk.push_back(Ck/modulo);


    }

    file.close();
    
    cout << "Vertices:" << n_vertices << endl << "Faces:" << n_faces << endl;
    cout << "Vertices:" << n_vertices << endl << "Faces:" << n_faces << endl;
    return 0;
}

void objeto(float x, float y, float z, float tam, int flag)
{
	int hsamples=30;
	int vsamples=30;
    GLfloat l=0.0;
    glMaterialfv(GL_FRONT, GL_AMBIENT, a); // impõe a cor a no material
    glMaterialfv(GL_FRONT, GL_DIFFUSE, b); // impõe a cor a no material
    //glColorMaterial(GL_FRONT, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL); /* WARNING: Ambient and diffuse material latch immediately to the current color. */
    //glLoadIdentity();
    GLfloat TexCoordX, TexCoordY;
	    glColor4f(1.0, 1.0, 1.0, 1.0);
        glBindTexture( GL_TEXTURE_2D, texture[flag] );
	
    for (int i=0; i<n_faces; i++)
    {
    		
		if (i%2==1){   /* Alterna os triangulos nos quadrilateros do objeto */
	    TexCoordX=GLfloat((i/2)%hsamples)/(float)hsamples;
		TexCoordY=(GLfloat)l;
		
        glBegin (GL_POLYGON);
        glNormal3f(Ni[i],Nj[i],Nk[i]);
        glTexCoord2f(TexCoordX, TexCoordY+1.0/(float)vsamples);
        glVertex3f(cx[faces[i].I1]*tam+x, cy[faces[i].I1]*tam+y, cz[faces[i].I1]*tam+z);
        glTexCoord2f(TexCoordX+1.0/(float)hsamples, TexCoordY+(1.0/(float)vsamples));
        glVertex3f(cx[faces[i].I2]*tam+x, cy[faces[i].I2]*tam+y, cz[faces[i].I2]*tam+z);
        glTexCoord2f(TexCoordX, TexCoordY);
        glVertex3f(cx[faces[i].I3]*tam+x, cy[faces[i].I3]*tam+y, cz[faces[i].I3]*tam+z);
        glEnd();
		}
		else{
	
		glBegin (GL_POLYGON);
        glNormal3f(Ni[i],Nj[i],Nk[i]);
        glTexCoord2f(TexCoordX+1.0/(float)hsamples, TexCoordY+1.0/(float)vsamples);
        glVertex3f(cx[faces[i].I1]*tam+x, cy[faces[i].I1]*tam+y, cz[faces[i].I1]*tam+z);
        glTexCoord2f(TexCoordX+1.0/(float)hsamples, TexCoordY);
        glVertex3f(cx[faces[i].I2]*tam+x, cy[faces[i].I2]*tam+y, cz[faces[i].I2]*tam+z);
        glTexCoord2f(TexCoordX, TexCoordY);
        glVertex3f(cx[faces[i].I3]*tam+x, cy[faces[i].I3]*tam+y, cz[faces[i].I3]*tam+z);
        glEnd();
		}
		if (i>0 && (i%(2*vsamples))==0)l+=1.0/(float)vsamples;
    }
}

void display(void)
{
    int i;

    GLfloat rot_x=0.0, rot_y=0.0;
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glLightfv(GL_LIGHT2, GL_SPOT_EXPONENT, light2_diffuse);
    glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light2_lookat);
    glLightfv(GL_LIGHT0, GL_SPECULAR,cor_spec);
    
    GLfloat global_ambient[] = { 0.4, 0.4, 0.4, 1 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

    glLoadIdentity();

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    glEnable(GL_LIGHT2);

    glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
    glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light2_spot);

    glEnable( GL_TEXTURE_2D );

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


    gluLookAt (3.0, 4.0, 30.0, 0.0, 0.0, 0.0, 0.5, 1.0, 0.0);
    
    

    glScalef (sc,sc,sc); // sc =1 
   
    alpha=(theta/180.0)*3.14159265;

    glPushMatrix();
    glTranslated(0,0,-3);
    glRotatef(theta,1.0,0.0,1.0);
    objeto( 0.0,  0.0,  16.0,  3.0,0);
    glPopMatrix();
	
	glPushMatrix();
    glTranslated(-3,-3,0);
    glRotatef(theta,0.0,1.0,1.0);
    objeto( 0.0,  8.0,  0.0,  4.0,1);
    glPopMatrix();
	
	glPushMatrix();
    glTranslated(3,3,0);
    glRotatef(theta,1.0,1.0,0.0);
    objeto( 0.0,  0.0,  8.0,  1.0,2);
    glPopMatrix();


    glFlush();
    glutSwapBuffers();
   
}


void myReshape(int w, int h)
{
    float aspectRatio = (float)w/(float)h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45,aspectRatio,-10,10);

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
    glCullFace(GL_FRONT);    //  
    glEnable(GL_CULL_FACE); //

    glDepthFunc(GL_LEQUAL);  ///*******
    glDepthRange(50,-50);    ///*******
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glDepthMask(GL_TRUE);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glColor3f(0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);  

}

void Mouse(int button, int state, int x, int y)
{
  
    if (button==3)
    {
        sc=sc+0.1;
    }
    if (button==4)
    {
        if (sc>0)sc=sc-0.1;
    }


    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            x_0 =  x/100.0;
            y_0 =  y/100.0;
  //          cam_theta0=cam_theta;
  //          cam_phi0=cam_phi;

        }
        if (state == GLUT_UP)
        {
    //        cam_theta0=cam_theta;
    //        cam_phi0=cam_phi;


        }
    }

/*
    if (button == GLUT_RIGHT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            x0 =  x;
            y0 =  y;
            deslocx0=deslocx;
            deslocy0=deslocy;

        }
        if (state == GLUT_UP)
        {
            deslocx0=deslocx;
            deslocy0=deslocy;


        }
    }
*/
    glutPostRedisplay();
}



int main(int argc, char **argv)
{
    carrega3d("las");

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (100, 100);
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);
    glEnable (GL_BLEND | GL_DEPTH_TEST);
    // glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    texture[0]=LoadTexture2("mars.ppm",2048,1024);
    texture[1]=LoadTexture2("map.ppm",1200,600);
    texture[2]=LoadTexture2("moon.ppm",2000,1000);
	texture[3]=LoadTexture2("mrpostman.ppm",16,16);
    
    glutFullScreen();
    
    myinit();

    glutDisplayFunc(display);

    glutReshapeFunc(myReshape);
    glutMouseFunc(Mouse);

    glutTimerFunc(10, timer, 0);
    glutMainLoop();
    return 0;


}
