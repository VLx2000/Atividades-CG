//gcc exemplo3.c -o exemplo3 cg2d.o bibaux.o -lm -lX11
// sudo ./exemplo3
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "cg2d.h"

/*************************************************************************************
As três próximas estruturas estão sendo deixadas aqui, mas seria mais interessante deixá-as no cg2d.h

O mesmo vale para as implementações as funções que estão ao final deste aquivo, o melhor lugar é em cgd2.c
**************************************************************************************/

typedef struct ObjectList {
  	int numbers_of_objects;	
	object * obj;
  	} objectList;
  	
typedef struct just_a_point {
	int x, y;
	} points;
	
typedef struct FromMouse {
	points * p;
  	int n;		
  	} fromMouse;

fromMouse * GetMousePoints(int,int,int);
float calib_x(float);
float calib_y(float);
object * CreateObject(fromMouse *);
objectList * CreateListObject(int);
int SetObjectPointsFromMouse(fromMouse *, object *);
int SetListObject(object *, objectList *);


int main(int argc, char ** argv) {
  
  palette * palheta;
  bufferdevice * monitor;
  window * janela;
  viewport * porta;
  fromMouse * SetOfPoints;
  object * poligono;
  objectList * polig_list;
  
  float fator_calib_x, fator_calib_y;
  
  printf("Algumas calibrações para o dispositivo virtual de captura...\n\n");
  printf("Calibrando a unidade de medida na horizontal. Aperte o botão do centro para começar.\n");
  fator_calib_x = calib_x(4.0); // os parâmetros de entrada aqui dependem do sru usado, cuidado!
  printf("Calibrando a unidade de medida na vertical. Aperte o botão do centro para começar.\n");
  fator_calib_y = calib_y(3.0);
  
  printf("Fatores de calibração: (%f,%f)\n",fator_calib_x,fator_calib_y);
 
  polig_list = CreateListObject(4);
  
  // o uso de um for aqui fica mais elegante...
  SetOfPoints = GetMousePoints(1,0,0);
  poligono = CreateObject(SetOfPoints);
  SetObjectPointsFromMouse(SetOfPoints,poligono);
  SetListObject(poligono,polig_list);
  
  SetOfPoints = GetMousePoints(2,fator_calib_x*6.8,fator_calib_y*0.6);
  poligono = CreateObject(SetOfPoints);
  SetObjectPointsFromMouse(SetOfPoints,poligono);
  SetListObject(poligono,polig_list);
  
  SetOfPoints = GetMousePoints(3,fator_calib_x*(-6.8),fator_calib_y*0.7);
  poligono = CreateObject(SetOfPoints);
  SetObjectPointsFromMouse(SetOfPoints,poligono);
  SetListObject(poligono,polig_list);
  
  SetOfPoints = GetMousePoints(4,fator_calib_x*0.0,fator_calib_y*4.5);
  poligono = CreateObject(SetOfPoints);
  SetObjectPointsFromMouse(SetOfPoints,poligono);
  SetListObject(poligono,polig_list);

  janela = CreateWindow(-10.0*fator_calib_x,-6.0*fator_calib_y,10.0*fator_calib_x,7.0*fator_calib_y);
  
  porta = CreateViewPort(0, 0, 639, 479);
  monitor = CreateBuffer(640,480);
  
  // que tal um for aqui também?
  DrawObject(&(polig_list->obj[0]),janela,porta,monitor,1);
  DrawObject(&(polig_list->obj[1]),janela,porta,monitor,1);
  DrawObject(&(polig_list->obj[2]),janela,porta,monitor,1);
  DrawObject(&(polig_list->obj[3]),janela,porta,monitor,1);

  RasterFill(janela,monitor,1);

  palheta = CreatePalette(5);
  SetColor(0,0,0,palheta);
  SetColor(1,0,0,palheta);
  SetColor(0,1,0,palheta);
  SetColor(0,0,1,palheta);
  SetColor(1,1,1,palheta);
  Dump2X(monitor,palheta);
 
  return 0;
  }
 
 // Colocar no arquivo cg2d.c
 
 struct FromMouse * GetMousePoints(int set, int i, int j) {

	int fd, bytes, esquerdo, direito, centro, n, scan, start;
	signed char x, y;
	unsigned char data[3];
	const char * mouse_device = "/dev/input/mice";
	struct FromMouse * pontos;
	int max_number_of_points = 10000;

	pontos = (struct FromMouse *) malloc(sizeof(struct FromMouse));
	pontos->p = (points *) malloc(max_number_of_points*sizeof(points));
	
	printf("\nLendo o %dº conjunto de pontos a partir do mouse...\n",set);  
	printf("Aperte o botão esquerdo para cada ponto desejado.\n");
	printf("Finalize com o botão direito.\n"); 

	// Abre o mouse
	if ((fd = open(mouse_device, O_RDWR)) == -1) {
		printf("Houve um erro abrindo %s... você executou como superusuário?\n", mouse_device);
		return pontos;
		}

	// Fica lendo o mouse
	start = 0;
	scan = 1;
	n = 0;
	while (scan) {
		bytes = read(fd, data, sizeof(data));
		if (bytes > 0) {
			// informações sobre os botões do mouse
			esquerdo = data[0] & 0x1;
			direito = data[0] & 0x2;
			centro = data[0] & 0x4;
			// posição relativa do ponteiro
			x = data[1];
			y = data[2];
			if (centro) start = !start;	      
			if (start) {
				i = i + x; 
				j = j + y; 	
				if (esquerdo) {				
					pontos->p[n].x = i; 
					pontos->p[n].y = j;
					n++;
					}
				if (direito) {
					pontos->p[n].x = i;
					pontos->p[n].y = j;
					pontos->n = n;
					scan = !scan;
					}
				}
			}   
		}
		return pontos;		
	}

object * CreateObject(fromMouse * SetOfPoints) {
  object * ob;
  int numbers_of_points;
 
  numbers_of_points = SetOfPoints->n;
  ob = (object *) malloc(sizeof(object));
  ob->numbers_of_points = 0;
  ob->points = (point *) malloc(numbers_of_points*sizeof(point));
 
  return ob;
  }
  
objectList * CreateListObject(int numbers_of_objects) {
  objectList * polig_list;
  
  polig_list = (objectList *) malloc(sizeof(objectList));
  polig_list->numbers_of_objects = 0;
  polig_list->obj = (object *) malloc(numbers_of_objects*sizeof(object));
  polig_list->obj->points = (point *) malloc(10000*sizeof(point));
  
  return polig_list;
  }

int SetObjectPointsFromMouse(fromMouse * SetOfPoints, object * poligono) {
  int k;
  
  for(k=0;k<(SetOfPoints->n);k++) {
  	//printf("Ponto %d: (%d,%d)\n",k,SetOfPoints->p[k].x,SetOfPoints->p[k].y);
  	SetObject(SetPoint(SetOfPoints->p[k].x,SetOfPoints->p[k].y,1,1),poligono);
    	}
    
  return 0;
  }
  
int SetListObject(object * poligono, objectList * lista) {
  lista->obj[lista->numbers_of_objects] = * poligono;
  lista->numbers_of_objects = lista->numbers_of_objects + 1;
  
  return 0;
  }

float calib_x(float med) {

	int fd, bytes, centro, esquerdo, direito, scan, start; 
	int i, j, x_inicio, x_fim, y_inicio, y_fim;
	signed char x, y;
	unsigned char data[3];
	const char * mouse_device = "/dev/input/mice";
	float calib = 0.0;

	if ((fd = open(mouse_device, O_RDWR)) == -1) {
		printf("Houve um erro abrindo %s... você executou como superusuário?\n", mouse_device);
		return calib;
		}

	scan = 1;
	start = 0;
	i = 0;
	j = 0;
	while (scan) {
		bytes = read(fd, data, sizeof(data));
		if (bytes > 0) {
			esquerdo = data[0] & 0x1;
			direito = data[0] & 0x2;
			centro = data[0] & 0x4;
			x = data[1];
			y = data[2];
			if (centro) start = !start;
			if (start) {
				i = i + x; 
				j = j + y;
				if (esquerdo) {
					x_inicio = i;
					y_inicio = j; // desnecessário... estamos calibrando x apenas...
					}
				if (direito) {
					x_fim = i;
					y_fim = j;
					scan = !scan;
					}					
				}
			}   
		}
		
		calib = (float) abs((x_fim - x_inicio) / med);
		return calib;		
	}

 
float calib_y(float med) {

	int fd, bytes, centro, esquerdo, direito, scan, start; 
	int i, j, x_inicio, x_fim, y_inicio, y_fim;
	signed char x, y;
	unsigned char data[3];
	const char * mouse_device = "/dev/input/mice";
	float calib = 0.0;

	if ((fd = open(mouse_device, O_RDWR)) == -1) {
		printf("Houve um erro abrindo %s... você executou como superusuário?\n", mouse_device);
		return calib;
		}

	scan = 1;
	start = 0;
	i = 0;
	j = 0;
	while (scan) {
		bytes = read(fd, data, sizeof(data));
		if (bytes > 0) {
			esquerdo = data[0] & 0x1;
			direito = data[0] & 0x2;
			centro = data[0] & 0x4;
			x = data[1];
			y = data[2];
			if (centro) start = !start;
			if (start) {
				i = i + x; 
				j = j + y;
				if (esquerdo) {
					x_inicio = i;  // desnecessário... estamos calibrando y apenas... precisa melhorar isso aqui.
					y_inicio = j;
					}
				if (direito) {
					x_fim = i;
					y_fim = j;
					scan = !scan;
					}					
				}
			}   
		}
		
		calib = (float) abs((y_fim - y_inicio) / med);
		return calib;		
	}


