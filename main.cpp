#include <iostream>

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>

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

void decodage(const std::string &filename);

void codageComposante(IplImage *Y, int QP, int **&qI, int **&qM, int **&sI, int **&sM);

void decodageComposante(int **qI, int **qM, int **sI, int **sM, int QP, IplImage *&I, IplImage *&M, int w, int h);

void sample();

int main()
{
    codage("mandril.jpg", valQP::eleve);
    decodage("saveTest.xml");
    return EXIT_SUCCESS;
}

void codage(const std::string &filename, int QP)
{
    IplImage * BGR, *image = cvLoadImage(filename.c_str());
    //ajustement de l'image
    BGR = ajustementImage(image);
    IplImage *recouv = recouvrement(BGR, taille);
    recouv = ajustementImage(recouv);

    //conversion en YUV
    IplImage * YUV = cvBGR2YUV( BGR );
    IplImage * YUVrecouv = cvBGR2YUV( recouv );
    cvShowAnyImageYUV("BGR", BGR);

    //separation des composantes
    IplImage *Y, *U, *V, *ry, *ru, *rv;
    separateComponents(YUV, Y, U, V);
    separateComponents(YUVrecouv, ry, ru, rv);

    int **qyI, **quI, **qvI, **syI, **suI, **svI;
    int **qyM, **quM, **qvM, **syM, **suM, **svM;

    int **rqyI, **rquI, **rqvI, **rsyI, **rsuI, **rsvI;
    int **rqyM, **rquM, **rqvM, **rsyM, **rsuM, **rsvM;

    codageComposante(Y, QP, qyI, qyM, syI, syM);
    codageComposante(U, QP, quI, quM, suI, suM);
    codageComposante(V, QP, qvI, qvM, svI, svM);

    codageComposante(ry, QP, rqyI, rqyM, rsyI, rsyM);
    codageComposante(ru, QP, rquI, rquM, rsuI, rsuM);
    codageComposante(rv, QP, rqvI, rqvM, rsvI, rsvM);

    //sauvegarde
    std::vector<int **> vecY;
    vecY.push_back(qyI);
    vecY.push_back(qyM);
    vecY.push_back(rqyI);
    vecY.push_back(rqyM);
    std::vector<int **> vecU;
    vecU.push_back(quI);
    vecU.push_back(quM);
    vecU.push_back(rquI);
    vecU.push_back(rquM);
    std::vector<int **> vecV;
    vecV.push_back(qvI);
    vecV.push_back(qvM);
    vecV.push_back(rqvI);
    vecV.push_back(rqvM);
    std::vector<int **> stratY;
    stratY.push_back(syI);
    stratY.push_back(syM);
    stratY.push_back(rsyI);
    stratY.push_back(rsyM);
    std::vector<int **> stratU;
    stratU.push_back(suI);
    stratU.push_back(suM);
    stratU.push_back(rsuI);
    stratU.push_back(rsuM);
    std::vector<int **> stratV;
    stratV.push_back(svI);
    stratV.push_back(svM);
    stratV.push_back(rsvI);
    stratV.push_back(rsvM);
    std::vector<int> h;
    h.push_back(Y->height);
    h.push_back(Y->height/taille);
    h.push_back(ry->height);
    h.push_back(recouv->height/taille);
    std::vector<int> w;
    w.push_back(Y->width);
    w.push_back(Y->width/taille);
    w.push_back(ry->width);
    w.push_back(recouv->width/taille);
/*
    for(int ha = 0; ha < h[0]; ha++)
        for(int wa = 0; wa < w[0]; wa++)
        {
            cout << wa << ", " << ha << endl;
            cvGet2D(Y, ha, wa);
        }
*/
    saveXML("saveTest.xml", vecY, vecU, vecV , stratY, stratU, stratV, h, w, QP);
}

void decodage(const std::string &filename){
    //chargement

    IplImage *yi, *ym, *ui, *um, *vi, *vm,
    *ryi, *rym, *rui, *rum, *rvi, *rvm;
    std::vector<int **> vecY;
    std::vector<int **> vecU;
    std::vector<int **> vecV;
    std::vector<int **> stratY;
    std::vector<int **> stratU;
    std::vector<int **> stratV;
    std::vector<int> h;
    std::vector<int> w;
    int QP;

    loadXML(filename.c_str(), vecY, vecU, vecV , stratY, stratU, stratV, h, w, QP);

    decodageComposante(vecY[0], vecY[1], stratY[0], stratY[1], QP, yi, ym, w[0], h[0]);
    decodageComposante(vecU[0], vecU[1], stratU[0], stratU[1], QP, ui, um, w[0], h[0]);
    decodageComposante(vecV[0], vecV[1], stratV[0], stratV[1], QP, vi, vm, w[0], h[0]);

    //recouvrement
    decodageComposante(vecY[2], vecY[3], stratY[2], stratY[3], QP, ryi, rym, w[2], h[2]);
    decodageComposante(vecU[2], vecU[3], stratU[2], stratU[3], QP, rui, rum, w[2], h[2]);
    decodageComposante(vecV[2], vecV[3], stratV[2], stratV[3], QP, rvi, rvm, w[2], h[2]);

    IplImage *yuvI, *yuvM,
    *ryuvI, *ryuvM;
    yuvM = unifiateComponents(ym, um, vm);
    yuvI = unifiateComponents(yi, ui, vi);

    ryuvM = unifiateComponents(rym, rum, rvm);
    ryuvI = unifiateComponents(ryi, rui, rvi);

    IplImage *bgrM = cvYUV2BGR(yuvM);
    IplImage *bgrI = cvYUV2BGR(yuvI);

    IplImage *rbgrM = cvYUV2BGR(ryuvM);
    IplImage *rbgrI = cvYUV2BGR(ryuvI);

    cvShowAnyImageYUV("miniature", bgrM);
    cvShowAnyImageYUV("image", bgrI);

    cvShowAnyImageYUV("miniature R", rbgrM);
    cvShowAnyImageYUV("image R", rbgrI);

    bgrI = mergedRecouvrement(bgrI, rbgrI, taille);
    cvShowAnyImageYUV("image RO", rbgrI);
}

void codageComposante(IplImage *Y, int QP, int **&qI, int **&qM, int **&sI, int **&sM)
{
    IplImage *image, *miniature;

    image = DCT(Y, taille);
    //cout << "image : " << Y->width << ", " << Y->height << endl;
    miniature = ExplodeDCT(image, taille);
    //cout << "miniature : " << miniature->width << ", " << miniature->height << endl;
    miniature = DCT(miniature, taille);

    image = predictImage(image, taille, sI);
    miniature = predictImage(miniature, taille, sM);

    qI = applyQuantification(image, QP, R, valR::grandeImage);
    qM = applyQuantification(miniature, QP, R, valR::petiteImage);

}

void decodageComposante(int **qI, int **qM, int **sI, int **sM, int QP, IplImage *&I, IplImage *&M, int w, int h)
{
    M = ReverseApplyQuantification(qM, QP, R, valR::petiteImage, w/taille, h/taille);
    I = ReverseApplyQuantification(qI, QP, R, valR::grandeImage, w, h);

    M = ReversepredictImage(M, taille, sM);
    I = ReversepredictImage(I, taille, sI);
    //cout  << "pixel 5,1 dans I" << cvGet2D(I, 5, 1).val[0] << endl;
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

