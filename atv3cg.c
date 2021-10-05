// Um exemplo utizando as funções da biblioteca implementada

// Para executar:
// gcc -c bibaux.c
// gcc -c cg2d.c
// gcc exemplo1.c -o exemplo1 cg2d.o bibaux.o -lm -lX11
// ./exemplo1

#include "cg2d.h"
#include <math.h>

int main(int argc, char ** argv) {
  
  palette * palheta;
  bufferdevice * monitor;
  window * janela;
  viewport *porta1 , *porta2;
  object * poligono1, * poligono2;
  
  SetWorld(-128, 128, 127, 127); // Define o tamanho do mundo  
  monitor = CreateBuffer(640,480); // Cria um monitor virtual
  
  palheta = CreatePalette(5);  // Cria um colormap (lookup table) com 5 cores
  SetColor(0,0,0,palheta);
  SetColor(1,0,0,palheta);
  SetColor(0,1,0,palheta);
  SetColor(0,0,1,palheta);
  SetColor(1,1,1,palheta);
  
  // cria dois objetos bidimensionais
  poligono1 = CreateObject(5); 
  poligono2 = CreateObject(5); // polígono fechado com 5 vértices
  
  // Insere as coordenadas dos pontos representados no SRU em cada objeto
  // O terceiro parâmetro será discutido em aula futura
  // O quarto parâmetro é o indice associado a lookup table (cor)
  SetObject(SetPoint(-15.0,-12.0,2,1), poligono1);
  SetObject(SetPoint(-10.0,-5.0,2,1), poligono1);
  SetObject(SetPoint(-9.0,-7.0,2,1), poligono1);
  SetObject(SetPoint(-7.0,-10.0,2,1), poligono1);
  SetObject(SetPoint(-9.0,-11.0,2,1), poligono1);
  
  for(int i = 0;i<5;i++){
    float x = poligono1->points[i].x;
    float y = poligono1->points[i].y;
    float new_x = x*sin(45) - y*sin(45);
    printf("%f\n",new_x);
    float new_y = y*cos(45) + x*sin(45);
    SetObject(SetPoint(new_x,new_y,2,2), poligono2);
  }

  
  
  janela = CreateWindow(-10.0,-10.0,0.0,0.0); // cria uma janela de visualização (coordenadas no SRU)
 
  porta1 = CreateViewPort(0, 0, 639, 479); // Cria uma viewport
  porta2 = CreateViewPort(0,0,639,479);
  // no caso uma única saída para o dispositivo de visualização com 640x480 entradas
  
  DrawObject(poligono1,janela,porta1,monitor,1);
  DrawObject(poligono2,janela,porta2,monitor,2);
 
  Dump2X(monitor,palheta);
 free(poligono1);
 free(poligono2);
  return 0;
  }


