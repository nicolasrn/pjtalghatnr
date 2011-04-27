#ifndef LIBXML_H_INCLUDED
#define LIBXML_H_INCLUDED

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "MyImage.hpp"

/**
 *@param Original_Y: image de r�f�rence pour la Y
 *@param Original_U: image de r�f�rence pour la
 *@param Original_V :
 *@param Ipred : l'image sur laquelle se portera la pr�diction
 *@param taille : taille des zones des pr�dictions
 *@param strategie : tableau � deux dimensions repr�sentant la strat�gie � adapter
 */
void saveXMLOriginal(IplImage *&Y, IplImage *&U, IplImage *&V , int **&stratY, int **&stratU, int **&stratV);

#endif // LIBXML_H_INCLUDED
