// gcc -o teapot teapot.c -lGL -lGLU -lglut -lm

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

typedef struct {
    float x, y, z;
} Vec3;

typedef struct {
    int v1, v2, v3;      
} Face;

typedef struct {
    Vec3 *vertices;
    Vec3 *normais;      
    Face *faces;
    int numVertices;
    int numFaces;
} Objeto3D;

Objeto3D objeto = {0};   // objeto global, carregado no main


static Vec3 normalFace(Vec3 a, Vec3 b, Vec3 c)
{
    Vec3 u = { b.x - a.x, b.y - a.y, b.z - a.z };
    Vec3 w = { c.x - a.x, c.y - a.y, c.z - a.z };
    Vec3 n = { u.y * w.z - u.z * w.y,
               u.z * w.x - u.x * w.z,
               u.x * w.y - u.y * w.x };
    float len = sqrt(n.x * n.x + n.y * n.y + n.z * n.z);
    if (len > 1e-12f) {
        n.x /= len; n.y /= len; n.z /= len;
    }
    return n;
}

/* ---------- carregamento do OBJ ---------- */

int loadOBJ(const char *arquivo_nome, Objeto3D *obj)
{
    FILE *arquivo = fopen(arquivo_nome, "r");
    if (!arquivo)
        return 0;

    char line[256];

    obj->numVertices = 0;
    obj->numFaces = 0;

    // 1a passada: contando v e f para poder alocar dinamicamente os vetores
    while (fgets(line, sizeof(line), arquivo)) {
        if (strncmp(line, "v ", 2) == 0)
            obj->numVertices++;
        else if (strncmp(line, "f ", 2) == 0)
            obj->numFaces++;
    }

    obj->vertices = malloc(obj->numVertices * sizeof(Vec3));
    obj->faces    = malloc(obj->numFaces * sizeof(Face));
    obj->normais  = malloc(obj->numFaces * sizeof(Vec3));

    // 2a passada: popula a struct
   fseek(arquivo, 0, SEEK_SET);  // volta para o inicio do arquivo
    int nv = 0, nf = 0;

    while (fgets(line, sizeof(line), arquivo)) {
        if (strncmp(line, "v ", 2) == 0) {
            Vec3 v;
            if (sscanf(line, " v %f %f %f", &v.x, &v.y, &v.z) == 3) {
                obj->vertices[nv] = v;
                nv++;
            }
        }
        else if (strncmp(line, "f ", 2) == 0) {
            Face f;
            if (sscanf(line, "f %d %d %d", &f.v1, &f.v2, &f.v3) == 3) {
                f.v1--; f.v2--; f.v3--;      
                obj->faces[nf] = f;
                // calcula a normal da face, popula a struct e ja normaliza o vetor normal
                obj->normais[nf] = normalFace(obj->vertices[f.v1],
                                              obj->vertices[f.v2],
                                              obj->vertices[f.v3]);
                nf++;
            }
        }
    }

    fclose(arquivo);
    obj->numVertices = nv;
    obj->numFaces = nf;
    return 1;
}

void liberaObjeto(void)
{
    free(objeto.vertices);
    free(objeto.normais);
    free(objeto.faces);
}

/* Centraliza o objeto na origem e o escala para que a maior dimensao
   da caixa envolvente seja 'tamanho'. Chamada fora do desenho. */
void centralizaEEscala(Objeto3D *obj, float tamanho)
{
    if (obj->numVertices == 0) return;

    Vec3 mn = obj->vertices[0], mx = obj->vertices[0];
    for (int i = 1; i < obj->numVertices; i++) {
        Vec3 v = obj->vertices[i];
        if (v.x < mn.x) mn.x = v.x;  if (v.x > mx.x) mx.x = v.x;
        if (v.y < mn.y) mn.y = v.y;  if (v.y > mx.y) mx.y = v.y;
        if (v.z < mn.z) mn.z = v.z;  if (v.z > mx.z) mx.z = v.z;
    }

    Vec3 c = { (mn.x + mx.x) / 2, (mn.y + mx.y) / 2, (mn.z + mx.z) / 2 };
    float ext = fmaxf(mx.x - mn.x, fmaxf(mx.y - mn.y, mx.z - mn.z));
    float s = (ext > 1e-12f) ? tamanho / ext : 1.0f;

    for (int i = 0; i < obj->numVertices; i++) {
        obj->vertices[i].x = (obj->vertices[i].x - c.x) * s;
        obj->vertices[i].y = (obj->vertices[i].y - c.y) * s;
        obj->vertices[i].z = (obj->vertices[i].z - c.z) * s;
    }
    // escala uniforme + translação não alteram as normais
}

/* ---------- configuração OpenGL ---------- */

int window_width  = 800;  // "static int" evitaria que outros códigos do mesmo projeto vejam estas variáveis
int window_height = 600;

GLfloat light0_position[] = { 50.0f, 50.0f, 50.0f, 1.0f }; // x,y,z,w = 1 , luz pontual
GLfloat light0_ambient[]  = { 0.15f, 0.15f, 0.15f, 1.0f };
GLfloat light0_diffuse[]  = { 0.80f, 0.80f, 0.80f, 1.0f };
GLfloat light0_specular[] = { 1.00f, 1.00f, 1.00f, 1.0f };

GLfloat global_ambient[]  = { 0.05f, 0.05f, 0.05f, 1.0f };

GLfloat mat_ambient[]   = { 0.1f, 0.1f, 0.2f, 1.0f };
GLfloat mat_diffuse[]   = { 0.2f, 0.3f, 0.8f, 1.0f };
GLfloat mat_specular[]  = { 0.9f, 0.9f, 0.9f, 1.0f };
GLfloat mat_shininess[] = { 64.0f };

GLfloat cam_eye[]    = { 0.0f, 1.5f, 4.0f };
GLfloat cam_center[] = { 0.0f, 0.5f, 0.0f };
GLfloat cam_up[]     = { 0.0f, -1.0f, 0.0f };

float angle = 0.0f; // "static float", a mesma coisa.

void meuinit(void)
{
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);

    // Iluminacao global
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // modelo de observador local para LUZ especular

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
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}

void reshape(int w, int h)
{
    window_width = w;
    window_height = (h == 0) ? 1 : h;
    glViewport(0, 0, window_width, window_height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLdouble)window_width / (GLdouble)window_height, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void mostraobjeto( Objeto3D *obj)
{
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < obj->numFaces; i++) {
        //Face *f = &(obj->faces[i]);
        glNormal3f(obj->normais[i].x, obj->normais[i].y, obj->normais[i].z);
        glVertex3f(obj->vertices[(obj->faces[i]).v1].x, obj->vertices[(obj->faces[i]).v1].y, obj->vertices[(obj->faces[i]).v1].z);
        glVertex3f(obj->vertices[(obj->faces[i]).v2].x, obj->vertices[(obj->faces[i]).v2].y, obj->vertices[(obj->faces[i]).v2].z);
        glVertex3f(obj->vertices[(obj->faces[i]).v3].x, obj->vertices[(obj->faces[i]).v3].y, obj->vertices[(obj->faces[i]).v3].z);
    }
    glEnd();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // gluLookAt configura a matriz de visualizacao
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cam_eye[0], cam_eye[1], cam_eye[2], cam_center[0], cam_center[1], cam_center[2],cam_up[0], cam_up[1], cam_up[2]);

    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

    glPushMatrix();
      glDisable(GL_LIGHTING); // chao simples
      glBegin(GL_LINES);
        for (int i = -10; i <= 10; i++) {
            glColor3f((GLfloat)(i + 10) / 20, (GLfloat)(i + 20) / 30, (GLfloat)(i + 40) / 50);
            glVertex3f((GLfloat)i, 0.0f, -10.0f);
            glVertex3f((GLfloat)i, 0.0f,  10.0f);
            glVertex3f(-10.0f, 0.0f, (GLfloat)i);
            glVertex3f( 10.0f, 0.0f, (GLfloat)i);
        }
      glEnd();
      glEnable(GL_LIGHTING);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(0.0f, 0.5f, 0.0f);
      glRotatef(angle, 0.0f, 1.0f, 0.0f);
      glColor3f(1.0f, 0.0f, 0.0f);   // cor/material definidos aqui fora
      mostraobjeto(&objeto);
    glPopMatrix();

    glutSwapBuffers();
}

void idleFunc(void)
{
    angle += 1.0f;
    if (angle > 360.0f) angle -= 360.0f;
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    (void)x; (void)y;
    if (key == 27 || key == 'q' || key == 'Q') { // ESC ou Q sai
        exit(0);
    }
}

int main(int argc, char **argv)
{
    if (argc <= 1){printf("Passe o nome do arquivo obj como argumento!\n");return -1;}

    if (!loadOBJ(argv[1], &objeto)) {
        printf("Erro ao carregar o arquivo OBJ.\n");
        return -1;
    }

    centralizaEEscala(&objeto, 1.0f);   // maior dimensão = 5.0, centrado na origem

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(window_width, window_height);
    glutCreateWindow("mostra objeto");

    meuinit();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idleFunc);

    glutMainLoop();
    return 0;
}