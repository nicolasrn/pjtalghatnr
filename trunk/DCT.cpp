/**
 * \file dct.cpp
 * \brief librairie de codage principale
 */

 #include "DCT.h"

IplImage* DCT(IplImage *I, int taille)
{
    //penser a copier l'image I
    IplImage *A = cvCreateImage(cvSize(I->width, I->height), IPL_DEPTH_64F, 1);
    cvCopy(I, A);

    //cvCvtGray(A, A);

    for(int i = 0; i < A->width; i += taille)
    {
        for(int j = 0; j < A->height; j += taille)
        {
            //cout << "(i, j) : (" << i << ", " << j << ") => (" << taille << ", " << taille << ")" << endl;
            cvSetImageROI(A, cvRect(i, j, taille, taille));
            cvDCT(A, A, CV_DXT_FORWARD);
            cvResetImageROI(A);
        }
    }

    return A;
}

IplImage* InverseDCT(IplImage *I, int taille)
{
    //penser a copier l'image I
    IplImage *A = cvCreateImage(cvSize(I->width, I->height), IPL_DEPTH_64F, 1);
    cvCopy(I, A);

    //cvCvtGray(A, A);

    for(int i = 0; i < A->width; i += taille)
    {
        for(int j = 0; j < A->height; j += taille)
        {
            //cout << "(i, j) : (" << i << ", " << j << ") => (" << taille << ", " << taille << ")" << endl;
            cvSetImageROI(A, cvRect(i, j, taille, taille));
            cvDCT(A, A, CV_DXT_INVERSE);
            cvResetImageROI(A);
        }
    }

    return A;
}

IplImage* ExplodeDCT (IplImage *I,int taille)
{
    // création de l'image qui contiendra les best DC
    IplImage * DCTbloc  = cvCreateImage(cvSize(I->width/taille,I->height/taille),IPL_DEPTH_64F,1);

    double* dataI = (double*) I->imageData;
    double* dataDCTbloc = (double*) DCTbloc->imageData;

    //on rempli le petit bloc avec les DC de l'image
    for( int i = 0; i< I->height; i+=taille)
        for( int j = 0 ; j < I->width; j+=taille)
            {
                dataDCTbloc[i/taille*I->width/taille+j/taille] = dataI[i*I->width+j];
                dataI[i*I->width+j] = 0;
            }


     return DCTbloc;
}
IplImage * MergeDCT(IplImage * LowDC, IplImage * BestDC,int taille){

    double* dataLowDC= (double*) LowDC->imageData;
    double* dataBestDC = (double*) BestDC->imageData;

    for( int i = 0; i< LowDC->height; i+=taille)
        for( int j = 0 ; j < LowDC->width; j+=taille)
            {

                dataLowDC[i*LowDC->width+j] = dataBestDC[i/taille*LowDC->width/taille+j/taille];
            }
    return LowDC;
}
