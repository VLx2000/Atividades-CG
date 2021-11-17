#include "cg2d.h"

point * SetPoint(float x, float y, float w, int color) {
  point * pnt;
  
  pnt = (point *) malloc(sizeof(point)); 
  pnt->x = x/w;
  pnt->y = y/w;
  pnt->w = w;
  pnt->color = color;
  
  return pnt;
  }

object * CreateObject(int numbers_of_points) {
  object * ob;
 
  ob = (object *) malloc(sizeof(object));
  ob->numbers_of_points = 0;
  ob->points = (point *) malloc(numbers_of_points*sizeof(point));
 
  return ob;
  }

int SetObject(point * pnt, object * ob) {
  ob->points[ob->numbers_of_points] = *pnt;
  ob->numbers_of_points = ob->numbers_of_points + 1;

  return 0;
  }

palette * CreatePalette(int numbers_of_colors) {
  palette * pal;

  pal = (palette *) malloc(sizeof(palette));
  pal->numbers_of_colors = 0;
  pal->colors = (ColorValues *) malloc(numbers_of_colors*sizeof(ColorValues));
  
  return pal;
  }

int SetColor(float red, float green, float blue, palette * pal) {
  pal->colors[pal->numbers_of_colors].red = red;
  pal->colors[pal->numbers_of_colors].green = green;
  pal->colors[pal->numbers_of_colors].blue = blue;
  pal->numbers_of_colors = pal->numbers_of_colors + 1;

  return 0;
  }
  
ColorValues * GetColor(int value, palette * pal) {
 ColorValues * color;
 
 color = (ColorValues *) malloc(sizeof(ColorValues)); 
 color->red = pal->colors[value].red;
 color->green = pal->colors[value].green;
 color->blue = pal->colors[value].blue; 
 
 return color;
 }

window * CreateWindow(float xmin, float ymin, float xmax, float ymax) {
  window * win;

  win = (window *) malloc(sizeof(window));
  win->xmin = xmin;
  win->xmax = xmax;
  win->ymin = ymin;
  win->ymax = ymax;
  
  return win;
  }

viewport * CreateViewPort(int xmin, int ymin, int xmax, int ymax) {
  viewport * port;

  port = (viewport *) malloc(sizeof(viewport));
  port->xmin = xmin;
  port->xmax = xmax;
  port->ymin = ymin;
  port->ymax = ymax;
  
  return port;
  }

// O aluno atento irá perceber que nas próximas duas funções utilizamos as respostas da primeira atividade!
point * Sru2Srn(point * ponto, window * janela) {
  point * np;

  np = (point *) malloc(sizeof(point));
  np->x = (ponto->x - janela->xmin)/(janela->xmax - janela->xmin);
  np->y = (ponto->y - janela->ymin)/(janela->ymax - janela->ymin);
  np->w = ponto->w;
  np->color = ponto->color;
  
  return np;  
  }

point * Srn2Srd(point * ponto, viewport * port) {
  point * dpt;

  dpt = (point *) malloc(sizeof(point));
  dpt->x = port->xmin + round((ponto->x)*((port->xmax - port->xmin) - 1));
  dpt->y = port->ymin + round((ponto->y)*((port->ymax - port->ymin) - 1));
  dpt->w = ponto->w;
  dpt->color = ponto->color;
 
  return dpt;
  }

bufferdevice * CreateBuffer(int maxx, int maxy) {  // O buffer nesse caso se refere a um SRD
  bufferdevice * dev;
  
  dev = (bufferdevice *) malloc(sizeof(bufferdevice));
  dev->MaxX = maxx;
  dev->MaxY = maxy;
  dev->buffer = (int *) malloc(maxx*maxy*sizeof(int));
    
  return dev;
  }

int DrawObject(object * ob, window * win, viewport * port, bufferdevice * dev, int color) {

  // A cor do objeto pode ser alterada no parâmetro color ou
  // pode-se implementar uma função que faça isso separadamente

  int i;
  float aux;
  point * p1, * p2, * paux;

  // Os limites das viewports devem ser traçados 
  // diretamente no SRD e não no SRU.
  // O uso dessa função foi sugerido pelo professor
  // como alternativa e foi implementada nesta versão
  // para fins de ilustração 
  DrawViewPort(port,dev,1);
  
  for(i=0;i<ob->numbers_of_points;i++) {
    p1 = SetPoint(ob->points[i].x,ob->points[i].y,ob->points[i].w,ob->points[i].color);
    p2 = SetPoint(ob->points[(i+1)%ob->numbers_of_points].x,ob->points[(i+1)%ob->numbers_of_points].y,ob->points[(i+1)%ob->numbers_of_points].w,ob->points[(i+1)%ob->numbers_of_points].color);
    
    if (p1->y > p2->y) {
      aux = p1->y;
      p1->y = p2->y;
      p2->y = aux;
      aux = p1->x;
      p1->x = p2->x;
      p2->x = aux;
      }   
    if ((p1->y < win->ymax)&&(p2->y > win->ymax)) {
      paux = InterY(p1,p2,win->ymax);
      if (InWin(paux,win)) {
        p2 = paux;
        } 
      }
    if ((p1->y < win->ymin)&&(p2->y > win->ymin)) {
      paux = InterY(p1,p2,win->ymin);
      if (InWin(paux,win)) {
        p1 = paux;
        } 
      }

    if (p1->x > p2->x) {
      aux = p1->y;
      p1->y = p2->y;
      p2->y = aux;
      aux = p1->x;
      p1->x = p2->x;
      p2->x = aux;
      }      
    if ((p1->x < win->xmax)&&(p2->x > win->xmax)) {
      paux = InterX(p1,p2,win->xmax);
      if (InWin(paux,win)) {
        p2 = paux;
        } 
      }
    if ((p1->x < win->xmin)&&(p2->x > win->xmin)) {
      paux = InterX(p1,p2,win->xmin);
      if (InWin(paux,win)) {
        p1 = paux;
        } 
      }

    if ((InWin(p1,win))&&(InWin(p2,win)))
        // Cuidado! A função DrawLine() foi escrita de modo a trabalhar apenas se chamada pela DrawObject()
	DrawLine(p1,p2,win,port,dev,color); 
    }

  return 0;
  }

matrix * SetRotMatrix(float theta) {
  matrix * m;

  m = (matrix *) malloc(sizeof(matrix));
  
  m->a11 = cos((theta*M_PI)/180.0); 
  m->a12 = (-1.0)*sin((theta*M_PI)/180.0);
  m->a13 = 0.0;

  m->a21 = sin((theta*M_PI)/180.0);
  m->a22 = cos((theta*M_PI)/180.0);
  m->a23 = 0.0;

  m->a31 = 0.0;
  m->a32 = 0.0;
  m->a33 = 1.0;
  
  return m;
  }

matrix * SetSclMatrix(float sx, float sy) {
  matrix * m;

  m = (matrix *) malloc(sizeof(matrix));
  
  m->a11 = sx;  m->a12 = 0.0;  m->a13 = 0.0;
  m->a21 = 0.0; m->a22 = sy;   m->a23 = 0.0;
  m->a31 = 0.0; m->a32 = 0.0;  m->a33 = 1.0;
  
  return m;
  }

matrix * SetSftMatrix(float dx, float dy) {
  matrix * m;

  m = (matrix *) malloc(sizeof(matrix));
  
  m->a11 = 1.0;  m->a12 = 0.0;  m->a13 = dx;
  m->a21 = 0.0;  m->a22 = 1.0;  m->a23 = dy;
  m->a31 = 0.0;  m->a32 = 0.0;  m->a33 = 1.0;
  
  return m;
  }

point * LinearTransf(matrix * m, point * p) {
  point * pt;
  
  pt = (point *) malloc(sizeof(point));
  
  pt->x = m->a11*p->x + m->a12*p->y + m->a13*p->w;
  pt->y = m->a21*p->x + m->a22*p->y + m->a23*p->w;
  pt->w = m->a31*p->x + m->a32*p->y + m->a33*p->w;
  pt->color = p->color;
  
  return pt;
  }

matrix * ComposeMatrix(matrix * m1, matrix * m2) {
  matrix * m;

  m = (matrix *) malloc(sizeof(matrix));
  
  m->a11 = m1->a11*m2->a11 + m1->a12*m2->a21 + m1->a13*m2->a31;
  m->a12 = m1->a11*m2->a12 + m1->a12*m2->a22 + m1->a13*m2->a32;
  m->a13 = m1->a11*m2->a13 + m1->a12*m2->a23 + m1->a13*m2->a33;
  
  m->a21 = m1->a21*m2->a11 + m1->a22*m2->a21 + m1->a23*m2->a31;
  m->a22 = m1->a21*m2->a12 + m1->a22*m2->a22 + m1->a23*m2->a32;
  m->a23 = m1->a21*m2->a13 + m1->a22*m2->a23 + m1->a23*m2->a33;
  
  m->a31 = m1->a31*m2->a11 + m1->a32*m2->a21 + m1->a33*m2->a31;
  m->a32 = m1->a31*m2->a12 + m1->a32*m2->a22 + m1->a33*m2->a32;
  m->a33 = m1->a31*m2->a13 + m1->a32*m2->a23 + m1->a33*m2->a33;
  
  return m;
  }

object * TransObj(object * ob, matrix * m) {
  int i;
  object * obj;
  point * p, * pt;

  obj = CreateObject(ob->numbers_of_points);
  
  for(i=0;i<ob->numbers_of_points;i++) {
     p = SetPoint(ob->points[i].x,ob->points[i].y,ob->points[i].w,ob->points[i].color);
     pt = LinearTransf(m,p);
     SetObject(SetPoint(pt->x,pt->y,pt->w,pt->color),obj);
     }

  return obj;
  }


