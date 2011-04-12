#include "lib.h"

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

void separateComponents( IplImage * YUV )
{
    IplImage * Y = cvCreateImage(cvGetSize(YUV), YUV->depth, 1 );
    IplImage * U = cvCreateImage(cvGetSize(YUV), YUV->depth, 1 );
    IplImage * V = cvCreateImage(cvGetSize(YUV), YUV->depth, 1 );
    CvScalar pxYUV, px1Comp;

    for( int ligne=0; ligne<Y->height; ligne ++ )
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

    cvShowAnyImage( Y, "Y" );
    cvShowAnyImage( U, "U" );
    cvShowAnyImage( V, "V" );

    /*cvNamedWindow("Y", CV_WINDOW_AUTOSIZE);
	cvShowImage("Y", Y);
	cvWaitKey(0);
    cvNamedWindow("U", CV_WINDOW_AUTOSIZE);
	cvShowImage("U", U);
	cvWaitKey(0);
	cvNamedWindow("V", CV_WINDOW_AUTOSIZE);
	cvShowImage("V", V);
	cvWaitKey(0);*/

	cvDestroyWindow("Y");

    DCT3(Y, 4, "DCTY");
    DCT3(U, 4, "DCTU");
    DCT3(V, 4, "DCTV");

    cvReleaseImage( &Y );
    cvReleaseImage( &U );
    cvReleaseImage( &V );
}

IplImage * cvYUV2BGR( IplImage * YUV )
{
    IplImage * BGR = cvCreateImage( cvGetSize(YUV), YUV->depth, 3 );
    CvScalar pxYUV, pxBGR;

    for( int ligne=0; ligne<BGR->height; ligne++ )
        for( int colonne=0; colonne<BGR->width; colonne++ )
        {
            pxYUV = cvGet2D( YUV, ligne, colonne );
            pxBGR.val[1] = pxYUV.val[0] + floor(pxYUV.val[1]/2);
            pxBGR.val[2] = pxBGR.val[1] - pxYUV.val[1] - ceil(pxYUV.val[2]/2);
            pxBGR.val[0] = pxYUV.val[2] + pxBGR.val[2];
            cvSet2D( BGR, ligne, colonne, pxBGR );
        }
    return BGR;
}

