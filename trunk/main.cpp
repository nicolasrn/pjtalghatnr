#include <iostream>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "lib.h"

using namespace std;

int main()
{
    printf("Hello world!\n");
    IplImage * BGR = cvOpenImage( "C:\\Users\\Nicolas\\Desktop\\Test\\bin\\Debug\\mandril.jpg" );
    cvShowAnyImage( BGR, "BGR" );

    IplImage * YUV = cvBGR2YUV( BGR );
    cvShowAnyImage( YUV, "YUV" );

    separateComponents( YUV );

    IplImage * BGR_back = cvYUV2BGR( YUV );
    cvShowAnyImage( BGR_back, "BGR reconstruite" );

    return 0;
}
