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
  //poligono2 = CreateObject(3); // polígono fechado com 5 vértices
  
  // Insere as coordenadas dos pontos representados no SRU em cada objeto
  // O terceiro parâmetro será discutido em aula futura
  // O quarto parâmetro é o indice associado a lookup table (cor)
  //SetObject(SetPoint(-6.0,-5.34,1,1), poligono1);
  //SetObject(SetPoint(-4.67,-2.0,1,1), poligono1);
  //SetObject(SetPoint(-2.67,-2.67,1,1), poligono1);
  SetObject(SetPoint(-4.5,-4.0,1,1), poligono1);
  SetObject(SetPoint(-3.5,-1.5,1,1), poligono1);
  SetObject(SetPoint(-2.0,-2.0,1,1), poligono1);
  SetObject(SetPoint(-1.5,-3.0,1,1), poligono1);
  SetObject(SetPoint(-3.0,-4.5,1,1), poligono1);


point * multiplicao_matriz(float matriz[3][3], point ponto){
  float result[3] = {0.0,0.0,0.0};
  float coord_ponto[] = {ponto.x,ponto.y,ponto.w};
  for(int i=0;i<3;i++){
    for(int j = 0;j<3;j++){
      result[i] += matriz[i][j]*coord_ponto[j];
    }
  }
  point *new_point  = SetPoint(result[0],result[1],result[2],ponto.color);
  return new_point;
}


object * Rotacao(object * poligono, int angulo){
  int num_points = poligono->numbers_of_points;
  object *poligono_rotacionado = CreateObject(num_points);
  float matriz_rotacao[3][3] ={{cos(angulo),-sin(angulo),0},
                             {sin(angulo),cos(angulo),0},
                              {0,0,1}};
  point *p;
  for(int i=0;i<num_points;i++){
    p = multiplicao_matriz(matriz_rotacao,poligono->points[i]);
    SetObject(p,poligono_rotacionado);
  }
  return poligono_rotacionado; 
}

float Area(object * poligono){
  int num_points = poligono->numbers_of_points;
  float area = 0;
  int cord;
  for(int i = 0;i<num_points;i++){
    if(i+1 == num_points){
      cord=0;
    }else{
      cord=i+1;
    }
    area = area + poligono->points[i].x*poligono->points[cord].y - poligono->points[cord].x*poligono->points[i].y;
  }
  area = area/2;
  return area;
}

float * Centroide(object * poligono){
  float area = Area(poligono);
  float * centroides = malloc(sizeof(float)*2);
  centroides[0] = 0.0;
  centroides[1] = 0.0;
  int cord;
  int num_points = poligono->numbers_of_points;
  for(int i=0; i<num_points; i++){
    if(i+1 == num_points){
      cord=0;
    }else{
      cord=i+1;
    }
    centroides[0] = centroides[0] + (poligono->points[i].x + poligono->points[cord].x)*(poligono->points[i].x*poligono->points[cord].y 
                                                                                        - poligono->points[cord].x*poligono->points[i].y);
    centroides[1] = centroides[1] + (poligono->points[i].y + poligono->points[cord].y)*(poligono->points[i].x*poligono->points[cord].y 
                                                                                        - poligono->points[cord].x*poligono->points[i].y);

  }
  centroides[0] = centroides[0]/(6*area);
  centroides[1] = centroides[1]/(6*area);
  return centroides;
}


object * Desolocamento(object * poligono){
  int num_points = poligono->numbers_of_points;
  object *poligono_deslocado = CreateObject(num_points);
  float inversa[3][3] = {{1, 0, -3},
						   {0, 1, -1},
						   {0, 0, 1}};
  point *p;
  for(int i=0;i<num_points;i++){
    p = multiplicao_matriz(inversa,poligono->points[i]);
    SetObject(p,poligono_deslocado);
  }
  return poligono_deslocado; 
}


float * teste = Centroide(poligono1);
printf("%f %f\n", teste[0],teste[1]);

//for(int i = 0;i<3;i++){
    //float x = poligono1->points[i].x;
    //float y = poligono1->points[i].y;
    //float new_x = x*sin(45) - y*sin(45);
    //printf("%f\n",new_x);
    //float new_y = y*cos(45) + x*sin(45);
    //SetObject(SetPoint(new_x,new_y,1,2), poligono2);
  //}

  
  poligono2 = Rotacao(poligono1,45);
  poligono2 = Desolocamento(poligono2);
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


