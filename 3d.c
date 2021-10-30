/* Feito por:

Lucas Vinícius Domingues 769699
Rafael Yoshio Yamawaki Murata 769681
Victor Luís Aguilar Antunes 769734
*/

#include "src/cg3d.h"
#include <math.h>
#define PI 3.14159265

matrix3d *Px, *Py, *Pz;

object3d *Rotacionar(matrix3d *matrizP, object3d *ob)
{
    //realiza a transformação linear de rotação
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

void setMatrizX(float q)
{
    Px = (matrix3d *)malloc(sizeof(matrix3d));
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

void setMatrizY(float q)
{
    Py = (matrix3d *)malloc(sizeof(matrix3d));
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

void setMatrizZ(float q)
{
    Pz = (matrix3d *)malloc(sizeof(matrix3d));
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

object3d *RotacionaX(matrix3d *Px, object3d *cob)
{
    object3d *cob_new = ConvertObjectBase(Px, Rotacionar(Px, cob)); // prisma modificado no eixo x
    return cob_new;
}

object3d *RotacionaY(matrix3d *Py, object3d *cob)
{
    object3d *cob_new = ConvertObjectBase(Py, Rotacionar(Py, cob)); //prisma modificado no eixo y
    return cob_new;
}

object3d *RotacionaZ(matrix3d *Pz, object3d *cob)
{
    object3d *cob_new = ConvertObjectBase(Pz, Rotacionar(Pz, cob)); //prisma modificado no eixo z
    return cob_new;
}

matrix3d *MultiplicaM3d(matrix3d *M, matrix3d *N)
{
    matrix3d *m_new = (matrix3d *)malloc(sizeof(matrix3d));

    m_new->a11 = M->a11 * N->a11 + M->a12 * N->a21 + M->a13 * N->a31;
    m_new->a12 = M->a11 * N->a12 + M->a12 * N->a22 + M->a13 * N->a32;
    m_new->a13 = M->a11 * N->a13 + M->a12 * N->a23 + M->a13 * N->a33;
    m_new->a21 = M->a21 * N->a11 + M->a22 * N->a21 + M->a23 * N->a31;
    m_new->a22 = M->a21 * N->a12 + M->a22 * N->a22 + M->a23 * N->a32;
    m_new->a23 = M->a21 * N->a13 + M->a22 * N->a23 + M->a23 * N->a33;
    m_new->a31 = M->a31 * N->a11 + M->a32 * N->a21 + M->a33 * N->a31;
    m_new->a32 = M->a31 * N->a12 + M->a32 * N->a22 + M->a33 * N->a32;
    m_new->a33 = M->a31 * N->a13 + M->a32 * N->a23 + M->a33 * N->a33;

    return m_new;
}

object3d *Rotaciona3Eixos(object3d *cob)
{
    // matriz rotação 3 eixos
    matrix3d *P = (matrix3d *)malloc(sizeof(matrix3d));

    P = MultiplicaM3d(Px, MultiplicaM3d(Py, Pz));

    object3d *cob_new = ConvertObjectBase(P, Rotacionar(P, cob)); //prisma modificado nos 3 eixos
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

    printf("Digite em qual eixo o prisma será rotacionado(x,y,z ou rotação simultânea dos 3 eixos(digite s)): ");
    scanf("%c", &eixo);
    printf("Digite em quantos graus o prisma sera rotacionado: ");
    scanf("%d", &angulo);

    float q = angulo * (PI / (180.0 * 2)); // Conversão do ângulo para radianos

    object3d *ob_new;

    switch (eixo)
    { //realiza rotação no eixo escolhido
    case 'x':
        setMatrizX(q);
        ob_new = RotacionaX(Px, ob);
        break;
    case 'y':
        setMatrizY(q);
        ob_new = RotacionaY(Py, ob);
        break;
    case 'z':
        setMatrizZ(q);
        ob_new = RotacionaZ(Pz, ob);
        break;
    case 's':
        setMatrizX(q);
        setMatrizY(q);
        setMatrizZ(q);
        ob_new = Rotaciona3Eixos(ob);
        break;
    default:
        ob_new = cob;
        printf("Entrada Inválida! o eixo deve ser x, y, z ou s para rotação simultânea dos 3 eixos\n");
    }

    //faces = ParalProjFaces(cob);
    cob = ConvertObjectBase(H, ob_new);

    zpp = 40.0;
    zcp = -45.0;

    faces = PerspProjFaces(cob, zpp, zcp); // exibe rotação escolhida

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