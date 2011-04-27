#include "lib.h"
#include "math.h"

void cvShowAnyImageYUV(string s, IplImage * I, int x, int y)
{
    cvShowAnyImage(I, s, x, y);
}

void cvShowAnyImage(string s, IplImage *I, int x, int y)
{
    // create a window
	cvNamedWindow(s.c_str(), CV_WINDOW_AUTOSIZE);
	cvMoveWindow(s.c_str(), x, y);

	// show image
	cvShowImage (s.c_str (), I);
	cvWaitKey(0);
}

IplImage * cvBGR2YUV( IplImage * BGR )
{
    IplImage * YUV = cvCreateImage( cvGetSize(BGR), IPL_DEPTH_64F, 3 );
    CvScalar pxBGR, pxYUV;

    // conversion
    for( int ligne=0; ligne<BGR->height; ligne++ ) // pour chaque ligne
        for( int colonne=0; colonne<BGR->width; colonne++ ) // pour chaque colonne
        {
            // attention ici on a trois valeurs (BGR) par pixel
            pxBGR = cvGet2D( BGR, ligne, colonne );
            pxYUV.val[2] = pxBGR.val[0] - pxBGR.val[2]; // V
            pxYUV.val[1] = pxBGR.val[1] - pxBGR.val[2] - ceil(pxYUV.val[2]/2 ); // U
            pxYUV.val[0] = pxBGR.val[1] - floor(pxYUV.val[1]/2); // Y
            cvSet2D( YUV, ligne, colonne, pxYUV );
        }
    return YUV;
}

void separateComponents( IplImage * YUV, IplImage *&Y, IplImage *&U, IplImage *&V )
{
    Y = cvCreateImage(cvGetSize(YUV), YUV->depth, 1 );
    U = cvCreateImage(cvGetSize(YUV), YUV->depth, 1 );
    V = cvCreateImage(cvGetSize(YUV), YUV->depth, 1 );
    CvScalar pxYUV, px1Comp;

    for( int ligne=0; ligne<Y->height; ligne ++ )
    {
        for( int colonne=0; colonne<Y->width; colonne++ )
        {
            pxYUV = cvGet2D( YUV, ligne, colonne );
            px1Comp.val[0] = pxYUV.val[0];
            cvSet2D( Y, ligne, colonne, px1Comp );
            px1Comp.val[0] = pxYUV.val[1];
            cvSet2D( U, ligne, colonne, px1Comp );
            px1Comp.val[0] = pxYUV.val[2];
            cvSet2D( V, ligne, colonne, px1Comp );
        }
    }
}

IplImage* unifiateComponents(IplImage *Y, IplImage *U, IplImage *V)
{
    IplImage *YUV = cvCreateImage(cvGetSize(Y), IPL_DEPTH_64F, 3 );

    CvScalar px, pximg;

    for( int ligne=0; ligne<Y->height; ligne ++ )
    {
        for( int colonne=0; colonne<Y->width; colonne++ )
        {
            pximg = cvGet2D(YUV, ligne, colonne);
            px = cvGet2D(Y, ligne, colonne);
            pximg.val[0] = px.val[0];
            px = cvGet2D(U, ligne, colonne);
            pximg.val[1] = px.val[0];
            px = cvGet2D(V, ligne, colonne);
            pximg.val[2] = px.val[0];

            cvSet2D(YUV, ligne, colonne, pximg);
        }
    }

    return YUV;
}

IplImage * cvYUV2BGR( IplImage * YUV )
{
    IplImage * BGR = cvCreateImage( cvGetSize(YUV), YUV->depth, 3 );
    CvScalar pxYUV, pxBGR;

    for( int ligne=0; ligne<BGR->height; ligne++ )
    {
        for( int colonne=0; colonne<BGR->width; colonne++ )
        {
            pxYUV = cvGet2D( YUV, ligne, colonne );
            pxBGR.val[1] = pxYUV.val[0] + floor(pxYUV.val[1]/2);
            pxBGR.val[2] = pxBGR.val[1] - pxYUV.val[1] - ceil(pxYUV.val[2]/2);
            pxBGR.val[0] = pxYUV.val[2] + pxBGR.val[2];
            cvSet2D( BGR, ligne, colonne, pxBGR );
        }
    }
    return BGR;
}

int getMultiple16(int nb)
{
    int mul = 0;

    while((nb+mul)%16 != 0)
          mul++;

    return mul + nb;
}

IplImage * ajustementImage(IplImage *image)
{
    int widthAjuste = 0, heightAjuste = 0;

    if (image->width % 16 != 0) //ajustement de la width
        widthAjuste = getMultiple16(image->width);

    if (image->height % 16 != 0) //ajustement de la height
        heightAjuste = getMultiple16(image->height);

    //creation de l'image ajustée
    IplImage *A = cvCreateImage(cvSize(widthAjuste, heightAjuste), IPL_DEPTH_64F, image->nChannels);
    CvScalar px;

    //remplissage avec du noir de l'image
    for(int i = 0; i < image->height; i++)
    {
        for(int j = 0; j < image->width; j++)
        {
            px = cvGet2D(image, i, j);
            cvSet2D(A, i, j, px);
        }
    }

    return A;
}


void quantification(IplImage * I, int QP, int* R,int i,int ** q,int c,int l){

    CvScalar px;

//Remplissage manuel de x en triant selon le modèle
px = cvGet2D( I, 0, 0);
q[l][c] = floor(px.val[0]/(pow(2, QP+R[i])));

px = cvGet2D( I, 0, 1);
q[l][c+1] = floor(px.val[0]/(pow(2, QP+R[i])));

px = cvGet2D( I, 1, 0);
q[l+1][c] = floor(px.val[0]/(pow(2, QP+R[i])));

px = cvGet2D( I, 0, 2);
q[l][c+2] = floor(px.val[0]/(pow(2, QP+R[i])));

px = cvGet2D( I, 2, 0);
q[l+2][c] = floor(px.val[0]/(pow(2, QP+R[i])));

px = cvGet2D( I, 0, 3);
q[l][c+3] = floor(px.val[0]/(pow(2, QP+R[i])));

px = cvGet2D( I, 3, 0);
q[l+3][c] = floor(px.val[0]/(pow(2, QP+R[i])));

px = cvGet2D( I, 1, 1);
q[l+1][c+1] = floor(px.val[0]/(pow(2, QP+R[i])));

px = cvGet2D( I, 1, 2);
q[l+1][c+2] = floor(px.val[0]/(pow(2, QP+R[i])));

px = cvGet2D( I, 2, 1);
q[l+2][c+1] = floor(px.val[0]/(pow(2, QP+R[i])));

px = cvGet2D( I, 3, 1);
q[l+3][c+1] = floor(px.val[0]/(pow(2, QP+R[i])));

px = cvGet2D( I, 2, 2);
q[l+2][c+2] = floor(px.val[0]/(pow(2, QP+R[i])));

px = cvGet2D( I, 1, 3);
q[l+1][c+3] = floor(px.val[0]/(pow(2, QP+R[i])));

px = cvGet2D( I, 2, 3);
q[l+2][c+3] = floor(px.val[0]/(pow(2, QP+R[i])));

px = cvGet2D( I, 3, 2);
q[l+3][c+2] = floor(px.val[0]/(pow(2, QP+R[i])));

px = cvGet2D( I, 3, 3);
q[l+3][c+3] = floor(px.val[0]/(pow(2, QP+R[i])));

    return q;
}


int **applyQuantification( IplImage *I, int QP, int* R, int i){


    int **tab = (int **)malloc (sizeof(int*)* I->width * I->height);

    //penser a copier l'image I
    IplImage *A = cvCreateImage(cvSize(I->width, I->height), IPL_DEPTH_64F, 1);
    cvCopy(I, A);

    for(int l = 0; l < A->width; l += 4)
    {
        for(int j = 0; j < A->height; j += 4)
        {
            cvSetImageROI(A, cvRect(l, j, 4, 4));
            quantification(A, QP, R, i,tab,l,j);
            cvResetImageROI(A);
        }
    }

    return tab;
}


IplImage * ReverseApplyQuantification(int ** q, int QP, int* R, int i, int width, int heigh)
{
    IplImage * I = cvCreateImage(cvGetSize(YUV), YUV->depth, 1 );

    for(int l = 0; l < I->width; l += 4)
    {
        for(int j = 0; j < I->height; j += 4)
        {
         cvSetImageROI(I, cvRect(l, j, 4, 4));
            ReverseQuantification (I,QB,R,i,q,l,j);
         cvResetImageROI(I) ;
        }
    }

}

void ReverseQuantification (IplImage * I, int QB, int* R, int i, int ** q, int col, int ligne){
    CvScalar px;

//Remplissage manuel de x en triant selon le modèle

px = cvGet2D( I, 0, 0);
q[l][c] = floor(px.val[0]/(pow(2, QP+R[i])));


px = cvGet2D( I, 0, 1);
q[l][c+1] = floor(px.val[0]/(pow(2, QP+R[i])));

px = cvGet2D( I, 1, 0);
q[l+1][c] = floor(px.val[0]/(pow(2, QP+R[i])));

px = cvGet2D( I, 0, 2);
q[l][c+2] = floor(px.val[0]/(pow(2, QP+R[i])));

px = cvGet2D( I, 2, 0);
q[l+2][c] = floor(px.val[0]/(pow(2, QP+R[i])));

px = cvGet2D( I, 0, 3);
q[l][c+3] = floor(px.val[0]/(pow(2, QP+R[i])));

px = cvGet2D( I, 3, 0);
q[l+3][c] = floor(px.val[0]/(pow(2, QP+R[i])));

px = cvGet2D( I, 1, 1);
q[l+1][c+1] = floor(px.val[0]/(pow(2, QP+R[i])));

px = cvGet2D( I, 1, 2);
q[l+1][c+2] = floor(px.val[0]/(pow(2, QP+R[i])));

px = cvGet2D( I, 2, 1);
q[l+2][c+1] = floor(px.val[0]/(pow(2, QP+R[i])));

px = cvGet2D( I, 3, 1);
q[l+3][c+1] = floor(px.val[0]/(pow(2, QP+R[i])));

px = cvGet2D( I, 2, 2);
q[l+2][c+2] = floor(px.val[0]/(pow(2, QP+R[i])));

px = cvGet2D( I, 1, 3);
q[l+1][c+3] = floor(px.val[0]/(pow(2, QP+R[i])));

px = cvGet2D( I, 2, 3);
q[l+2][c+3] = floor(px.val[0]/(pow(2, QP+R[i])));

px = cvGet2D( I, 3, 2);
q[l+3][c+2] = floor(px.val[0]/(pow(2, QP+R[i])));

px = cvGet2D( I, 3, 3);
q[l+3][c+3] = floor(px.val[0]/(pow(2, QP+R[i])));


}
 IplImage * predictImage(IplImage * I, int taille){


    //On copie l'image à prédire dans l'image prédite
    IplImage * Ipred = cvCreateImage(cvSize(I->width, I->height), IPL_DEPTH_64F, 1);
    cvCopy(I, Ipred);

    //On parcourt les zones de 4x4
    for(int i = 0; i < Ipred->height; i += taille)
    {
        for(int j = 0; j < Ipred->width; j += taille)
        {
            predictZone(i, j, I, Ipred, taille);
        }
    }

    return Ipred;
 }

void predictZone(int ligne, int colonne, IplImage * I, IplImage *&Ipred, int taille)
{

    //prediction du pixel DC de l'image
    predictPixel(ligne, colonne, I, Ipred, taille);

    //On fait la prédiction sur la ligne haute des AD
    for(int k = 1; k < taille; k++)
    {
        predictPixel(ligne, colonne + k, I, Ipred, taille);
    }

    //On fait la prédiction sur la ligne gauche des AD
    for(int k = 1; k < taille; k++)
    {
        predictPixel(ligne + k, colonne, I, Ipred, taille);
    }
}

void predictPixel(int ligne, int colonne, IplImage * I, IplImage *&Ipred, int taille)
{
    //Pixel pour l'image a prédire, et pour l'image prédite
    CvScalar pxITop, pxILeft, pxI, pxIpred, pxBest;

    //On récupère la valeur du pixel à prédire
    pxI = cvGet2D( I, ligne, colonne );

    //On vérifie qu'on n'est pas sur une zone de bord haut
    if(ligne == 0)
        pxITop = cvGet2D( I, ligne - taille, colonne);

    else
        pxITop.val[0] = pxI.val[0];

    //On vérifie qu'on n'est pas sur une zone de bord gauche
    if(colonne == 0)
        pxILeft = cvGet2D( I, ligne, colonne - taille);

    else
        pxILeft.val[0] = pxI.val[0];

    //On recherche la meilleur stratégie pour le pixel
    pxBest.val[0] = (pxI.val[0] - pxITop.val[0] < pxI.val[0] - pxILeft.val[0]) ? pxI.val[0] - pxITop.val[0] : pxI.val[0] - pxILeft.val[0];

    //On affecte le résultat de la stratégie
    pxIpred.val[0] = pxI.val[0] - pxBest.val[0];
    cvSet2D( Ipred, ligne, colonne, pxIpred);
}
