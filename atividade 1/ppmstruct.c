#include <stdio.h>
#include <stdlib.h>
#include "ppmstruct.h"
#include <string.h>


//######## convenção [x=linha/altura][y=coluna/largura]

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

void apaga(image* imagem){//void tbm era o erro
/// Corrija a função “apaga”, que deveria desalocar a imagem da memória mas contém
// erros.
    int i;
    for (i=0; i<imagem->altura; i++) {
        free(imagem->red[i]);
        free(imagem->green[i]);
        free(imagem->blue[i]);
    }
    free(imagem->red);
    free(imagem->green);
    free(imagem->blue);
    free(imagem);// acho que era esse

}

image* mostra(image* imagem){

    int i,j;
    printf("P3\n%d %d\n255\n",imagem->largura, imagem->altura);
    for (i = 0; i < imagem->altura; i++){
        for (j = 0; j < imagem->largura; j++){
            printf("%d %d %d",imagem->red[i][j],imagem->green[i][j],imagem->blue[i][j]);
        }
    printf ("\n");
    }
    return imagem;
}

// crie respectivamente, salvar e ler uma imagem ppm do disco
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

image* salvaImagem_P6(image* imagem, const char* nomeDoArquivo){

    FILE* arquivo;
    arquivo = fopen(nomeDoArquivo,"wb"); // wb: escrita binaria

    if (arquivo == NULL) {
        return NULL;
    }

    int i,j;
    fprintf(arquivo, "P6\n%d %d\n255\n", imagem->largura, imagem->altura);

    for (i = 0; i < imagem->altura; i++){
        for (j = 0; j < imagem->largura; j++){
            fputc(imagem->red[i][j],   arquivo);   // 1 byte por canal, sem espacos
            fputc(imagem->green[i][j], arquivo);
            fputc(imagem->blue[i][j],  arquivo);;
        }
    }
    fclose(arquivo);
    return imagem;

/*
Os dois pontos vêm de uma diferença só: o P3 guarda os números como texto, e o P6 guarda os valores como bytes.

Por que fputc e não fprintf

Veja o que cada função grava para o valor 65:

fprintf(arquivo, "%d ", 65) grava os caracteres '6', '5' e ' '. São 3 bytes no arquivo, e quem lê precisa converter o texto de volta em número.
fputc(65, arquivo) grava um único byte de valor 65.

Como cada canal vai de 0 a 255, ele cabe exatamente em 1 byte, e é isso que o P6 pede. Um pixel laranja 255 128 0 fica assim:

Formato	O que vai para o arquivo	Bytes
P3	255 128 0 (texto)	11
P6	3 bytes: 255, 128, 0	3

Por isso o arquivo P6 é bem menor e mais rápido de ler.

Por que sem espaços entre os valores

Não dá para usar separador no P6. Um espaço é o byte de valor 32, e 32 também é uma cor válida. Se o arquivo tivesse espaços, o leitor não saberia se o byte 32 é separador ou dado. A regra do formato resolve isso: cada pixel ocupa exatamente 3 bytes seguidos, então o leitor só conta bytes.

Por que nessa ordem (red, green, blue, pixel a pixel, linha a linha)

Porque é o que a especificação do formato define, e qualquer visualizador lê assim. Seu carregaArquivo lê na mesma ordem, e é isso que as duas funções precisam ter em comum. Se você gravasse blue antes de red, a imagem abriria com as cores trocadas.

Por que "wb" e não "wt"

O b diz à biblioteca C: “não mexa nos bytes, grave exatamente o que eu mandei”. No modo texto, o Windows troca cada \n (byte 10) por dois bytes (\r\n). Num P3 isso é inofensivo, mas num P6 qualquer pixel com valor 10 viraria 2 bytes e desalinharia o arquivo inteiro dali em diante. No Linux o "wt" e o "wb" se comportam igual, mas usar "wb" deixa o código correto em qualquer sistema.
    
*/ 
}

image* carregaArquivo(const char* nomeDoArquivo){

    FILE* arquivo;
    char linha1[100];   //vetor da primeira linha
    int i,j, width,heigth, maxcolor;
    image* buffer;      //espaço de memoria que 

    //rb serve para P3 e P6
    arquivo = fopen(nomeDoArquivo,"rb"); //apenas abro o arquivo e depois começo as operações
    
    if (arquivo == NULL) {
    printf("erro ao abrir o arquivo\n");
    return NULL;
    }

    fgets(linha1,sizeof linha1, arquivo); // lê uma linha de texto de um fluxo de entrada
    fscanf(arquivo, "%i %i", &width, &heigth); //as dimensões do ppm
    fscanf(arquivo, "%i",&maxcolor);// cor do ppm

    //ANTES de alocar tenho que verificar se é dos formatos P3 e P6
    // formato invalido: sai antes de alocar qualquer coisa
    if (strncmp(linha1, "P3", 2) != 0 && strncmp(linha1, "P6", 2) != 0){
        printf("formato diferente\n");
        fclose(arquivo);
        return NULL;
    }


    buffer = malloc(sizeof *buffer);//aloca a imagem
    buffer = inicializa(buffer, width, heigth);


    if (strncmp(linha1,"P3",2) == 0){ //se o arquivo for p3, ja faço o que estava
    
    for (i = 0; i <buffer->altura; i++){
        for (j = 0; j < buffer->largura; j++){
           fscanf(arquivo,"%i",&buffer->red[i][j]);  //leio o pixel na posicao xy da matrix red do arquivo
           fscanf(arquivo,"%i",&buffer->green[i][j]);//leio o pixel na posicao xy da matrix green do arquivo
           fscanf(arquivo,"%i",&buffer->blue[i][j]); //leio o pixel na posicao xy da matrix blue do arquivo
        }
    }
    
    }
    else{   // so sobra o P6

        fgetc(arquivo);   // consome o unico espaco em branco depois do maxval

        for (i = 0; i < buffer->altura; i++){
            for (j = 0; j < buffer->largura; j++){
                buffer->red[i][j]   = fgetc(arquivo);
                buffer->green[i][j] = fgetc(arquivo);
                buffer->blue[i][j]  = fgetc(arquivo);
            }
        }
    }
    fclose(arquivo);
    return buffer;
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

void set_pixel(image* figura, int coordenada_x, int coordenada_y, int r, int g, int b){
    // função que
    /// irá definir a cor de um pixel dadas as suas coordenadas na imagem e as
    /// intensidades de suas componentes vermelha, verde e azul.
    if (coordenada_x>= 0 && coordenada_x < figura->altura && coordenada_y >=0 && coordenada_y < figura->largura)
    {
        figura->red[coordenada_x][coordenada_y]   = r;
        figura->green[coordenada_x][coordenada_y] = g;
        figura->blue[coordenada_x][coordenada_y]  = b;
    }
    else{
        printf("coordenada fora dos limites da imagem [undefined behavior / corrupção de memória]");
    }
}

pixel get_pixel(image* figura, int coordenada_x, int coordenada_y){
/*
função deve fazer: dado um ponteiro pra imagem e uma coordenada, ler os três valores daquela posição e 
devolver os três juntos, de alguma forma (por isso o tipo de retorno precisa carregar três valores — daí a struct pixel,
 que já existe pronta no seu .h com os campos red, green, blue).
 dicas:
 - nao devo percorrer a imagem
 - já tem x e y, não precisa iterar por nada, só indexar direto, igual você fez em putPixel.
*/
    pixel p;

    //checagem de limites
    if (coordenada_x >= 0 && coordenada_x < figura->altura && coordenada_y >= 0 && coordenada_y < figura->largura){

    p.blue = figura->blue[coordenada_x][coordenada_y];//aqui devemos atribuir a coordenada a cada pixel do RGB
    p.green = figura->green[coordenada_x][coordenada_y];
    p.red = figura->red[coordenada_x][coordenada_y];
    return p;

    }  
    else{
    p.blue = -1;
    p.green = -1;
    p.red = -1;
    return p;   // agora devolve a struct inteira, não o último int da atribuição
    }
    // do jeito que estava eu tava devolvendo o ultimo valor
    //return p.blue = -1, p.green = -1, p.red = -1;
}

// pega o pixel especifico de cada matriz
int get_red(image* figura, int x, int y){

    if (x >= 0 && x < figura->altura && y >= 0 && y < figura->largura){
        return figura->red[x][y];
    }
    else{
        return -1;
    }
}

int get_blue(image* figura, int x, int y){

    if (x >= 0 && x < figura->altura && y >= 0 && y < figura->largura){
        return figura->blue[x][y];
    }
    else{
        return -1;
    }
}

int get_green(image* figura, int x, int y){

    if (x >= 0 && x < figura->altura && y >= 0 && y < figura->largura){
        return figura->green[x][y];
    }
    else{
        return -1;
    }

}

void carimbaImagem(image* figura_destino, image* carimbo, int coordenada_x_figura_destino, int coordenada_y_figura_destino, int transparencia, int tr,int tg, int tb){
    
    int i, j;
    pixel LePixel;// 1. lê o pixel do carimbo na posição local (i,j)

    //primeiro a imagem carimbo tem de ser menor que a figura a ser carimbada, mas nao precisa de checagem de limites, pois o putpixel ja faz isso
    if (coordenada_x_figura_destino + carimbo->altura > figura_destino->altura || coordenada_y_figura_destino + carimbo->largura > figura_destino->largura) {
    printf("aviso: carimbo excede os limites do destino, parte sera cortada\n");
    }
    for (i = 0; i < carimbo->altura; i++){
        for (j = 0; j < carimbo->largura; j++){

            LePixel = get_pixel(carimbo, i, j);//aquiu eu só to pegando o pixel e atribuindo ao LePixel

            if (transparencia == 1 && LePixel.red == tr && LePixel.green == tg && LePixel.blue == tb){
                //salta esse pixel
            }else{
                putPixel(figura_destino, i + coordenada_x_figura_destino, j + coordenada_y_figura_destino, LePixel.red, LePixel.green,LePixel.blue);
            }
        }
    }     
    
}

int carregaMapa(const char* nomeDoArquivo, char mapa[MAX_LINHAS][MAX_COLUNAS], int* linhas, int* colunas){

    //variaveis que vamos precisar
    FILE* arquivo;
    char buffer[MAX_COLUNAS + 2];//é assim pq tenho que descartar o \n e \0
    int tamanho;// quantos caracteres a linha tem de verdade
    int k; //posicao dentro das linhas
    int l,c;//usados só para limpar a matriz

    arquivo = fopen(nomeDoArquivo,"r");

    if (arquivo == NULL) {
    printf("erro ao abrir o arquivo\n");
    return 0;
    }
    //int* linhas, int* colunas sao duas variaveis que armazenao enderecos de memoria na qual so podem armazenar valores inteiros

    //Vá até o endereço para o qual linhas aponta e coloque o valor zero na variável que está lá.
    *linhas = 0; // conteudo do endereco para a qual linha aponta
    *colunas = 0;// conteudo do endereco para a qual linha aponta


    for (l = 0;l < MAX_LINHAS; l++) {
        for (c = 0;c < MAX_COLUNAS; c++){
            mapa[l][c] = ' ';
        }
    }


    //fgets(onde_guardar, tamanho_maximo, de_qual_arquivo) lê uma linha do arquivo e precisa de um lugar para colocar o texto que leu. Esse lugar é o buffer:
    // um vetor de char temporário, uma “bandeja” onde a linha chega antes de você decidir o que fazer com ela.
     /*
     Por que não ler direto para mapa[linha]? Por dois motivos:
    1) A linha chega com um '\n' no fim e com um '\0' depois dele. Uma linha da matriz só tem MAX_COLUNAS posições,
    então esses dois caracteres a mais não cabem. É por isso que o buffer tem MAX_COLUNAS + 2.

    2) Você precisa medir a linha e tirar o '\n' antes de guardar. Isso é mais fácil de fazer na bandeja do que dentro da matriz.

    Fluxo de cada volta: a linha chega ao buffer, você mede, copia só os caracteres do labirinto para o mapa e o buffer é reaproveitado para a próxima linha.
    */
    while (*linhas < MAX_LINHAS && fgets(buffer, sizeof buffer, arquivo) != NULL) {
    
        tamanho = strcspn(buffer,"\r\n");

        for (k=0; k< tamanho && k < MAX_COLUNAS; k++) {
            mapa[*linhas][k] = buffer[k];
        }
        if (tamanho>*colunas) {
            *colunas = tamanho;
        }
            *linhas = *linhas + 1;

    }
    fclose(arquivo);
    return 1;
}

image* montaLabirinto(char mapa[MAX_LINHAS][MAX_COLUNAS], int linhas, int colunas, image* arbusto, image* chao){

    int alturaTile = arbusto->altura;
    int larguraTile = arbusto->largura;

    image* labirinto = malloc(sizeof(*labirinto));
    labirinto = inicializa(labirinto, colunas*larguraTile, linhas*alturaTile);

    for (int l=0; l<linhas; l++) {
        for (int c=0; c<colunas; c++) {
            int x = l*alturaTile;
            int y = c*larguraTile;

            if (mapa[l][c] == '#') {
                carimbaImagem(labirinto,arbusto, x, y,0, 0, 0, 0);
            }
            else if (mapa[l][c] == '.') {
                carimbaImagem(labirinto,chao, x, y,0, 0, 0, 0);
            }
        }
    }
   return labirinto;
}

int carregaDicionario(const char* nomeDoArquivo, tile dicionario[MAX_TILES]){

    FILE* arquivo;
    arquivo = fopen(nomeDoArquivo, "r");

    if (arquivo == NULL) {
    printf("erro ao abrir o arquivo\n");
    return 0;
    }

    int n = 0;

    return 1;
}