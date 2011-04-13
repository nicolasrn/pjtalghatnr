#include <iostream>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "lib.h"

using namespace std;

int main()
{
    IplImage * BGR, *image = cvOpenImage( "fraise.jpg" );
    BGR = ajustementImage(image);
    cvShowAnyImageYUV("BGR", BGR);

    IplImage * YUV = cvBGR2YUV( BGR );
    cvShowAnyImageYUV("YUV", YUV);

    IplImage *Y, *U, *V;
    separateComponents( YUV, Y, U, V );

    IplImage *a, *b, *c, *Ba,*Bb, *Bc;
    a = DCT3(Y, 4);
    b = DCT3(U, 4);
    c = DCT3(V, 4);

    Ba = DCTDC(Y,4);
    Bb = DCTDC(U,4);
    Bc = DCTDC(V,4);



    IplImage * BGR_back = cvYUV2BGR( YUV );

    IplImage *ia, *ib, *ic;
    ia = InverseDCT(a, 4);
    ib = InverseDCT(b, 4);
    ic = InverseDCT(c, 4);

    IplImage *reconst = unifiateComponents(ia, ib, ic);
    IplImage *reconstBGR = cvYUV2BGR(reconst);


    cvShowImage("DCTY",a);
    cvShowImage("DCTU",b);
    cvShowImage("DCTV",c);
    cvShowImage("DCDCTa",Ba);
    cvShowImage("DCDCTb",Bb);
    cvShowImage("DCDCTc",Bc);
    cvShowImage("DCTYmodif",a);
    cvShowImage("DCTUmodif",b);
    cvShowImage("DCTVmodif",c);

    cvShowAnyImageYUV("Y", Y );
    cvShowAnyImageYUV("U", U );
    cvShowAnyImageYUV("V", V );


    cvShowAnyImage( BGR_back, "BGR reconstruite" );

    cvShowAnyImageYUV("DCTY", a);
    cvShowAnyImageYUV("DCTU", b);
    cvShowAnyImageYUV("DCTV", c);

    cvShowAnyImageYUV("IA", ia);
    cvShowAnyImageYUV("IB", ib);
    cvShowAnyImageYUV("IC", ic);

    cvShowAnyImageYUV("Unifier", reconst);
    cvShowAnyImageYUV("UnifierBGR", reconstBGR);

    IplImage *res = cvCreateImage(cvGetSize(BGR), IPL_DEPTH_64F, 3);
    cvSub(BGR, reconstBGR, res);

    cvShowAnyImageYUV("sub", res);

    cvDestroyWindow("Y");
    cvDestroyWindow("U");
    cvDestroyWindow("V");

    cvDestroyWindow("Unifier");
    cvDestroyWindow("UnifierBGR");

    cvDestroyWindow("DCTY");
    cvDestroyWindow("DCTU");
    cvDestroyWindow("DCTV");

    cvDestroyWindow("IA");
    cvDestroyWindow("IB");
    cvDestroyWindow("IC");

    cvDestroyWindow("sub");

    cvReleaseImage( &Y );
    cvReleaseImage( &U );
    cvReleaseImage( &V );

    cvReleaseImage( &a );
    cvReleaseImage( &b );
    cvReleaseImage( &c );

    cvReleaseImage( &ia );
    cvReleaseImage( &ib );
    cvReleaseImage( &ic );

    cvReleaseImage( &res );

    cvReleaseImage(&reconst);
    cvReleaseImage(&reconstBGR);

    return EXIT_SUCCESS;

    /*IplImage *image = cvOpenImage( "fraise.jpg" );
    IplImage *bis = ajustementImage(image);

    cvNamedWindow("image", CV_WINDOW_AUTOSIZE);
	cvShowImage("image", image);

    cvNamedWindow("bis", CV_WINDOW_AUTOSIZE);
	cvShowImage("bis", bis);

	cvWaitKey(0);

	cvDestroyWindow("image");
	cvDestroyWindow("bis");

	cvReleaseImage(&image);
	cvReleaseImage(&bis);*/

    return EXIT_SUCCESS;
}
