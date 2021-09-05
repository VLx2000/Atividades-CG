/*Victor Luís Aguilar Antunes 769734*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "cg2d.h"

#define TAM 5000

int main(void)
{
	int fd, bytes, esquerdo, direito, centro, registrar = 0, sair = 0;
	signed char x = 0, y = 0;
	int posx = 0, posy = 0;
	unsigned char data[3];
	const char *mouse_device = "/dev/input/mice";
	palette *palheta;
	bufferdevice *monitor;
	window *janela;
	viewport *porta;
	object *curva;

	SetWorld(-128, -128, 127, 127);	  // Define o tamanho do mundo
	monitor = CreateBuffer(800, 600); // Cria um monitor virtual

	palheta = CreatePalette(5); // Cria um colormap (lookup table) com 5 cores
	SetColor(0, 0, 0, palheta);
	SetColor(1, 0, 0, palheta);
	SetColor(0, 1, 0, palheta);
	SetColor(0, 0, 1, palheta);
	SetColor(1, 1, 1, palheta);

	// cria objeto bidimensional
	curva = CreateObject(TAM);

	// Abre o mouse
	if ((fd = open(mouse_device, O_RDWR)) == -1)
	{
		printf("Houve um erro abrindo %s... você executou como superusuário?\n", mouse_device);
		return -1;
	}

	// Fica lendo o mouse
	while (!sair)
	{
		bytes = read(fd, data, sizeof(data));
		if (bytes > 0)
		{
			if (registrar == 0)
				printf("Clique com o botão ESQUERDO para COMEÇAR\n");
			// informações sobre os botões do mouse
			esquerdo = data[0] & 0x1;
			direito = data[0] & 0x2;
			centro = data[0] & 0x4;

			if (esquerdo == 1) // ao clicar com o botao esquerdo
				if (registrar == 1)
				{ // se for o segundo clique ira sair do while
					sair = 1;
					registrar = 0;
				}
				else
					registrar = 1; // senao começa a registrar os inputs

			if (registrar == 1)
			{
				// posição relativa do ponteiro
				x = data[1];
				y = data[2];
				posx += (int)x;
				posy += (int)y;
				// exibe as informações
				printf("Registrando... Clique com o botão ESQUERDO para PARAR\t");
				printf("x = %d, y = %d\n", posx, posy);
				// Insere as coordenadas dos pontos representados no SRU no objeto
				// O terceiro parâmetro será discutido em aula futura
				// O quarto parâmetro é o indice associado a lookup table (cor)
				SetObject(SetPoint(posx, posy, 1, 1), curva);
			}
		}
	}
	janela = CreateWindow(-1000.0, -1000.0, 1000.0, 1000.0); // cria uma janela de visualização (coordenadas no SRU)
	porta = CreateViewPort(0, 0, 799, 599); // Cria uma viewport
	// no caso uma única saída para o dispositivo de visualização com 640x480 entradas

	DrawObject(curva, janela, porta, monitor, 1);
	Dump2X(monitor, palheta);
	return 0;
}