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

int R[2] = {2, 0};

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
            cout << strategie[i][j] << " ";
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

int main()
{
    IplImage * BGR, *image = cvLoadImage( "mandril.jpg" );
    BGR = ajustementImage(image);

    IplImage * YUV = cvBGR2YUV( BGR );
    IplImage *Y, *U, *V, *composantATraiter;
    separateComponents(YUV, Y, U, V);
    composantATraiter = Y;
    cvShowAnyImageYUV("Y", composantATraiter);

    IplImage * dct = DCT(composantATraiter, 4);
    //cvShowAnyImageYUV("DCT Y", dct);

    int **strategie;
    IplImage *pred = predictImage(dct, 4, strategie);
    //cvShowAnyImageYUV("pred", pred);

    int **q = applyQuantification(pred, valQP::eleve, R, valR::grandeImage);

    // IplImage * ReverseApplyQuantification(int ** q, int QP, int* R, int i, int width, int heigh);
    IplImage *reverse = ReverseApplyQuantification(q, valQP::eleve, R, valR::grandeImage, pred->width, pred->height);

    reverse = ReversepredictImage(reverse, 4, strategie);
    reverse = InverseDCT(reverse, 4);
    cvShowAnyImageYUV("reverse", reverse);

    cvDestroyWindow("Y");
    //cvDestroyWindow("DCT Y");
    //cvDestroyWindow("pred");
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
