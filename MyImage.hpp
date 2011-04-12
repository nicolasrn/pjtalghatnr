#ifndef _MyImage_
#define _MyImage_

#include <iostream>
#include <sstream>
#include <string>
using namespace std;

#include <OpenCV/cv.h>
#include <OpenCV/highgui.h>

/**
 * min between R, B and G values
 */
double minBGR( const double B, const double G, const double R );

/**
 * max between R, B and G values
 */
double maxBGR( const double B, const double G, const double R );

/**
 * @param I
 * 	image with one or several components
 *
 * @return the mimimum value for each component
 */
double* cvMinImage( const IplImage * I );

/**
 * @param I
 * 	image with one or several components
 *
 * @return the maximum value for each component
 */
double* cvMaxImage( const IplImage * I );

/**
 * @param I
 * 	image with one or several components
 *
 * @return the mean value for each component
 */
double* cvMeanImage( const IplImage * I );

/**
 * @param I
 * 	image with one or several components
 *
 * @return minPix
 * 	the minimum value for each component
 * @return maxPix
 * 	the maximum value for each component
 */
void cvMinMaxImage( const IplImage * I, double * & minPix, double * & maxPix );

void cvSubstractImage( IplImage * I, const double * values );

/**
 * @param Gray64F : IPL_DEPTH_64F grayscale image
 * @return Gray8U : IPL_DEPTH_8U	grayscale image
 */
void cvCvt8U( const IplImage * gray64F, IplImage * gray8U, CvSize sZ );


/**
 * @param Gray64F : IPL_DEPTH_64F grayscale image
 * @return Gray8S : IPL_DEPTH_8S	grayscale image
 */
void cvCvt8S( const IplImage* gray64F, IplImage* gray8S );

/**
 * @param I : IPL_DEPTH_64F image 3 channels
 *
 * @return I s.a. data translate into [0,1]
 * 	(b-min_B(I))/(max_B(I)-min_B(I))
 * 	(g-min_G(I))/(max_G(I)-min_G(I))
 * 	(r-min_R(I))/(max_R(I)-min_R(I))
 */
void cvNormalizeImage( IplImage * I );

/**
 * @param filename
 * 	image file name
 *
 * @return an IPL_DEPTH_64F image
 */
IplImage*	cvOpenImage( const char * filename );

/**
 * @param I : IplImage to visualize
 * @param s : striing as title fo the window
 * @param x, y : position coordonate of the window
 */
void cvShowAnyImage( IplImage * I, string s = "Nimp!!!", int x = 800, int y = 200 );

void minmax( IplImage * X );

void cvGausColor( IplImage * I, IplImage * & luma, IplImage * & chroma1, IplImage * & chroma2, char visu = 'N' );

IplImage*	cvResizeImage( IplImage * I, int Jmax );

void cvCvtGray( IplImage * I, IplImage * G );

/**
 * @param fct_name : the name of the function embedded into troubles
 * @param msg_error : the message to display
 *
 * use cvError() to print error message and stop process
 */
void cvErrMsg ( string fct_name, string msg_error );

/**
 * @param src : matrice de la source dont l'origine est située en haut à gauche
 * @param dst : matrice de la destination dont l'origine est centrée
 *
 * @return dst qui est l'image src après permuttion des quadrants pour centrer l'origine
 * Attention : il faut que src & dst soient de même taille et type
 */
void cvShiftDFT(CvArr * src, CvArr * dst );

/**
 * @param I : image source supposé réelle (pas complexe)
 *
 * @return F : la matrice fréquentielle complexe de la DFT
 */
CvMat * cvForwardDFT( IplImage * I );

#endif
