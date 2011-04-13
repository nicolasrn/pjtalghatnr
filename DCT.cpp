#include "DCT.h"

IplImage* DCT3(IplImage *I, int taille)
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
