#ifndef LIB_H_INCLUDED
#define LIB_H_INCLUDED

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "DCT.h"
#include "MyImage.hpp"

IplImage * cvBGR2YUV( IplImage * BGR );

void separateComponents( IplImage * YUV, IplImage *&Y, IplImage *&U, IplImage *&V );

IplImage * cvYUV2BGR( IplImage * YUV );

#endif // LIB_H_INCLUDED
