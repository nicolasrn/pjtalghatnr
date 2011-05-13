/**
 * \file dct.h
 * \brief contient les fonctions de DCT
 */

#ifndef DCT_H_INCLUDED
#define DCT_H_INCLUDED

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <iostream>
#include "lib.h"
#include "MyImage.hpp"

/**
 *execute la dct
 *@param I : l'image référence : travail sur une copie de l'image et retourne cette copie
 *@param taille : la taille du bloc sur lequel on veut appliquer la DCT
 *@return IplImage* l'image qui a subit la DCT par bloc
 */
IplImage* DCT(IplImage *I, int taille);

/**
 *execute la dect inverse
 *@param I : l'image référence : travail sur une copie de l'image et retourne cette copie
 *@param taille : la taille du bloc sur lequel on veut appliquer la DCT
 *@return IplImage* l'image qui a subit la DCT par bloc
 */
IplImage* InverseDCT(IplImage *I, int taille);

/**
 *execute la dct sur les DC de chaque bloc de taille taille
 *@param I : l'image référence : travail sur une copie de l'image et retourne cette copie
 *@param taille : la taille du bloc sur lequel on veut appliquer la DCT
 *@return IplImage* l'image qui a subit la DCT par bloc
 */
IplImage* ExplodeDCT(IplImage *I,int taille);

/**
 *Réunion le bloc contenant les DC avec le reste de l'image
 *@param LowDC : image contenant pas les DC
 *@param BestDC : image contenant les DC de l'image d'origne
 *@param taille : taille des bloc à traiter
 *@return IplImage : image qui réunis les 2.
 */
IplImage * MergeDCT(IplImage * LowDC, IplImage * BestDC,int taille);

#endif // DCT_H_INCLUDED
