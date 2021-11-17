#include "cg3d.h"

point3d * VectorProduct3d(point3d * u, point3d * v) {
 point3d * w;
 w = (point3d *) malloc(sizeof(point3d));
 
/* 
 i  j  k
 u1 u2 u3       u2v3i + u3v1j + u1v2k - u2v1k - u3v2i - u1v3j = (u2v3 - u3v2)i + (u3v1 - u1v3)j + (u1v2 - u2v1)k 
 v1 v2 v3
 */ 

 w->x = (u->y)*(v->z) - (u->z)*(v->y);
 w->y = (u->z)*(v->x) - (u->x)*(v->z);
 w->z = (u->x)*(v->y) - (u->y)*(v->x);
 
 w->color = 1;
 
 return w;
 }
 
float Modulo3d(point3d * u) {
 return (sqrt(((u->x)*(u->x))+((u->y)*(u->y))+((u->z)*(u->z))));
 }

point3d * Versor3d(point3d * u) {
 point3d * w;
 float modulo = Modulo3d(u);
 w = (point3d *) malloc(sizeof(point3d));

 w->x = (u->x) / modulo;
 w->y = (u->y) / modulo; 
 w->z = (u->z) / modulo;
 
 w->color = 1;
 
 return w;
 }
 
face * CreateFace(int numbers_of_points) {
  face * fob;
 
  fob = (face *) malloc(sizeof(face));
  fob->numbers_of_points = 0;
  fob->points = (point3d *) malloc(numbers_of_points*sizeof(point3d));
 
  return fob;
  }

int SetPointFace(point3d * pnt, face * fc) {
  fc->points[fc->numbers_of_points] = *pnt;
  fc->numbers_of_points = fc->numbers_of_points + 1;

  return 0;
  } 

object3d * CreateObject3D(int numbers_of_faces) {
  object3d * ob;
 
  ob = (object3d *) malloc(sizeof(object3d));
  ob->numbers_of_faces = 0;
  ob->faces = (face *) malloc(numbers_of_faces*sizeof(face));
 
  return ob;
  }

int SetObject3D(face * fc, object3d * ob) {
  ob->faces[ob->numbers_of_faces] = *fc;
  ob->numbers_of_faces = ob->numbers_of_faces + 1;

  return 0;
  }

point3d * LinearTransf3d(matrix3d * W, point3d * u) {
 point3d * p;
  
 p = (point3d *) malloc(sizeof(point3d));
  
 p->x = W->a11*u->x + W->a12*u->y + W->a13*u->z;
 p->y = W->a21*u->x + W->a22*u->y + W->a23*u->z;
 p->z = W->a31*u->x + W->a32*u->y + W->a33*u->z;
 
 p->color = u->color;
  
 return p;  
 }

object3d * ConvertObjectBase(matrix3d * M, object3d * ob) {
 object3d * mob;
 point3d * p0, * p1;
 int numbers_of_points, numbers_of_faces;
  
 p0 = (point3d *) malloc(sizeof(point3d));
 p1 = (point3d *) malloc(sizeof(point3d));
 
 mob = (object3d *) malloc(sizeof(object3d));
 mob->numbers_of_faces = ob->numbers_of_faces;
 mob->faces = (face *) malloc(ob->numbers_of_faces*sizeof(face));
 
 for(numbers_of_faces=0;numbers_of_faces<ob->numbers_of_faces;numbers_of_faces++) {
   mob->faces[numbers_of_faces].numbers_of_points = ob->faces[numbers_of_faces].numbers_of_points;
   mob->faces[numbers_of_faces].points = (point3d *) malloc(ob->faces[numbers_of_faces].numbers_of_points*sizeof(point3d));
   for(numbers_of_points=0;numbers_of_points<ob->faces[numbers_of_faces].numbers_of_points;numbers_of_points++) {
     p0->x = ob->faces[numbers_of_faces].points[numbers_of_points].x;
     p0->y = ob->faces[numbers_of_faces].points[numbers_of_points].y;
     p0->z = ob->faces[numbers_of_faces].points[numbers_of_points].z;
     p0->color = ob->faces[numbers_of_faces].points[numbers_of_points].color;
     p1 = LinearTransf3d(M,p0); 
     mob->faces[numbers_of_faces].points[numbers_of_points].x = p1->x;    
     mob->faces[numbers_of_faces].points[numbers_of_points].y = p1->y; 
     mob->faces[numbers_of_faces].points[numbers_of_points].z = p1->z;
     mob->faces[numbers_of_faces].points[numbers_of_points].color = p1->color;
     }
   } 

 return mob; 
 }
 
object * ParalProjFaces(object3d * ob3d) {
 object * facelist;
 int numbers_of_points, numbers_of_faces;
 
 facelist = (object *) malloc(ob3d->numbers_of_faces*sizeof(object));
 for(numbers_of_faces=0;numbers_of_faces<ob3d->numbers_of_faces;numbers_of_faces++) {
   facelist[numbers_of_faces].numbers_of_points = ob3d->faces[numbers_of_faces].numbers_of_points;
   facelist[numbers_of_faces].points = (point *) malloc(facelist[numbers_of_faces].numbers_of_points*sizeof(point));
   for(numbers_of_points=0;numbers_of_points<ob3d->faces[numbers_of_faces].numbers_of_points;numbers_of_points++) {
     facelist[numbers_of_faces].points[numbers_of_points].x = ob3d->faces[numbers_of_faces].points[numbers_of_points].x;
     facelist[numbers_of_faces].points[numbers_of_points].y = ob3d->faces[numbers_of_faces].points[numbers_of_points].y;
     facelist[numbers_of_faces].points[numbers_of_points].w = 1;
     facelist[numbers_of_faces].points[numbers_of_points].color = ob3d->faces[numbers_of_faces].points[numbers_of_points].color;      
     }
   }
 
 return facelist;
 }

object * PerspProjFaces(object3d * ob3d, float zpp, float zcp) {
 object * facelist;
 int numbers_of_points, numbers_of_faces;
 
 facelist = (object *) malloc(ob3d->numbers_of_faces*sizeof(object));
 for(numbers_of_faces=0;numbers_of_faces<ob3d->numbers_of_faces;numbers_of_faces++) {
   facelist[numbers_of_faces].numbers_of_points = ob3d->faces[numbers_of_faces].numbers_of_points;
   facelist[numbers_of_faces].points = (point *) malloc(facelist[numbers_of_faces].numbers_of_points*sizeof(point));
   for(numbers_of_points=0;numbers_of_points<ob3d->faces[numbers_of_faces].numbers_of_points;numbers_of_points++) {
     facelist[numbers_of_faces].points[numbers_of_points].x = ((ob3d->faces[numbers_of_faces].points[numbers_of_points].x)*(zpp-zcp))/(ob3d->faces[numbers_of_faces].points[numbers_of_points].z-zcp);
     facelist[numbers_of_faces].points[numbers_of_points].y = ((ob3d->faces[numbers_of_faces].points[numbers_of_points].y)*(zpp-zcp))/(ob3d->faces[numbers_of_faces].points[numbers_of_points].z-zcp);
     facelist[numbers_of_faces].points[numbers_of_points].w = 1;
     facelist[numbers_of_faces].points[numbers_of_points].color = ob3d->faces[numbers_of_faces].points[numbers_of_points].color;      
     }
   }
 
 return facelist;  
 }
 
