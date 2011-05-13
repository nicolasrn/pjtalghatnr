/**
 * \file lib.h
 * \brief librairie de codage principale
 */

#ifndef LIB_H_INCLUDED
#define LIB_H_INCLUDED

#include <opencv/cv.h>
#include <opencv/highgui.h>


#include "DCT.h"
#include "MyImage.hpp"

/**
 *\brief Affiche une image en YUV ( fonction de Mr Paris v2)
 *@brief Affiche une image en YUV ( fonction de Mr Paris v2)
 *@param s : nom de la fênetre
 *@param I : Image a afficher
 *@param x : position d'affichage en x de la fenêtre
 *@param y : position d'affichage en y de la fenêtre
 */
void cvShowAnyImageYUV(string s, IplImage * I, int x = 800, int y = 200 );

/**
 *Affiche une image en BGR ( fonction de Mr Paris v1)
 *@param s : nom de la fênetre
 *@param I : Image a afficher
 *@param x : position d'affichage en x de la fenêtre
 *@param y : position d'affichage en y de la fenêtre
 */

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
 *renvoie le multiple de 16 le plus proche de nb
 *@param nb : le nombre qu'il faut ajuster
 */
int getMultiple16(int nb);

/**
 *ajuste l'image
 *@param image : l'image à ajuster
 *@return IplImage : l'image dont les dimensions sont multiples de 16
 */
IplImage* ajustementImage(IplImage *image);

/**
 *recouvre l'image
 *@param image : l'image d'origine
 *@param taile : represente la taille a rogner sur les cotés. taille/2 pixels sont rogné à gauche, à droite, en haut, en bas
 *@return IplImage : le resultat recouvert
 */
IplImage* recouvrement(IplImage *image, int taille);

/**
 *fusionne l'image avec le recouvrement
 *@param i : l'image d'origine
 *@param r : l'image de recouvrement
 *@param taile : represente la taille de recouvrement des blocs
 *@return IplImage : le resultat de la fusion
 */
IplImage* mergedRecouvrement(IplImage *i, IplImage *r, int taille);

/**
 *Applique la quatification sur l'image
 *@param I : L'image
 *@param QB : parametre de qualité
 *@param R : pas de quatification
 *@param q : tableau des valeurs quantifiées
 *@param c : nombre de colonnes du tableau
 *@param l : nombre de lignes du tableau
 */
void quantification(IplImage * I, int QP, int* R,int i,int ** q,int c,int l);

/**
 *Applique la quatification sur l'image
 *@param I : L'image
 *@param QB : parametre de qualité
 *@param R : pas de quatification
 *@return int** : retourne un tableau des valeurs quatifiées
 */
int **applyQuantification( IplImage *I, int QP, int* R, int i);

/**
 *Déquantifie les valeurs pour reconstruire l'image
 *@param q : tableau des valeurs a déquantifier
 *@param QB : parametre de qualité
 *@param R : pas de quatification
 *@param width : largeur du tableau
 *@param heigh : hauteur du tableau
 */
 IplImage * ReverseApplyQuantification(int ** q, int QP, int* R, int i, int width, int heigh);

/**
 *applique la quatification inverse sur une zone de l'image
 *@param I : l'image sur laquelle porte la quantification inverse
 *@param QB : parametre de qualité
 *@param R : pas de quatification
 *@param q : suite de valeur a déquatifier
 *@param col : numéro de la colone pour la zone de l'image à traiter
 *@param ligne : numéro de la ligne pour la zone de l'image à traiter
 */
void ReverseQuantification (IplImage * I, int QB, int* R, int i, int ** q, int col, int ligne);

/**
 *@param strategie : tableau contenant les stratégies
 *@param width : largeur du tableau
 *@return height : hauteur en prédite
 */
void intializeStrategie(int** &strategie, int width, int height);

/**
 *@param I : l'image sur laquelle se portera la prédiction
 *@param taille : taille des zone des prédictions
 *@return IplImage : l'image en prédite
 */
 IplImage * predictImage(IplImage * I, int taille, int** &strategie);

/**
 *@param ligne : indice de la ligne de la zone à prédire
 *@param colonne : indice de la colonne de la zone à prédire
 *@param I : image de référence pour la prédiction
 *@param Ipred : l'image sur laquelle se portera la prédiction
 *@param taille : taille des zones des prédictions
 */
void predictZone(int ligne, int colonne, IplImage * I, IplImage *&Ipred, int taille, int** &strategie);

/**
 *@param ligne : indice de la ligne du pixel à prédire
 *@param colonne : indice de la colonne du pixel à prédire
 *@param I : image de référence pour la prédiction
 *@param Ipred : l'image sur laquelle se portera la prédiction
 *@param taille : taille des zones des prédictions
 */
void predictPixel(int ligne, int colonne, IplImage * I, IplImage *&Ipred, int taille, int** &strategie);

/**
 *@param ligne : indice de la ligne du pixel à prédire
 *@param colonne : indice de la colonne du pixel à prédire
 *@param I : image de référence pour la prédiction
 *@param Ipred : l'image sur laquelle se portera la prédiction
 *@param taille : taille des zones des prédictions
 *@param strategie : tableau des stratégies
 *@param stratDC : Stratégie utilisée pour le DC du bloc courant
 */
void predictPixelAD(int ligne, int colonne, IplImage * I, IplImage *&Ipred, int taille, int** &strategie, int stratDC);

/**
 *@param I : l'image sur laquelle se portera la prédiction
 *@param taille : taille des zone des prédictions
 *@return IplImage : l'image en prédite
 */
 IplImage * ReversepredictImage(IplImage * I, int taille, int** &strategie);

/**
 *@param ligne : indice de la ligne de la zone à prédire
 *@param colonne : indice de la colonne de la zone à prédire
 *@param I : image de référence pour la prédiction
 *@param Ipred : l'image sur laquelle se portera la prédiction
 *@param taille : taille des zones des prédictions
 */
void ReversepredictZone(int ligne, int colonne, IplImage * I, IplImage *&Ipred, int taille, int** &strategie);

/**
 *@param ligne : indice de la ligne du pixel à prédire
 *@param colonne : indice de la colonne du pixel à prédire
 *@param I : image de référence pour la prédiction
 *@param Ipred : l'image sur laquelle se portera la prédiction
 *@param taille : taille des zones des prédictions
 */
void ReversepredictPixel(int ligne, int colonne, IplImage * I, IplImage *&Ipred, int taille, int** &strategie);
#endif // LIB_H_INCLUDED
