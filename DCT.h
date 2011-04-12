#ifndef DCT_H_INCLUDED
#define DCT_H_INCLUDED

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <iostream>
#include "lib.h"
#include "MyImage.hpp"

IplImage* DCT3(IplImage *I, int taille, std::string id);

#endif // DCT_H_INCLUDED
