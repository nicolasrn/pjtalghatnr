#include <iostream>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "lib.h"
#include "libXML.h"

using namespace std;

static int R[2] = {2, 0};
static int taille = 4;

struct valQP
{
    static const int faible = 4, moyen = 3, haut = 2, eleve = 1;
};

struct valR
{
    static const int grandeImage = 1, petiteImage = 0;
};

void codage(const std::string &filename, int QP);

void codageComposante(IplImage *Y, int QP, int **&qI, int **&qM, int **&sI, int **&sM);

void decodage(int **qI, int **qM, int **sI, int **sM, int QP, IplImage *&I, IplImage *&M, int w, int h);

void sample();

int main()
{
    codage("mandril.jpg", valQP::eleve);
    return EXIT_SUCCESS;
}

void codage(const std::string &filename, int QP)
{
    IplImage * BGR, *image = cvLoadImage(filename.c_str());
    //ajustement de l'image
    BGR = ajustementImage(image);
    //conversion en YUV
    IplImage * YUV = cvBGR2YUV( BGR );
    cvShowAnyImageYUV("BGR", BGR);

    //separation des composantes
    IplImage *Y, *U, *V;
    separateComponents(YUV, Y, U, V);

    int **qyI, **quI, **qvI, **syI, **suI, **svI;
    int **qyM, **quM, **qvM, **syM, **suM, **svM;

    codageComposante(Y, QP, qyI, qyM, syI, syM);
    codageComposante(U, QP, quI, quM, suI, suM);
    codageComposante(V, QP, qvI, qvM, svI, svM);

    //sauvegarde
    //ici la sauvegarde

    //chargement
    IplImage *yi, *ym, *ui, *um, *vi, *vm;
    decodage(qyI, qyM, syI, syM, QP, yi, ym, Y->width, Y->height);
    decodage(quI, quM, suI, suM, QP, ui, um, Y->width, Y->height);
    decodage(qvI, qvM, svI, svM, QP, vi, vm, Y->width, Y->height);

    IplImage *yuvI, *yuvM;
    yuvM = unifiateComponents(ym, um, vm);
    yuvI = unifiateComponents(yi, ui, vi);
    IplImage *bgrM = cvYUV2BGR(yuvM);
    IplImage *bgrI = cvYUV2BGR(yuvI);
    cvShowAnyImageYUV("miniature", bgrM);
    cvShowAnyImageYUV("image", bgrI);
}

void codageComposante(IplImage *Y, int QP, int **&qI, int **&qM, int **&sI, int **&sM)
{
    IplImage *image, *miniature;

    image = DCT(Y, taille);
    miniature = ExplodeDCT(image, taille);
    miniature = DCT(miniature, taille);

    image = predictImage(image, taille, sI);
    miniature = predictImage(miniature, taille, sM);

    qI = applyQuantification(image, QP, R, valR::grandeImage);
    qM = applyQuantification(miniature, QP, R, valR::petiteImage);
}

void decodage(int **qI, int **qM, int **sI, int **sM, int QP, IplImage *&I, IplImage *&M, int w, int h)
{
    M = ReverseApplyQuantification(qM, QP, R, valR::petiteImage, w/taille, h/taille);
    I = ReverseApplyQuantification(qI, QP, R, valR::grandeImage, w, h);

    M = ReversepredictImage(M, taille, sM);
    I = ReversepredictImage(I, taille, sI);

    M = InverseDCT(M, taille);
    I = MergeDCT(I, M, taille);
    I = InverseDCT(I, taille);
}

void sample()
{
    IplImage *reverse = cvCreateImage(cvSize(8, 8), IPL_DEPTH_64F, 1);
    double *data = (double*)reverse->imageData;
    data[0] = 1;
    data[1] = 5;
    data[2] = 6;
    data[3] = 2;
    data[4] = 4;
    data[5] = 2;
    data[6] = 6;
    data[7] = 9;
    data[8] = 3;
    data[9] = 4;
    data[10] = 5;
    data[11] = 8;
    data[12] = 2;
    data[13] = 0;
    data[14] = 7;
    data[15] = 8;
    data[16] = 9;
    data[17] = 0;
    data[18] = 3;
    data[19] = 6;
    data[20] = 8;
    data[21] = 8;
    data[22] = 5;
    data[23] = 7;
    data[24] = 2;
    data[25] = 5;
    data[26] = 4;
    data[27] = 2;
    data[28] = 7;
    data[29] = 9;
    data[30] = 2;
    data[31] = 6;

    data[32] = 5;
    data[33] = 7;
    data[34] = 8;
    data[35] = 1;
    data[36] = 3;
    data[37] = 2;
    data[38] = 6;
    data[39] = 5;
    data[40] = 4;
    data[41] = 6;
    data[42] = 2;
    data[43] = 5;
    data[44] = 9;
    data[45] = 1;
    data[46] = 8;
    data[47] = 4;
    data[48] = 3;
    data[49] = 1;
    data[50] = 9;
    data[51] = 8;
    data[52] = 6;
    data[53] = 3;
    data[54] = 9;
    data[55] = 3;
    data[56] = 7;
    data[57] = 6;
    data[58] = 5;
    data[59] = 4;
    data[60] = 3;
    data[61] = 2;
    data[62] = 1;
    data[63] = 0;

    //IplImage *reverse = ReverseApplyQuantification(q, valQP::faible, Resolution, valR::grandeImage, pred->width, pred->height);
    int **strategie;
    CvScalar a;
    cout << "Table origine" << endl;
    for(int i = 0; i < reverse->height; i++)
    {
        for(int j = 0; j < reverse->width; j++)
        {
            a = cvGet2D(reverse, i, j);
            //cout << "res : (" << i << ", " << j << ") : " << a.val[0] << endl;// << " ? " << b.val[0] << " strategie : " << strategie[j][i] << endl;
            cout << a.val[0] << " ";
        }
        cout << endl;
    }

    cout << "predict image" << endl;
    reverse = predictImage(reverse, 4, strategie);
    for(int i = 0; i < reverse->height; i++)
    {
        for(int j = 0; j < reverse->width; j++)
        {
            a = cvGet2D(reverse, i, j);
            //cout << "res : (" << i << ", " << j << ") : " << a.val[0] << endl;// << " ? " << b.val[0] << " strategie : " << strategie[j][i] << endl;
            cout << a.val[0] << " ";
        }
        cout << endl;
    }

    cout << endl << "Strategie " << endl;
    for(int i = 0; i < reverse->height; i++)
    {
        for(int j = 0; j < reverse->width; j++)
        {
            a = cvGet2D(reverse, i, j);
            //cout << "res : (" << i << ", " << j << ") : " << a.val[0] << endl;// << " ? " << b.val[0] << " strategie : " << strategie[j][i] << endl;
            cout << strategie[j][i] << " ";
        }
        cout << endl;
    }

    reverse = ReversepredictImage(reverse, 4, strategie);
    cout << "reverse" << endl;
    for(int i = 0; i < reverse->height; i++)
    {
        for(int j = 0; j < reverse->width; j++)
        {
            a = cvGet2D(reverse, i, j);
            //cout << "res : (" << i << ", " << j << ") : " << a.val[0] << endl;// << " ? " << b.val[0] << " strategie : " << strategie[j][i] << endl;
            cout << a.val[0] << " ";
        }
        cout << endl;
    }
}

