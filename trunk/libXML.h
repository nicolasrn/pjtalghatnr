#ifndef LIBXML_H_INCLUDED
#define LIBXML_H_INCLUDED

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "MyImage.hpp"

/**
 *@param Original_Y: image de référence pour la Y
 *@param Original_U: image de référence pour la
 *@param Original_V :
 *@param Ipred : l'image sur laquelle se portera la prédiction
 *@param taille : taille des zones des prédictions
 *@param strategie : tableau à deux dimensions représentant la stratégie à adapter
 */
void saveXMLOriginal(IplImage *&Y, IplImage *&U, IplImage *&V , int **&stratY, int **&stratU, int **&stratV);

#endif // LIBXML_H_INCLUDED
