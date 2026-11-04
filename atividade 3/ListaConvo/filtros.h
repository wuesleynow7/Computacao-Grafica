#ifndef FILTROS_H       // Evita inclusões múltiplas
#define FILTROS_H

//assinaturas de ppm
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

//assinatudas dos filtros
image* filtro_miniatura(image *in);
image* edges_sobel(image *in);       //magnitude Sobel 
image* edges_sobel_h(image *in);      // Sobel horizontal 
image* edges_sobel_v(image *in);      // Sobel vertical 
image* edges_laplacian(image *in);    // Laplaciano 3x3 
image* motion_from_edges(image *A,image *B); // bônus

#endif