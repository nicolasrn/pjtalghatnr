/**
 * \file main.cpp
 * \brief programme de codage & décodage
 */

#include <iostream>

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>

#include "lib.h"
#include "libXML.h"

using namespace std;

static int R[2] = {2, 0};
static int taille = 4;

///permet de nommer certaines variables
struct valQP
{
    static const int faible = 4, moyen = 3, haut = 2, eleve = 1;
};

///contient le type de l'image en fonction du tableau R
struct valR
{
    static const int grandeImage = 1, petiteImage = 0;
};

///permet de lancer l'interface utilisateur
void interface_utilisateur();

/**
 * permet de faire le codage d'une image avec QP, et un caractère pour faire le recouvrement
 * execute aussi la sauvegarde XML
 * @param filename : le nom de fichier a charger
 * @param QP : le parametre de qualite
 * @param recouv : o pour faire un recouvrement, autre chose sinon
 */
void codage(const std::string &filename, int QP,char recouv);

/**
 * permet de faire le codage d'une image avec QP, et un caractère pour faire le recouvrement
 * @param filename : le fichier a decoder
 */
void decodage(const std::string &filename);

/**
 * permet de faire le codage d'une composante de l'image
 * @param Y : la composante a decoder
 * @param QP : le parametre de qualite
 * @param qI : (out)tableau de quantification de l'image
 * @param qM : (out)tableau de quantification de la miniature
 * @param sI : (out)tableau de strategie de l'image
 * @param sM : (out)tableau de strategie de la miniature
 */
void codageComposante(IplImage *Y, int QP, double **&qI, double **&qM, int **&sI, int **&sM);

/**
 * permet de faire le codage d'une composante de l'image
 * @param qI : tableau de quantification de l'image
 * @param qM : tableau de quantification de la miniature
 * @param sI : tableau de strategie de l'image
 * @param sM : tableau de strategie de la miniature
 * @param QP : le parametre de qualite
 * @param I : (out)l'image resultant du decodage
 * @param M : (out)la miniature resultant du decodage
 */
void decodageComposante(double **qI, double **qM, int **sI, int **sM, int QP, IplImage *&I, IplImage *&M, int w, int h);

int main()
{
    interface_utilisateur();
    return EXIT_SUCCESS;
}

void interface_utilisateur()
{
    std::string nom_fichier ;
    int qp;
    char recouv;
    cout << "Bienvenu sur le projet Multimedia " << endl << "Auteurs : Tauraatua Aurore / Huguin Gabriel / Lefumeur Alexandre / Reitz Nicolas" << endl << endl;
    cout << "Veuillez entrer le nom du fichier a traiter (avec l'extension)" << endl;
    cin >> nom_fichier;
    cout << "Veuillez entrer le parametre de qualite QP " << endl;
    cout << "de 1 = eleve a 4 = faible" << endl;
    cin >> qp;
    cout << "Voulez-vous utiliser le recouvrement ? ( o -> oui, n -> non ) " << endl;
    cin >> recouv;
    codage(nom_fichier, qp, recouv);
    decodage("saveTest.xml");
}

void codage(const std::string &filename, int QP,char recv)
{
    IplImage * BGR, *image = cvLoadImage(filename.c_str()), *YUVrecouv, *recouv;
    //ajustement de l'image
    BGR = ajustementImage(image);
    if ( recv == 'o')
    {
        recouv = recouvrement(BGR, taille);
        recouv = ajustementImage(recouv);
    }

    //conversion en YUV
    IplImage * YUV = cvBGR2YUV( BGR );
    if(recv == 'o')
    {
        YUVrecouv = cvBGR2YUV( recouv );
    }
    cvShowAnyImageYUV("Image d'origine", BGR);
    //separation des composantes
    IplImage *Y, *U, *V, *ry, *ru, *rv;
    separateComponents(YUV, Y, U, V);
    if(recv == 'o')
        separateComponents(YUVrecouv, ry, ru, rv);

    double **qyI, **quI, **qvI;
    int **syI, **suI, **svI;

    double **qyM, **quM, **qvM;
    int **syM, **suM, **svM;

    double **rqyI, **rquI, **rqvI;
    int **rsyI, **rsuI, **rsvI;

    double **rqyM, **rquM, **rqvM;
    int **rsyM, **rsuM, **rsvM;

    codageComposante(Y, QP, qyI, qyM, syI, syM);
    codageComposante(U, QP, quI, quM, suI, suM);
    codageComposante(V, QP, qvI, qvM, svI, svM);
    if(recv == 'o')
    {
        codageComposante(ry, QP, rqyI, rqyM, rsyI, rsyM);
        codageComposante(ru, QP, rquI, rquM, rsuI, rsuM);
        codageComposante(rv, QP, rqvI, rqvM, rsvI, rsvM);
    }
    cout<<"Sauvegarde ... "<<endl;
    //sauvegarde
    std::vector<double **> vecY;
    vecY.push_back(qyI);
    vecY.push_back(qyM);
    if(recv == 'o')
    {
        vecY.push_back(rqyI);
        vecY.push_back(rqyM);
    }

    std::vector<double **> vecU;
    vecU.push_back(quI);
    vecU.push_back(quM);
    if(recv == 'o')
    {
        vecU.push_back(rquI);
        vecU.push_back(rquM);
    }
    std::vector<double **> vecV;
    vecV.push_back(qvI);
    vecV.push_back(qvM);
    if(recv == 'o')
    {
        vecV.push_back(rqvI);
        vecV.push_back(rqvM);
    }
    std::vector<int **> stratY;
    stratY.push_back(syI);
    stratY.push_back(syM);
    if(recv == 'o')
    {
        stratY.push_back(rsyI);
        stratY.push_back(rsyM);
    }
    std::vector<int **> stratU;
    stratU.push_back(suI);
    stratU.push_back(suM);
    if(recv == 'o')
    {
        stratU.push_back(rsuI);
        stratU.push_back(rsuM);
    }
    std::vector<int **> stratV;
    stratV.push_back(svI);
    stratV.push_back(svM);
    if(recv == 'o')
    {
        stratV.push_back(rsvI);
        stratV.push_back(rsvM);
    }
    std::vector<int> h;
    h.push_back(Y->height);
    h.push_back(Y->height/taille);
    if(recv == 'o')
    {
        h.push_back(ry->height);
        h.push_back(recouv->height/taille);
    }
    std::vector<int> w;
    w.push_back(Y->width);
    w.push_back(Y->width/taille);
    if(recv == 'o')
    {
        w.push_back(ry->width);
        w.push_back(recouv->width/taille);
    }
/*
    for(int ha = 0; ha < h[0]; ha++)
        for(int wa = 0; wa < w[0]; wa++)
        {
            cout << wa << ", " << ha << endl;
            cvGet2D(Y, ha, wa);
        }
*/
    //cout << "w : " << Y->width << ", h : " << Y->height << endl;

    saveXML("saveTest.xml", vecY, vecU, vecV , stratY, stratU, stratV, h, w, QP);
    cout<<"Sauvegarde fini ... "<<endl;
}

void decodage(const std::string &filename){
    cout<<"Chargement en cours ...."<<endl;
    //chargement

    IplImage *yi, *ym, *ui, *um, *vi, *vm,
    *ryi, *rym, *rui, *rum, *rvi, *rvm;
    std::vector<double **> vecY;
    std::vector<double **> vecU;
    std::vector<double **> vecV;
    std::vector<int **> stratY;
    std::vector<int **> stratU;
    std::vector<int **> stratV;
    std::vector<int> h;
    std::vector<int> w;
    int QP;

    loadXML(filename.c_str(), vecY, vecU, vecV , stratY, stratU, stratV, h, w, QP);
    cout<<"Chargement fini...."<<endl;
    decodageComposante(vecY[0], vecY[1], stratY[0], stratY[1], QP, yi, ym, w[0], h[0]);
    decodageComposante(vecU[0], vecU[1], stratU[0], stratU[1], QP, ui, um, w[0], h[0]);
    decodageComposante(vecV[0], vecV[1], stratV[0], stratV[1], QP, vi, vm, w[0], h[0]);

    if(vecY.size() > 2)
    {
        //recouvrement
        decodageComposante(vecY[2], vecY[3], stratY[2], stratY[3], QP, ryi, rym, w[2], h[2]);
        decodageComposante(vecU[2], vecU[3], stratU[2], stratU[3], QP, rui, rum, w[2], h[2]);
        decodageComposante(vecV[2], vecV[3], stratV[2], stratV[3], QP, rvi, rvm, w[2], h[2]);
    }
    IplImage *yuvI, *yuvM,*ryuvI, *ryuvM,*rbgrM,*rbgrI;
    yuvM = unifiateComponents(ym, um, vm);
    yuvI = unifiateComponents(yi, ui, vi);
    if(vecY.size() > 2)
    {
        ryuvM = unifiateComponents(rym, rum, rvm);
        ryuvI = unifiateComponents(ryi, rui, rvi);
    }

    //IplImage *bgrM = cvYUV2BGR(yuvM);
    IplImage *bgrI = cvYUV2BGR(yuvI);

    if(vecY.size() > 2)
    {
        rbgrM = cvYUV2BGR(ryuvM);
        rbgrI = cvYUV2BGR(ryuvI);
    }

    //cvShowAnyImageYUV("miniature image decompresse", bgrM);
    //cvShowAnyImageYUV("image decompresse", bgrI);
    if(vecY.size() > 2)
    {
        //cvShowAnyImageYUV("miniature image Recouvrement", rbgrM);
        //cvShowAnyImageYUV("image Recouvrement", rbgrI);

        bgrI = mergedRecouvrement(bgrI, rbgrI, taille);
        //cvShowAnyImageYUV("image fusionné avec recouvrement", bgrI);
    }
    cvShowAnyImageYUV("image decompresse", bgrI);
}

void codageComposante(IplImage *Y, int QP, double **&qI, double **&qM, int **&sI, int **&sM)
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

void decodageComposante(double **qI, double **qM, int **sI, int **sM, int QP, IplImage *&I, IplImage *&M, int w, int h)
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
