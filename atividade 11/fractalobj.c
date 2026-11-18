#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


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

//----------------- como vizualizar o objeto -----------------
/*
usando o codigo do teapot
use a funcao loadOBJ("terreno.obj", &objeto) para carregar o objeto




// compile: gcc -o fractalobj fractalobj.c -lm
// execute: ./fractalobj 
*/
              






double** fracland(int n, double escala, double decaimento) {
    int tam = (int)pow(2, n) + 1;
    double** grid = (double**)malloc(tam * sizeof(double*));
    for (int i = 0; i < tam; i++) {
        grid[i] = (double*)calloc(tam, sizeof(double));
    }

    grid[0][0] = (double)rand() / RAND_MAX;
    grid[0][tam-1] = (double)rand() / RAND_MAX;
    grid[tam-1][0] = (double)rand() / RAND_MAX;
    grid[tam-1][tam-1] = (double)rand() / RAND_MAX;

    int passo = tam - 1;
    double rng = escala;

    while (passo > 1) {
        int metade = passo / 2;

        // Diamante
        for (int x = metade; x < tam-1; x += passo) {
            for (int y = metade; y < tam-1; y += passo) {
                double avg = (grid[x-metade][y-metade] + grid[x-metade][y+metade] +
                              grid[x+metade][y-metade] + grid[x+metade][y+metade]) / 4.0;
                grid[x][y] = avg + ((double)rand() / RAND_MAX) * 2.0 * rng - rng;
            }
        }

        // Quadrado
        for (int x = 0; x < tam; x += metade) {
            for (int y = ((x + metade) % passo); y < tam; y += passo) {
                double sum = 0.0;
                int count = 0;

                if (x - metade >= 0) { sum += grid[x-metade][y]; count++; }
                if (x + metade < tam) { sum += grid[x+metade][y]; count++; }
                if (y - metade >= 0) { sum += grid[x][y-metade]; count++; }
                if (y + metade < tam) { sum += grid[x][y+metade]; count++; }

                double avg = sum / (double)count;
                grid[x][y] = avg + ((double)rand() / RAND_MAX) * 2.0 * rng - rng;
            }
        }

        passo /= 2;
        rng *= decaimento;
    }

    return grid;
}

int main() {
    srand(time(NULL));
    int n = 6;
    double escala = 1.0;
    double decay = 0.5;

    int size = (int)pow(2, n) + 1;
    double** altitude = fracland(n, escala, decay);

    FILE* f = fopen("terreno.obj", "w");
    if (f==NULL) {
        printf("Erro ao criar arquivo OBJ");
        return 1;
    }

    // --- Vértices ---
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            double x = (double)(i - size/2) / (double)(size/2);
            double y = (double)(j - size/2) / (double)(size/2);
            double z = altitude[i][j];
            fprintf(f, "v %f %f %f\n", x, y, z);
        }
    }

    // --- Faces ---
    // Cada celula da grade gera dois triangulos
    for (int i = 0; i < size-1; i++) {
        for (int j = 0; j < size-1; j++) {
            int v1 = i*size + j + 1;
            int v2 = (i+1)*size + j + 1;
            int v3 = (i+1)*size + (j+1) + 1;
            int v4 = i*size + (j+1) + 1;

            // Triangulo 1: v1, v2, v3
            fprintf(f, "f %d %d %d\n", v1, v2, v3);
            // Triangulo 2: v1, v3, v4
            fprintf(f, "f %d %d %d\n", v1, v3, v4);
        }
    }

    fclose(f);

    for (int i = 0; i < size; i++) {
        free(altitude[i]);
    }
    free(altitude);

    printf("Arquivo terreno.obj gerado com sucesso!\n");
    return 0;
}
