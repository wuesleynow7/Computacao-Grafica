#include<stdio.h>
#include <stdlib.h>
#include "filtros.h"


image* inicializa(image* imagem,int largura, int altura){

    int i,j;
    imagem->altura=altura;
    imagem->largura=largura;

    imagem->red = (int **)malloc( altura * sizeof(int *));

    for (i=0; i<altura; i++) {
        imagem->red[i] = (int *)malloc(largura * sizeof(int));
    }
    imagem->green = (int **)malloc(altura * sizeof(int *));

    for (i=0; i<altura; i++) {
        imagem->green[i] = (int *)malloc(largura * sizeof(int));
    }
    imagem->blue = (int **)malloc(altura * sizeof(int *));
    for (i=0; i<altura; i++) {
        imagem->blue[i] = (int *)malloc(largura * sizeof(int));
    }
    for (i = 0; i < imagem->altura; i++)
        for (j = 0; j < imagem->largura; j++){

        // só as 3 cores gera uma imagem sólida
        imagem->red[i][j] = 0;
        imagem->green[i][j] = 0;
        imagem->blue[i][j] = 0;

        // para fazer o arco iris ou cores separadas tem que mudar de altura ou largura, se nao todas as cores serao sobrepostas
        // if(i< 20){
        //     imagem->red[i][j] = 2;
        //     imagem->green[i][j] = 127;
        //     imagem->blue[i][j] = 99;
        // }
        // else if(i < 40){
        //     imagem->red[i][j] = 255;
        //     imagem->green[i][j] = 0;
        //     imagem->blue[i][j] = 200;
        // }
        //         else if(i < 300){
        //     imagem->red[i][j] = 255;
        //     imagem->green[i][j] = 255;
        //     imagem->blue[i][j] = 200;
        // }
        //         else if(i < 400){
        //     imagem->red[i][j] = 255;
        //     imagem->green[i][j] = 0;
        //     imagem->blue[i][j] = 255;
        // }
    }
return imagem;
}

image* salvaImagem(image* imagem, const char* nomeDoArquivo){

    FILE* arquivo;
    arquivo = fopen(nomeDoArquivo,"wt");

    if (arquivo == NULL) {
        return NULL;
    }

    int i,j;
    fprintf(arquivo, "P3\n%d %d\n255\n", imagem->largura, imagem->altura);

    for (i = 0; i < imagem->altura; i++){
        for (j = 0; j < imagem->largura; j++){
            fprintf(arquivo, "%d %d %d ", imagem->red[i][j], imagem->green[i][j], imagem->blue[i][j]);
    }
    fprintf(arquivo, "\n");
    }
    fclose(arquivo);
    return imagem;
}

void putPixel(image* figura, int coordenada_x, int coordenada_y, int r, int g, int b){
/*O que ela deve fazer: dado um ponteiro pra imagem, uma coordenada (x,y) e três valores de cor (r,g,b),
 escrever essa cor na posição indicada, sem sair dos limites da matriz alocada.*/

    if (coordenada_x >= 0 && coordenada_x < figura->altura && coordenada_y >= 0 && coordenada_y < figura->largura){
        //defini a cor que vai em cada matriz e a posicao 
        figura->red[coordenada_x][coordenada_y]   = r;
        figura->green[coordenada_x][coordenada_y] = g;
        figura->blue[coordenada_x][coordenada_y]  = b;
    }   
    else{
        // fora dos limites: pixel ignorado silenciosamente "coordenada fora dos limites da imagem [undefined behavior / corrupção de memória]"
    }
}


image* filtro_miniatura(image *in){

    int altura, largura;

    //




}


image* edges_sobel(image *in);       //magnitude Sobel 
image* edges_sobel_h(image *in);      // Sobel horizontal 
image* edges_sobel_v(image *in);      // Sobel vertical 
image* edges_laplacian(image *in);    // Laplaciano 3x3 
image* motion_from_edges(image *A,image *B); // bônus