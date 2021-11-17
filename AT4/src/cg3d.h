/* 
   Autor: Murillo Rodrigo Petrucelli Homem
   Departamento de Computação
   Universidade Federal de São Carlos

   Código ilustrativo (esboço) de uma pequena biblioteca gráfica tridimensional
*/

#include "cg2d.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Point3D {
  float x,
        y,
	z;
  int   color;
  } point3d;
      
typedef struct Face {
  int numbers_of_points;
  point3d * points;
  } face;
  
typedef struct Object3D {
  int numbers_of_faces;
  face * faces;
  } object3d;

typedef struct Matrix3D {
  float a11, a12, a13,
        a21, a22, a23,
        a31, a32, a33;
        } matrix3d;

point3d * VectorProduct3d(point3d *, point3d *);
float Modulo3d(point3d *);
point3d * Versor3d(point3d *);
face * CreateFace(int);
int SetPointFace(point3d *, face *);
object3d * CreateObject3D(int);
int SetObject3D(face *, object3d *);
point3d * LinearTransf3d(matrix3d *, point3d *);
object3d * ConvertObjectBase(matrix3d *, object3d *);
object * ParalProjFaces(object3d *);
object * PerspProjFaces(object3d *, float, float);



