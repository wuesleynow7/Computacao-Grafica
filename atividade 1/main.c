#include "ppmstruct.h"
#include <stdio.h>

int main(){


    char mapa[MAX_LINHAS][MAX_COLUNAS];
    int linhas, colunas;
    

    if (carregaMapa("maze.txt", mapa, &linhas, &colunas)==0) {
        printf("nao consegui carregar o mapa\n");
        return 1;
    }

    carregaMapa("maze.txt", mapa, &linhas, &colunas);

    // Ajustando um pouco o “por isso tenho que declarar”: você declara porque precisa de um lugar para guardar o que a função retorna. Se você só chamasse:
    // carregaArquivo("arbusto.ppm"); se fosse declarado assim ela carregaria a imagem... e perderia o endereço
    // a imagem seria criada na memória (pelo malloc lá dentro), mas você não teria 
    // como chegar até ela depois. É como alguém te passar o endereço de uma casa e 
    // você não anotar.

    // Dentro do carregaArquivo, o malloc cria a image de verdade, e a função devolve só o endereço dela. 
    // O arbusto é o papel onde você anota esse endereço. Por isso depois você usa arbusto->largura: a seta 
    // quer dizer “vá até o endereço e pegue o campo”.
    image* arbusto = carregaArquivo("arbusto.ppm");
    image* chao = carregaArquivo("chao.ppm");

    if (arbusto == NULL || chao == NULL) {
        printf("erro nos tiles\n");
        return 1; 
    } 

    image* destino = montaLabirinto(mapa, linhas, colunas, arbusto,chao);

    salvaImagem(destino, "mazemontado_2.ppm");
    apaga(arbusto);
    apaga(chao);
    apaga(destino);



    //############# aplicando o carimbo com transparencia a duas imagens ################
    // image* imagemDestino2 = carregaArquivo("xena_p3.ppm");
    // image* carimbo = carregaArquivo("shapes_p3.ppm");
    // carimbaImagem(imagemDestino2,carimbo, 60,45,1,255,255,255);
    // salvaImagem(imagemDestino2,"xena_carimbo_transparente.ppm");

    //############## testar salvar arquivo_p6 #########################
    // image* a = carregaArquivo("shapes_p3.ppm");
    // salvaImagem(a, "a_p3.ppm");                   // gravacao direta em P3
    // salvaImagem_P6(a, "teste_p6.ppm");            // grava em P6

    // image* b = carregaArquivo("teste_p6.ppm");    // le o P6 que acabou de gravar
    // salvaImagem(b, "b_p3.ppm");                   // regrava em P3
    // gcc -Wall -Wextra -g main.c ppmstruct.c -o programa && ./programa
    // comando Ver os primeiros bytes do arquivo P6 em hexadecimal
    // head -c 32 teste_p6.ppm | xxd


    //############## aplicando o carimbo sólido a duas imagens ########################
    // image* imagemDestino = carregaArquivo("xena_p3.ppm");
    // image* carimbo = carregaArquivo("gradiente.ppm");
    // carimbaImagem(imagemDestino,carimbo, 600,450,0,0,0,0);
    // salvaImagem(imagemDestino,"xena_carimbo_solido.ppm");



    // ############# testando o carimbo pequeno primeiro
    // image* imagemDestino = malloc(sizeof *imagemDestino);
    // imagemDestino = inicializa(imagemDestino, 130, 130);
    // image* testeCarimbo = carregaArquivo("carimbo_teste.ppm");
    // carimbaImagem(imagemDestino, testeCarimbo, 25, 10,0,0,0,0);
    // salvaImagem(imagemDestino, "carimbo_solido_pequeno.ppm");



    // ############# testando as funções
    // image* figura = malloc(sizeof *figura);
    // figura= inicializa(figura,640, 480);
    // image* figura2 = malloc(sizeof*figura2);
    // figura2 = inicializa(figura2, 10,10);

    // putPixel(figura2,0,0,5,5,60); //escrevendo na figura2 R=5, G=5, B=60, na posicao (0,0)
    // pixel p = get_pixel(figura2, 0, 0);// peguei o pixel de posicao (0,0)
    // printf("R=%d G=%d B=%d\n", p.red, p.green, p.blue); //tem que sair R=5 G=5 B=60, se sair o getpixel e putpixel estao certos


    //testes para salvar imagem
    //salvaImagem(figura, "olha3.ppm");  // imagem gerada quando modifiquei a altura [i] eixo x
    //salvaImagem(figura, "olha.ppm");   // imagem gerada quando modifiquei a largura [j] eixo y
    //salvaImagem(figura, "olha2.ppm");  // imagem gerada quando misturei a altura [i] e largura [j]

    // mostra(figura); //vai mostrar a matriz numérica no terminal
    // apaga(figura);

    return 0;

}