#include <iostream>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "lib.h"

using namespace std;

//enum QP {faible = 1, moyen = 2, haut = 3, eleve = 4};
struct valQP
{
    static const int faible = 4, moyen = 3, haut = 2, eleve = 1;
};

struct valR
{
    static const int grandeImage = 1, petiteImage = 0;
};

int Resolution[2] = {2, 0};

int main()
{
    IplImage * BGR, *image = cvLoadImage( "mandril.jpg" );
    BGR = ajustementImage(image);

    IplImage * YUV = cvBGR2YUV( BGR );
    IplImage *Y, *U, *V, *composantATraiter;
    separateComponents(YUV, Y, U, V);
    composantATraiter = Y;
    cvShowAnyImageYUV("Y", composantATraiter);

    IplImage * dct = DCT3(composantATraiter, 4);
    cvShowAnyImageYUV("DCT Y", dct);

    int **strategie;
    IplImage *pred = predictImage(dct, 4, strategie);
    cvShowAnyImageYUV("pred", pred);

    //int ** q = applyQuantification(pred, valQP::faible, Resolution, valR::grandeImage);
    //for(int i = 0; i < pred->width; i++)
    //    for(int j = 0; j < pred->height; j++)
    //        std::cout << "(" << i << ", " << j << ") : " << q[i][j] << std::endl;
    IplImage *reverse = NULL;
    //IplImage *reverse = ReverseApplyQuantification(q, valQP::faible, Resolution, valR::grandeImage, pred->width, pred->height);
    reverse = ReversepredictImage(pred, 4, strategie);
    CvScalar a, b;
    int nbErreur = 0;
    for(int i = 4; i < 8; i++)
    {
        for(int j = 4; j < 8; j++)
        {
            a = cvGet2D(pred, i, j);
            b = cvGet2D(reverse, i, j);
            if (a.val[0] != b.val[0])
            {
                nbErreur++;
                cout << "res : (" << i << ", " << j << ") : " << a.val[0] << " ? " << b.val[0] << " strategie : " << strategie[j][i] << endl;
            }
        }
    }
    cout << "nbErreur = " << nbErreur << endl;
    reverse = InverseDCT(reverse , 4);
    cvShowAnyImageYUV("reverse", reverse);

    cvDestroyWindow("DCT Y");
    cvDestroyWindow("pred");
    cvDestroyWindow("reverse");

    cvReleaseImage(&BGR);
    cvReleaseImage(&image);
    cvReleaseImage(&YUV);
    cvReleaseImage(&Y);
    cvReleaseImage(&U);
    cvReleaseImage(&V);
    cvReleaseImage(&dct);
    cvReleaseImage(&pred);
    cvReleaseImage(&reverse);

    return EXIT_SUCCESS;
}
