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
 *sépare les différentes couche YUV de l'image
 *@param YUV : l'image YUV
 *@param Y : l'image Y retourne
 *@param u : l'image U retourne
 *@param V : l'image V retourne
 */
void separateComponents( IplImage * YUV, IplImage *&Y, IplImage *&U, IplImage *&V );

/**
 *fusionne les différentes couche YUV dans une image
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
 *@param I : l'image sur laquelle se portera la prédiction
 *@param taille : taille des zone des prédictions
 *@return IplImage : l'image en prédite
 */
 IplImage * predictImage(IplImage * I, int taille);

/**
 *@param ligne : indice de la ligne de la zone à prédire
 *@param colonne : indice de la colonne de la zone à prédire
 *@param I : image de référence pour la prédiction
 *@param Ipred : l'image sur laquelle se portera la prédiction
 *@param taille : taille des zones des prédictions
 */
void predictZone(int ligne, int colonne, IplImage * I, IplImage *&Ipred, int taille);

/**
 *@param ligne : indice de la ligne du pixel à prédire
 *@param colonne : indice de la colonne du pixel à prédire
 *@param I : image de référence pour la prédiction
 *@param Ipred : l'image sur laquelle se portera la prédiction
 *@param taille : taille des zones des prédictions
 */
void predictPixel(int ligne, int colonne, IplImage * I, IplImage *&Ipred, int taille);

/**
 *renvoie le multiple de 16 le plus proche de nb
 *@param nb : le nombre qu'il faut ajuster
 */
int getMultiple16(int nb);

/**
 *ajuste l'image
 *@param image : l'image à ajuster
 *@return IplImage : l'image dont les dimensions sont multiples de 16
 */
IplImage * ajustementImage(IplImage *image);

#endif // LIB_H_INCLUDED
