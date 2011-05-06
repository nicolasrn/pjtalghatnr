#ifndef LIBXML_H_INCLUDED
#define LIBXML_H_INCLUDED

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>

#include "MyImage.hpp"
#include "tinyxml.h"

using namespace std;

/**
 *@param name: Nom du Fichier qu'on enregistre
 *@param Y: quantification pour Y
 *@param U: quantification pour U
 *@param V : quantification pour V
 *@param stratY : strat�gie pour Y
 *@param stratU : strat�gie pour U
 *@param stratV : strat�gie pour V
 *@param heightVector : hauteur pour chaque partie enregistr�e
 *@param widthVector : largeur pour chaque partie enregistr�e
 */
void saveXML(const char * name, std::vector<int **> Y, std::vector<int **> U, std::vector<int **> V , std::vector<int **> stratY, std::vector<int **> stratU, std::vector<int **> stratV, std::vector<int> heightVector, std::vector<int> widthVector);

/**
 *@param name: Nom du Fichier qu'on charge
 *@param Y: quantification pour Y
 *@param U: quantification pour U
 *@param V : quantification pour V
 *@param stratY : strat�gie pour Y
 *@param stratU : strat�gie pour U
 *@param stratV : strat�gie pour V
 *@param heightVector : hauteur pour chaque partie enregistr�e
 *@param widthVector : largeur pour chaque partie enregistr�e
 */
void loadXML(const char* name, std::vector<int **> &Y, std::vector<int **> &U, std::vector<int **> &V , std::vector<int **> &stratY, std::vector<int **> &stratU, std::vector<int **> &stratV, std::vector<int> &heightVector, std::vector<int> &widthVector);

#endif // LIBXML_H_INCLUDED
