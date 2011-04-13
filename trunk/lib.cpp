#include "lib.h"

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
