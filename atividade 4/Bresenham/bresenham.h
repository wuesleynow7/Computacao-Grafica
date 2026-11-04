#ifndef BRESENHAM_H
#define BRESENHAM_H

typedef struct image{

    int largura;
    int altura;
    int** red;
    int** green;
    int** blue;

} image;

image* inicializa(image* imagem, int largura, int altura);
image* salvaImagem(image* imagem, const char* nomeDoArquivo);
void putPixel(image* figura, int coordenada_x, int coordenada_y, int r, int g, int b);
image* bresenhamLine(image* imagem, int x0, int y0, int x1, int y1);

#endif