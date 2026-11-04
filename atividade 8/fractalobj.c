#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

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

    FILE* f = fopen("terrain.obj", "w");
    if (!f) {
        perror("Erro ao criar arquivo OBJ");
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
    // Cada célula da grade gera dois triângulos
    for (int i = 0; i < size-1; i++) {
        for (int j = 0; j < size-1; j++) {
            int v1 = i*size + j + 1;
            int v2 = (i+1)*size + j + 1;
            int v3 = (i+1)*size + (j+1) + 1;
            int v4 = i*size + (j+1) + 1;

            // Triângulo 1: v1, v2, v3
            fprintf(f, "f %d %d %d\n", v1, v2, v3);
            // Triângulo 2: v1, v3, v4
            fprintf(f, "f %d %d %d\n", v1, v3, v4);
        }
    }

    fclose(f);

    for (int i = 0; i < size; i++) {
        free(altitude[i]);
    }
    free(altitude);

    printf("Arquivo terrain.obj gerado com sucesso!\n");
    return 0;
}