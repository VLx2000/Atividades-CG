// Um exemplo utizando as funções da biblioteca implementada

// Para executar:
// gcc -c bibaux.c
// gcc -c cg2d.c
// gcc exemplo1.c -o exemplo1 cg2d.o bibaux.o -lm -lX11
// ./exemplo1

/*
	Como comentário no código fonte, descreva qual fenômeno pode ser observado juntamente com a rotação do objeto. 
	Este efeito é desejável? Descreva uma forma de resolver o problema.
*/
/*	
	R: Ao realizarmos a rotação do objeto, nota-se que ocorre um efeito indesejável de deslocamento do objeto para outro
	local no espaço. Para resolvermos esse problema, determinamos o centróide do objeto usando a função Centroide() criada.
	Dessa forma, conseguimos deslocar o centro do objeto original para a origem e aplicar a transformação linear em cada ponto
	do objeto usando a função multiplicacao_matriz() a fim de obter a rotação de cada ponto e, consequentemente, a rotação do 
	objeto sem que ocorra um deslocamento indesejável. 

*/

#include "cg2d.h"
#include <math.h>
#define TAM 5
#define PI 3.14159265

/*
	Função auxiliar para a multiplicação de um ponto por uma matriz
	- Parâmetros:
		matriz[3][3]: Matriz de multiplicação
		*ponto: Um ponteiro para um tipo ponto que representa o ponto homogêneo que será usado para calcular sua nova coordenada
	
	- Valor de Retorno:
		new_point: novo ponto homogêneo transladado obtido após a multiplicação
 */
point *multiplicacao_matriz(float matriz[3][3], point *ponto)
{
	float result[3] = {0.0, 0.0, 0.0};
	float coord_ponto[] = {ponto->x, ponto->y, ponto->w};
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			result[i] += matriz[i][j] * coord_ponto[j];
		}
	}
	point *new_point = SetPoint(result[0], result[1], result[2], ponto->color);
	printf("%f,%f,%f\n", result[0], result[1], result[2]);
	return new_point;
}

/*
	Função auxiliar para o cálculo da área de um polígono, utilizada para o cálculo do centróide
	- Parâmetros:
		*poligono: Um ponteiro para um tipo object que representa o polígono que terá sua área calculada

	- Valor de Retorno:
		area: Um float que representa a área do polígono
 */
float Area(object *poligono)
{
	int num_points = poligono->numbers_of_points;
	float area = 0;
	int cord;
	for (int i = 0; i < num_points; i++)
	{
		if (i + 1 == num_points)
		{
			cord = 0;
		}
		else
		{
			cord = i + 1;
		}
		area = area + poligono->points[i].x * poligono->points[cord].y - poligono->points[cord].x * poligono->points[i].y;
	}
	area = area / 2;
	return area;
}


/*
	Função auxiliar para o cálculo das coordenadas do centróide de um polígono
	- Parâmetros:
		*poligono: Um ponteiro para um tipo object que representa o polígono que terá seu centróide calculado

	- Valor de Retorno:
		centroides: vetor de tipo float que contém as coordenadas x e y do centróide do polígono
 */
float *Centroide(object *poligono)
{
	float area = Area(poligono);
	float *centroides = malloc(sizeof(float) * 2);
	centroides[0] = 0.0;
	centroides[1] = 0.0;
	int cord;
	int num_points = poligono->numbers_of_points;
	for (int i = 0; i < num_points; i++)
	{
		if (i + 1 == num_points)
		{
			cord = 0;
		}
		else
		{
			cord = i + 1;
		}
		centroides[0] = centroides[0] + (poligono->points[i].x + poligono->points[cord].x) * (poligono->points[i].x * poligono->points[cord].y - poligono->points[cord].x * poligono->points[i].y);
		centroides[1] = centroides[1] + (poligono->points[i].y + poligono->points[cord].y) * (poligono->points[i].x * poligono->points[cord].y - poligono->points[cord].x * poligono->points[i].y);
	}
	centroides[0] = centroides[0] / (6 * area);
	centroides[1] = centroides[1] / (6 * area);
	return centroides;
}

/*
	Função que realiza a rotação do poligono dado um ângulo (em radianos) qualquer
	- Parâmetros:
		*poligono: Um ponteiro para um tipo object que representa o polígono que será rotacionado

	- Valor de Retorno:
		poligono_rotacionado: polígono rotacionado em um determinado ângulo
 */
object *Rotacao(object *poligono, float angulo)
{
	int num_points = poligono->numbers_of_points;

	object *poligono_rotacionado = CreateObject(num_points);
	float matriz_rotacao[3][3] = {{cos(angulo), -sin(angulo), 0},
								  {sin(angulo), cos(angulo), 0},
								  {0, 0, 1}};
	point *p;
	for (int i = 0; i < num_points; i++)
	{
		p = multiplicacao_matriz(matriz_rotacao, &poligono->points[i]);
		SetObject(p, poligono_rotacionado);
	}
	return poligono_rotacionado;
}

int main(int argc, char **argv)
{

	palette *palheta;
	bufferdevice *monitor;
	window *janela;
	viewport *porta1, *porta2;
	object *poligono1, *poligono2, *poligono3, *poligono1T, *origem;

	SetWorld(-128, 128, 127, 127);	  // Define o tamanho do mundo
	monitor = CreateBuffer(640, 480); // Cria um monitor virtual

	palheta = CreatePalette(5); // Cria um colormap (lookup table) com 5 cores
	SetColor(0, 0, 0, palheta);
	SetColor(1, 0, 0, palheta);
	SetColor(0, 1, 0, palheta);
	SetColor(0, 0, 1, palheta);
	SetColor(1, 1, 1, palheta);

	// cria dois objetos bidimensionais
	poligono1 = CreateObject(TAM);	// poligono original
	poligono2 = CreateObject(TAM);	// poligono transladado para a origem
	poligono3 = CreateObject(TAM);  // poligono rotacionado na origem
	poligono1T = CreateObject(TAM);	// poligono que ira aparecer rotacionado em cima do original (vermelho)
	origem = CreateObject(4); // cria "ponto da origem"

	// Insere as coordenadas dos pontos representados no SRU em cada objeto
	// O terceiro parâmetro será discutido em aula futura
	// O quarto parâmetro é o indice associado a lookup table (cor)
	SetObject(SetPoint(-9.0, -8.0, 1, 1), poligono1);
	SetObject(SetPoint(-7.0, -3.0, 1, 1), poligono1);
	SetObject(SetPoint(-4.0, -4.0, 1, 1), poligono1);
	SetObject(SetPoint(-3.0, -6.0, 1, 1), poligono1);
	SetObject(SetPoint(-6.0, -9.0, 1, 1), poligono1);
	SetObject(SetPoint(-0.1, -0.1, 1, 5), origem);
	SetObject(SetPoint(0.1, -0.1, 1, 5), origem);
	SetObject(SetPoint(0.1, 0.1, 1, 5), origem);
	SetObject(SetPoint(-0.1, 0.1, 1, 5), origem);

	float *centroides = malloc(sizeof(float) * 2);
	centroides = Centroide(poligono1);
	float dx = -centroides[0];
	float dy = -centroides[1];
	float matriz[3][3] = {{1, 0, dx},
						  {0, 1, dy},			// Matriz de Translação	em coordenadas homegêneas
						  {0, 0, 1}};

	float inversa[3][3] = {{1, 0, -dx},
						   {0, 1, -dy},			// Matriz Inversa usada na operação de translação inversa
						   {0, 0, 1}};
	float angulo;

	printf("Digite o angulo de rotacao do poligono (em graus): ");
	scanf("%f", &angulo);

	float angulo_rad = angulo * (PI / 180.0);	// Conversão do ângulo para radianos 

	for (int i = 0; i < poligono1->numbers_of_points; i++)
	{
		SetObject(multiplicacao_matriz(matriz, SetPoint(poligono1->points[i].x, poligono1->points[i].y, poligono1->points[i].w, 1)), poligono2);
	}

	poligono3 = Rotacao(poligono2, angulo_rad);

	for (int i = 0; i < poligono1->numbers_of_points; i++)
	{
		SetObject(multiplicacao_matriz(inversa, SetPoint(poligono3->points[i].x, poligono3->points[i].y, poligono3->points[i].w, 1)), poligono1T);
	}

	janela = CreateWindow(-12, -12, 6, 6); // cria uma janela de visualização (coordenadas no SRU)

	porta1 = CreateViewPort(0, 0, 639, 479); // Cria uma viewport
	porta2 = CreateViewPort(0, 0, 639, 479);
	// no caso uma única saída para o dispositivo de visualização com 640x480 entradas

	DrawObject(poligono1, janela, porta1, monitor, 1);	// poligono original
	DrawObject(poligono2, janela, porta2, monitor, 2);	// poligono transladado para a origem
	DrawObject(poligono3, janela, porta2, monitor, 3);	// poligono rotacionado na origem
	DrawObject(poligono1T, janela, porta2, monitor, 4); // poligono que era pra aparecer rotacionado em cima do original(vermelho)
	DrawObject(origem, janela, porta2, monitor, 4);

	Dump2X(monitor, palheta);
	free(poligono1);
	free(poligono2);
	free(poligono3);
	free(poligono1T);
	return 0;
}
