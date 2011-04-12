#ifndef DCT_H_INCLUDED
#define DCT_H_INCLUDED

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <iostream>
#include "lib.h"
#include "MyImage.hpp"

/**
 *@param I : l'image référence : travail sur une copie de l'image et retourne cette copie
 *@param taille : la taille du bloc sur lequel on veut appliquer la DCT
 *@return IplImage* l'image qui a subit la DCT par bloc
 */
IplImage* DCT3(IplImage *I, int taille);

#endif // DCT_H_INCLUDED
