/* Feito por:

Lucas Vinícius Domingues 769699
Rafael Yoshio Yamawaki Murata 769681
Victor Luís Aguilar Antunes 769734
*/

#include "src/cg3d.h"
#include <math.h>
#define PI 3.14159265

object3d *Rotacionar(matrix3d *matrizP, object3d *ob){ //realiza a transformação linear de rotação
    
    object3d *nob = CreateObject3D(5);
    point3d *new_point = (point3d *)malloc(sizeof(point3d));
    
    for (int i = 0; i < ob->numbers_of_faces; i++)
    {
        face *nf = CreateFace(ob->faces[i].numbers_of_points);
        for (int j = 0; j < ob->faces[i].numbers_of_points; j++)
        {
            new_point = LinearTransf3d(matrizP, &ob->faces[i].points[j]);
            SetPointFace(new_point, nf);
        }
        SetObject3D(nf, nob);
    }
    return nob;
}

void setMatrizX(int angulo, matrix3d *Px){

    float q = angulo * (PI / 180.0);	// Conversão do ângulo para radianos

    // matriz rotação eixo x
    Px->a11 = 1;
    Px->a12 = 0;
    Px->a13 = 0;
    Px->a21 = 0;
    Px->a22 = cos(q);
    Px->a23 = -sin(q);
    Px->a31 = 0;
    Px->a32 = sin(q);
    Px->a33 = cos(q);

}

void setMatrizY(int angulo, matrix3d *Py){

    float q = angulo * (PI / 180.0);	// Conversão do ângulo para radianos

    // matriz rotação eixo y
    Py->a11 = cos(q);
    Py->a12 = 0;
    Py->a13 = -sin(q);
    Py->a21 = 0;
    Py->a22 = 1;
    Py->a23 = 0;
    Py->a31 = sin(q);
    Py->a32 = 0;
    Py->a33 = cos(q);
}
void setMatrizZ(int angulo, matrix3d *Pz){

    float q = angulo * (PI / 180.0);	// Conversão do ângulo para radianos

    // matriz rotação eixo z
    Pz->a11 = cos(q);
    Pz->a12 = -sin(q);
    Pz->a13 = 0;
    Pz->a21 = sin(q);
    Pz->a22 = cos(q);
    Pz->a23 = 0;
    Pz->a31 = 0;
    Pz->a32 = 0;
    Pz->a33 = 1;
}

object3d* RotacionaX(int angulo, object3d *cob){
    matrix3d * Px;
    Px = (matrix3d *)malloc(sizeof(matrix3d));
    setMatrizX(angulo,Px);
    object3d *cob_new = ConvertObjectBase(Px, Rotacionar(Px, cob));              // prisma modificado no eixo x
    return cob_new;
}
object3d* RotacionaY(int angulo, object3d *cob){
    matrix3d * Py;
    Py = (matrix3d *)malloc(sizeof(matrix3d));
    setMatrizY(angulo,Py);
    object3d *cob_new = ConvertObjectBase(Py, Rotacionar(Py, cob));             //prisma modificado no eixo y
    return  cob_new;
}
object3d* RotacionaZ(int angulo,object3d *cob){
    matrix3d *Pz;
    Pz = (matrix3d *)malloc(sizeof(matrix3d));
    setMatrizZ(angulo,Pz);
    object3d *cob_new = ConvertObjectBase(Pz, Rotacionar(Pz, cob));    //prisma modificado no eixo z
    return cob_new;  
    
}

object3d* Rotaciona3Eixos(int angulo,object3d *cob){
    object3d *cob_new = RotacionaX(angulo, cob);
    cob_new = RotacionaY(angulo, cob_new);
    cob_new = RotacionaZ(angulo, cob_new);                              //prisma modificado nos 3 eixos
    return cob_new;
}

int main(void)
{
    point3d *p;
    face *f1, *f2, *f3, *f4, *f5, *nf1, *nf2, *nf3, *nf4, *nf5;
    object3d *ob, *cob;
    point3d *u, *v, *vu, *w;
    matrix3d *H;
    object *faces;

    palette *palheta;
    bufferdevice *dispositivo;
    window *janela;
    viewport *porta;

    float zcp, zpp;
    int angulo = 0;
    char eixo;

    palheta = CreatePalette(6);
    SetColor(0, 0, 0, palheta);
    SetColor(1, 1, 1, palheta);
    SetColor(1, 1, 0, palheta);
    SetColor(1, 0, 0, palheta);
    SetColor(0, 1, 0, palheta);
    SetColor(0, 0, 1, palheta);

    p = (point3d *)malloc(sizeof(point3d));

    f1 = CreateFace(3);
    p->x = 10.0;
    p->y = 10.0;
    p->z = 0.0;
    p->color = 1;
    SetPointFace(p, f1);
    p->x = 10.0;
    p->y = 0.0;
    p->z = 15.0;
    p->color = 1;
    SetPointFace(p, f1);
    p->x = 10.0;
    p->y = -10.0;
    p->z = 0.0;
    p->color = 1;
    SetPointFace(p, f1);

    f2 = CreateFace(3);
    p->x = -10.0;
    p->y = 10.0;
    p->z = 0.0;
    p->color = 2;
    SetPointFace(p, f2);
    p->x = -10.0;
    p->y = 0.0;
    p->z = 15.0;
    p->color = 2;
    SetPointFace(p, f2);
    p->x = -10.0;
    p->y = -10.0;
    p->z = 0.0;
    p->color = 2;
    SetPointFace(p, f2);

    f3 = CreateFace(4);
    p->x = 10.0;
    p->y = 10.0;
    p->z = 0.0;
    p->color = 3;
    SetPointFace(p, f3);
    p->x = 10.0;
    p->y = 0.0;
    p->z = 15.0;
    p->color = 3;
    SetPointFace(p, f3);
    p->x = -10.0;
    p->y = 0.0;
    p->z = 15.0;
    p->color = 3;
    SetPointFace(p, f3);
    p->x = -10.0;
    p->y = 10.0;
    p->z = 0.0;
    p->color = 3;
    SetPointFace(p, f3);

    f4 = CreateFace(4);
    p->x = 10.0;
    p->y = 0.0;
    p->z = 15.0;
    p->color = 4;
    SetPointFace(p, f4);
    p->x = 10.0;
    p->y = -10.0;
    p->z = 0.0;
    p->color = 4;
    SetPointFace(p, f4);
    p->x = -10.0;
    p->y = -10.0;
    p->z = 0.0;
    p->color = 4;
    SetPointFace(p, f4);
    p->x = -10.0;
    p->y = 0.0;
    p->z = 15.0;
    p->color = 4;
    SetPointFace(p, f4);

    f5 = CreateFace(4);
    p->x = 10.0;
    p->y = 10.0;
    p->z = 0.0;
    p->color = 5;
    SetPointFace(p, f5);
    p->x = 10.0;
    p->y = -10.0;
    p->z = 0.0;
    p->color = 5;
    SetPointFace(p, f5);
    p->x = -10.0;
    p->y = -10.0;
    p->z = 0.0;
    p->color = 5;
    SetPointFace(p, f5);
    p->x = -10.0;
    p->y = 10.0;
    p->z = 0.0;
    p->color = 5;
    SetPointFace(p, f5);

    ob = CreateObject3D(5);
    SetObject3D(f1, ob);
    SetObject3D(f2, ob);
    SetObject3D(f3, ob);
    SetObject3D(f4, ob);
    SetObject3D(f5, ob);

    w = (point3d *)malloc(sizeof(point3d)); // vetor normal ao plano de visualização (vetor dado!)
    w->x = 0;
    w->y = 0;
    w->z = -1;

    vu = (point3d *)malloc(sizeof(point3d)); // view-up (vetor dado!)
    vu->x = 0;
    vu->y = 1;
    vu->z = 0;

    u = VectorProduct3d(vu, w);
    v = VectorProduct3d(w, u);

    H = (matrix3d *)malloc(sizeof(matrix3d));
    H->a11 = u->x;
    H->a12 = u->y;
    H->a13 = u->z;
    H->a21 = v->x;
    H->a22 = v->y;
    H->a23 = v->z;
    H->a31 = w->x;
    H->a32 = w->y;
    H->a33 = w->z;

    cob = ConvertObjectBase(H, ob);      // prisma original
    object3d *cob_x = cob;
    object3d *cob_y = cob;
    object3d *cob_z = cob;
    object3d *cob_new;

    printf("Digite em qual eixo o prisma será rotacionado(x,y,z ou rotação simultânea dos 3 eixos(digite s): ");
    scanf("%c",&eixo);
    printf("Digite em quantos graus o prisma sera rotacionado: ");
    scanf("%d", &angulo);

    
    switch(eixo){ //realiza rotação no eixo escolhido
    case 'x':
        cob_new = RotacionaX(angulo,cob);
        break;
    case 'y':
        cob_new = RotacionaY(angulo,cob);
        break;
    case 'z':
        cob_new = RotacionaZ(angulo,cob);
        break;
    case 's':
        cob_new = Rotaciona3Eixos(angulo,cob);
        break;
    default:
        cob_new = cob;
        printf("Entrada Inválida! o eixo deve ser x, y, z ou s para rotação simultânea dos 3 eixos\n");
    }


    //faces = ParalProjFaces(cob);
    zpp = 40.0;
    zcp = -45.0;


    faces = PerspProjFaces(cob_new, zpp, zcp);      // exibe rotação escolhida 

    janela = CreateWindow(-30, -30, 30, 30);
    dispositivo = CreateBuffer(640, 480);
    porta = CreateViewPort(0, 0, 639, 479);

    DrawObject(&faces[0], janela, porta, dispositivo, 3);
    DrawObject(&faces[1], janela, porta, dispositivo, 3);
    DrawObject(&faces[2], janela, porta, dispositivo, 3);
    DrawObject(&faces[3], janela, porta, dispositivo, 3);
    DrawObject(&faces[4], janela, porta, dispositivo, 3);

    Dump2X(dispositivo, palheta);

    return 0;
}