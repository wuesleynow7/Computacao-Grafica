#include<stdio.h>
#include <stdlib.h>
#include "filtros.h"


int main(){

    //essa é uma variavel que guarda endereco de memoria para o tipo image
    image* imagemMini = malloc(sizeof*imagemMini);

    //inicializa retorna o endereco de memoria 
    imagemMini = inicializa(imagemMini, 600,500);



    return 0;
}