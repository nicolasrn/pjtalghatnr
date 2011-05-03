#ifndef LIBXML_H_INCLUDED
#define LIBXML_H_INCLUDED

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>

#include "MyImage.hpp"
#include "tinyxml.h"

/**
 *@param name: Nom du Fichier qu'on enregistre
 *@param Y: image quantifi� pour Y
 *@param U: image quantifi� pour U
 *@param V : image quantifi� pour V
 *@param stratY : strat�gie pour Y
 *@param stratU : strat�gie pour U
 *@param stratV : strat�gie pour V
 */
void saveXML(const char * name, std::vector<IplImage *> Y, std::vector<IplImage *> U, std::vector<IplImage *> V , std::vector<int **> stratY, std::vector<int **> stratU, std::vector<int **> stratV);

#endif // LIBXML_H_INCLUDED
