#ifndef PPMSTRUCT_H       // Evita inclusões múltiplas
#define PPMSTRUCT_H
#define MAX_LINHAS 256
#define MAX_COLUNAS 256
#define MAX_TILES 20

// Protótipo da função (só declaração, sem corpo) também vai as structs
// convenção: x = índice de linha (0..altura-1), y = índice de coluna (0..largura-1)

typedef struct pixel{

    int red;
    int green;
    int blue;

} pixel;

typedef struct image{

    int largura;
    int altura;
    int** red;
    int** green;
    int** blue;

} image;

typedef struct tile {
    char simbolo;
    image* img;
} tile;


//codigo fornecido
image* inicializa(image* imagem, int largura, int altura);
void apaga(image* imagem);
image* mostra(image* imagem);

//codigo das tarefas
image* salvaImagem(image* imagem, const char* nomeDoArquivo);
image* salvaImagem_P6(image* imagem, const char* nomeDoArquivo);
image* carregaArquivo(const char* nomeDoArquivo);
void putPixel(image* figura, int coordenada_x, int coordenada_y, int r, int g, int b);
void set_pixel(image* imagem, int coluna,int linha,int r,int g,int b);
pixel get_pixel(image* figura, int coordenada_x, int coordenada_y);
int get_red(image* figura, int x, int y);
int get_green(image* figura, int x, int y);
int get_blue(image* figura, int x, int y);
void carimbaImagem(image* figura_destino, image* carimbo, int coordenada_x_figura_destino, int coordenada_y_figura_destino, int transparencia, int tr,int tg, int tb);
int carregaMapa(const char* nomeDoArquivo, char mapa[MAX_LINHAS][MAX_COLUNAS], int* linhas, int* colunas);
image* montaLabirinto(char mapa[MAX_LINHAS][MAX_COLUNAS], int linhas, int colunas, image* arbusto, image* chao);
int carregaDicionario(const char* nomeDoArquivo, tile dicionario[MAX_TILES]);

#endif