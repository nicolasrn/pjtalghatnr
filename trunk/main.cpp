#include <iostream>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "lib.h"

using namespace std;

int main()
{
    IplImage * BGR = cvOpenImage( "C:\\Users\\Nicolas\\Desktop\\Test\\bin\\Debug\\mandril.jpg" );
    cvShowAnyImage( BGR, "BGR" );

    IplImage * YUV = cvBGR2YUV( BGR );
    cvShowAnyImage( YUV, "YUV" );

    IplImage *Y, *U, *V;
    separateComponents( YUV, Y, U, V );

    IplImage *a, *b, *c;
    a = DCT3(Y, 16);
    b = DCT3(U, 32);
    c = DCT3(V, 64);

    IplImage * BGR_back = cvYUV2BGR( YUV );

    cvShowAnyImage( Y, "Y" );
    cvShowAnyImage( U, "U" );
    cvShowAnyImage( V, "V" );

    cvShowAnyImage(a, "DCTY");
    cvShowAnyImage(b, "DCTU");
    cvShowAnyImage(c, "DCTV");

    cvShowAnyImage( BGR_back, "BGR reconstruite" );

	cvDestroyWindow("Y");
	cvDestroyWindow("U");
	cvDestroyWindow("V");

    cvDestroyWindow("DCTY");
	cvDestroyWindow("DCTU");
	cvDestroyWindow("DCTV");

	cvDestroyWindow("BGR reconstruite");

	cvReleaseImage( &Y );
    cvReleaseImage( &U );
    cvReleaseImage( &V );

	cvReleaseImage( &a );
    cvReleaseImage( &b );
    cvReleaseImage( &c );

    cvReleaseImage(&BGR_back);

    return 0;
}
