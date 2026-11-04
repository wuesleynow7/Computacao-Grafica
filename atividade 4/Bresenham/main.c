#include <stdlib.h>
#include "bresenham.h"


int main(){

    image* imagemBresenham = malloc(sizeof *imagemBresenham);
    imagemBresenham = inicializa(imagemBresenham,300,300);

    bresenhamLine(imagemBresenham,10,130,300,60);
    bresenhamLine(imagemBresenham,30,10,200,70);
    bresenhamLine(imagemBresenham,45,38,155,20);
    bresenhamLine(imagemBresenham,75,23,130,90);
    
    // https://www.youtube.com/watch?v=CceepU1vIKo&t=647s
    return 0;
}