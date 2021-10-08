// Um exemplo utizando as funções da biblioteca implementada

// Para executar:
// gcc -c bibaux.c
// gcc -c cg2d.c
// gcc exemplo1.c -o exemplo1 cg2d.o bibaux.o -lm -lX11
// ./exemplo1

#include "cg2d.h"
#include <math.h>
#define TAM 5

point *multiplicao_matriz(float matriz[3][3], point *ponto)
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

object *Rotacao(object *poligono, int angulo)
{
	int num_points = poligono->numbers_of_points;

	object *poligono_rotacionado = CreateObject(num_points);
	float matriz_rotacao[3][3] = {{cos(angulo), -sin(angulo), 0},
								  {sin(angulo), cos(angulo), 0},
								  {0, 0, 1}};
	point *p;
	for (int i = 0; i < num_points; i++)
	{
		p = multiplicao_matriz(matriz_rotacao, &poligono->points[i]);
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
						  {0, 1, dy},
						  {0, 0, 1}};

	float inversa[3][3] = {{1, 0, -dx},
						   {0, 1, -dy},
						   {0, 0, 1}};

	for (int i = 0; i < poligono1->numbers_of_points; i++)
	{
		SetObject(multiplicao_matriz(matriz, SetPoint(poligono1->points[i].x, poligono1->points[i].y, poligono1->points[i].w, 1)), poligono2);
	}

	poligono3 = Rotacao(poligono2, 45);

	for (int i = 0; i < poligono1->numbers_of_points; i++)
	{
		SetObject(multiplicao_matriz(inversa, SetPoint(poligono3->points[i].x, poligono3->points[i].y, poligono3->points[i].w, 1)), poligono1T);
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
