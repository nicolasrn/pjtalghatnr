#ifndef LIB_H_INCLUDED
#define LIB_H_INCLUDED

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "DCT.h"
#include "MyImage.hpp"

void cvShowAnyImageYUV(string s, IplImage * I, int x = 800, int y = 200 );

void cvShowAnyImage(string s, IplImage *I, int x = 800, int y = 200);

/**
 *converti en YUV
 *@param BGR : l'image normal
 *@return IplImage : l'image en YUV
 */
IplImage * cvBGR2YUV( IplImage * BGR );

/**
 *s�pare les diff�rentes couche YUV de l'image
 *@param YUV : l'image YUV
 *@param Y : l'image Y retourne
 *@param u : l'image U retourne
 *@param V : l'image V retourne
 */
void separateComponents( IplImage * YUV, IplImage *&Y, IplImage *&U, IplImage *&V );

/**
 *fusionne les diff�rentes couche YUV dans une image
 *@param YUV : l'image YUV
 *@param Y : l'image Y retourne
 *@param u : l'image U retourne
 *@param V : l'image V retourne
 */
 IplImage* unifiateComponents(IplImage *Y, IplImage *U, IplImage *V);

/**
 *converti en BGR
 *@param BGR : l'image YUV
 *@return IplImage : l'image en BGR
 */
IplImage * cvYUV2BGR( IplImage * YUV );

/**
 *renvoie le multiple de 16 le plus proche de nb
 *@param nb : le nombre qu'il faut ajuster
 */
int getMultiple16(int nb);

/**
 *ajuste l'image
 *@param image : l'image � ajuster
 *@return IplImage : l'image dont les dimensions sont multiples de 16
 */
IplImage * ajustementImage(IplImage *image);

#endif // LIB_H_INCLUDED
