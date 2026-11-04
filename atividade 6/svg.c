// Crie funções auxiliares (simplificadas) em C para carregar um arquivo SVG e ler linha a linha com fgets, "parseando" cada linha à medida em 
// que é carregada, avaliando se é line ou polygon. Se for line, invocar a função pré-existente drawline (image* img, int x1, int y1, int x2, int y2, int r, int g, int b) 
// para desenhar na imagem img (não é preciso implementar drawline, ela já existe). Se houver polygon points, parsear a sequência de valores e fazer um loop fechado conectando 
// os pontos (em um for chamando drawline). Se houver fill, chamar o comando fill para o ponto médio do polígono. Forneça alguns exemplos de figuras em svg compatíveis com o código, 
// para teste.O seu código deve integrar perfeitamente com drawline e floodfill dados em aula.

void imagem::preencherAreaIterativo(int xInicial, int
yInicial,pixel corOriginal, pixel corNova)
{
// Evita preenchimento desnecessário (mesma cor)
if (corOriginal.r == corNova.r &&
corOriginal.g == corNova.g &&
corOriginal.b == corNova.b)
return;
int x_aux;
// Pilha para armazenar coordenadas ("simula recursão")
std::vector<int> pilha;
pilha.push_back(xInicial);
pilha.push_back(yInicial);
while (!pilha.empty()) // enquanto a pilha tiver algo
{
// Recupera coordenadas do topo da pilha
yInicial = pilha.back(); pilha.pop_back();
xInicial = pilha.back(); pilha.pop_back();
// este par retirado da pilha só será pintado se não

foi pintado ainda, mas *será* retirado da pilha

// Move para a esquerda até encontrar borda ou cor

diferente

x_aux = xInicial;
while (x_aux >= 0 &&
img[x_aux][yInicial].r == corOriginal.r &&
img[x_aux][yInicial].g == corOriginal.g &&
img[x_aux][yInicial].b == corOriginal.b)
{
x_aux--; // retrocede até o limite "esquerdo"
}
x_aux++; // Volta para o primeiro pixel válido
// Preenche para a direita até encontrar borda ou cor

diferente pintando (se já não foi pintado)

while (x_aux < largura &&
img[x_aux][yInicial].r == corOriginal.r &&

img[x_aux][yInicial].g == corOriginal.g &&
img[x_aux][yInicial].b == corOriginal.b)
{
// Volta, alterando a cor do pixel
img[x_aux][yInicial] = corNova;
// Verifica pixel acima
if (yInicial > 0 &&
img[x_aux][yInicial - 1].r == corOriginal.r &&
img[x_aux][yInicial - 1].g == corOriginal.g &&
img[x_aux][yInicial - 1].b == corOriginal.b)
{ // se for cor pintável e pos dentro do limite
pilha.push_back(x_aux); // guarda as coord
pilha.push_back(yInicial - 1); // na pilha
}
// Verifica pixel abaixo
if (yInicial < altura - 1 &&
img[x_aux][yInicial + 1].r == corOriginal.r &&
img[x_aux][yInicial + 1].g == corOriginal.g &&
img[x_aux][yInicial + 1].b == corOriginal.b)
{
pilha.push_back(x_aux); // guarda as coord
pilha.push_back(yInicial + 1); // na pilha
}
x_aux++; // Avança para o próximo pixel à direita
} // fecha o while que altera a cor dos pixels
} // fecha o while que checa a pilha
} // fecha a função