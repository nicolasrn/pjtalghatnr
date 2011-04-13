// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor Boston, MA 02110-1301,  USA

#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

#include <OpenCV/cv.h>
#include <OpenCV/highgui.h>

#include "MyImage.hpp"

double minBGR(const double B, const double G, const double R)
{
	return (B<G && B<R) ? B : ((G<R) ? G : R);
}

double maxBGR(const double B, const double G, const double R)
{
	return (B>G && B>R) ? B : ((G>R) ? G : R);
}

double* cvMinImage(const IplImage *I)
{
	const int nChannels = I->nChannels;
	const int taille = I->width*I->height;
	const double* dataI = (double*) I->imageData;
	double* minPix = (double*) calloc( nChannels, sizeof( double ) );

	for( int c=0; c<nChannels; c++ ) minPix[c] = dataI[c];

	for( int i=1; i<taille; i++ )
	{
		for( int c=0; c<nChannels; c++ )
		{
			if( minPix[c] > dataI[i*nChannels+c] )
			{
				minPix[c] = dataI[i*nChannels+c];
			}
		}
	}
	return minPix;
}

double* cvMaxImage(const IplImage *I)
{
	const int nChannels = I->nChannels;
	const int taille = I->width*I->height;
	const double* dataI = (double*) I->imageData;
	double* maxPix = (double*) calloc( nChannels, sizeof( double ) );

	for( int c=0; c<nChannels; c++ ) maxPix[c] = dataI[c];

	for( int i=1; i<taille; i++ )
	{
		for( int c=0; c<nChannels; c++ )
		{
			if( maxPix[c] < dataI[i*nChannels+c]  )
			{
				maxPix[c] = dataI[i*nChannels+c];
			}
		}
	}
	return maxPix;
}

double* cvMeanImage(const IplImage *I)
{
	const int nChannels = I->nChannels;
	const int taille = I->width*I->height;
	const double* dataI = (double*) I->imageData;
	double* meanPix = (double*) calloc( nChannels, sizeof( double ) );

	for( int i=1; i<taille; i++ )
	{
		for( int c=0; c<nChannels; c++ )
		{
			meanPix[c] += dataI[i*nChannels+c];
		}
	}

	for( int c=0; c<nChannels; c++ ) meanPix[c] /= taille;

	return meanPix;
}

void cvMinMaxImage(const IplImage* I, double *&minPix, double *&maxPix)
{
	const int nChannels = I->nChannels;
	const int taille = I->width*I->height;
	const double* dataI = (double*) I->imageData;

	minPix = (double*) calloc( nChannels, sizeof( double ) );
	maxPix = (double*) calloc( nChannels, sizeof( double ) );

	for( int c=0; c<nChannels; c++ )
	{
		minPix[c] = dataI[c];
		maxPix[c] = dataI[c];
	}

	for( int i=1; i<taille; i++ )
	{
		for( int c=0; c<nChannels; c++ )
		{
			if( minPix[c]>dataI[i*nChannels+c] )
			{
				minPix[c] = dataI[i*nChannels+c];
			}
			else if( maxPix[c]<dataI[i*nChannels+c]  )
			{
				maxPix[c] = dataI[i*nChannels+c];
			}
		}
	}
}

void cvSubstractImage( IplImage*	I, const double*		values )
{
	const int	nChannels = I->nChannels;
	const int	taille = I->width*I->height;
	double*		dataI = (double*) I->imageData;

	for( int i=0; i<taille; i++ )
	{
		for( int c=0; c<nChannels; c++ )
		{
			dataI[i*nChannels+c] = dataI[i*nChannels+c]-values[c];
		}
	}
}

void cvCvt8U( const IplImage* gray64F, IplImage* gray8U, CvSize sZ )
{
	const double* data64 = (double*) gray64F->imageData;
	const double min64 = *( cvMinImage( gray64F ) );
	const double max64 = *( cvMaxImage( gray64F ) );
	//const int	taille = gray64F->width*gray64F->height;
	unsigned char* data8 = (unsigned char*) gray8U->imageData;
	int p;

	for( int l=0; l<sZ.height; l++) for( int c=0; c<sZ.width; c++ )
	{
		p = l*sZ.width+c;
		data8[p] = (unsigned char) floor( (data64[p]-min64) / (max64-min64) * 255 );
	}
}

void cvCvt8S( const IplImage* gray64F, IplImage* gray8S )
{
	const double* data64 = (double*) gray64F->imageData;
	const double min64 = *( cvMinImage( gray64F ) );
	const double max64 = *( cvMaxImage( gray64F ) );
	const int	taille = gray64F->width*gray64F->height;
	char * data8 = (char *) gray8S->imageData;

	for( int p=0; p<taille; p++)
	{
		data8[p] = (char) ( (data64[p]-min64) / (max64-min64) * 255 );
	}
}

void cvNormalizeImage( IplImage* I )
{
	const int	nChannels = I->nChannels;
	const int	taille = I->width*I->height;
	double*		dataI = (double*) I->imageData;
	int				pos;
	double*		minPix;
	double*		maxPix;

	cvMinMaxImage( I, minPix, maxPix );

	for( int i=0; i<taille; i++ )
	{
		pos = i*nChannels;
		for( int c=0; c<nChannels; c++ )
		{
			dataI[pos+c] = (dataI[pos+c]-minPix[c])/(maxPix[c]-minPix[c]);
		}
	}
}

IplImage* cvOpenImage( const char* filename )
{
	// load an image and convert it into interleaved normalized 64FC3
	IplImage* I = cvLoadImage(filename);
	if (!I)
	{
		char* s = NULL;
		sprintf (s,"Cannot load image file: %s\n",filename);
		perror (s);
		exit (-1);
	}
	const int nChannels = I->nChannels;
	const int width = I->width;
	const int height = I->height;
	if( nChannels != 3 )
	{
		char* s = NULL;
		sprintf(s, "Only BGR or RGB uchar image format\n");
		perror (s);
		exit (-2);
	}
#ifdef _DEBUG_
	cout <<"cvOpenImage width="<<width<<" and height="<<height<<endl;
	cvShowAnyImage (I, "image at opening");
#endif
	if (I->depth == IPL_DEPTH_64F)
	{
		cvNormalizeImage (I);
		return I;
	}
	if( I->depth == IPL_DEPTH_32F )
	{
		IplImage*	X = cvCreateImage (cvGetSize (I), IPL_DEPTH_64F, nChannels);
		float*		dataI = (float*) I->imageData;
		const int 	widthStep = I->widthStep/sizeof(float);
		double*		dataX = (double*) X->imageData;
		const int	newStep = X->widthStep/sizeof(double);

		for (int row=0; row < height; row++)
		{
			for (int col=0; col < width; col++)
			{
				for (int ch=0; ch < nChannels; ch++)
				{
					dataX[row*newStep+col*nChannels+ch] =
					(double) dataI[row*widthStep+col*nChannels+ch];
				}
			}
		}
		cvReleaseImage( &I );
		cvNormalizeImage( X );
		return X;
	}
	if( I->depth ==IPL_DEPTH_8U )
	{
		IplImage*	X = cvCreateImage (cvGetSize (I), IPL_DEPTH_64F, nChannels);
		uchar*		dataI = (uchar*) I->imageData;
		const int 	widthStep = I->widthStep/sizeof(uchar);
		double*		dataX = (double*) X->imageData;
		const int	newStep = X->widthStep/sizeof(double);

		for (int row=0; row < height; row++)
		{
			for (int col=0; col < width; col++)
			{
				for (int ch=0; ch < nChannels; ch++)
				{
					dataX[row*newStep+col*nChannels+ch] =
					(double) dataI[row*widthStep+col*nChannels+ch];
				}
			}
		}
		cvReleaseImage( &I );
		cvNormalizeImage (X);
		return X;
	}
	else
	{
		char* s = NULL;
		sprintf(s, "Only 64F, 32F or 8U image depth\n");
		perror (s);
		exit (-2);
	}
}

void cvShowAnyImage(IplImage* I, string s, int x, int y )
{
	IplImage * J ;

    if( I->depth == IPL_DEPTH_8U )
        J = I;
    else if( I->depth == IPL_DEPTH_64F )
    {
        int nCh = I->nChannels;//, pos;
        CvScalar pxI, pxJ;

        double * minPix, * maxPix;
        cvMinMaxImage( I, minPix, maxPix );

        J = cvCreateImage( cvGetSize(I), IPL_DEPTH_8U, 3 );
        for( int l=0; l<I->height; l++ )
            for( int c=0; c<I->width; c++ )
            {
                pxI = cvGet2D( I, l, c );
                for( int ch=0; ch<nCh; ch++ )
                {
                    pxJ.val[ch] = (pxI.val[ch]-minPix[ch]) / (maxPix[ch]-minPix[ch]) * 255;
                }
                cvSet2D( J, l, c, pxJ );
            }
    }


	// create a window
	cvNamedWindow(s.c_str(), CV_WINDOW_AUTOSIZE);
	cvMoveWindow(s.c_str(), x, y);

	// show image
	cvShowImage (s.c_str (), J);
	cvWaitKey(0);
}

void cvGausColor( IplImage* I, IplImage*& luma, IplImage*& chroma1, IplImage*& chroma2, char visu )
{
	// from BGR (32FC3)to Gaussian Color (Ell,El,E) (32FC3)
	// from I to 2 separate images : one for luma, the other for chroma
	const int	nChG = 3;
	double*		dataI = (double*) I->imageData;
	CvSize		SizI = cvGetSize (I);
	int			taille = SizI.width*SizI.height;
	float		couleur[nChG];
	int 		i, l, c;
	const float GC[nChG][nChG] =
	{
		// BGR and not RGB version!!!
		{0.31, 0.63, 0.06},
		{-0.37, 0.18, 0.19},
		{0.06, -0.44, 0.22}
	};

	// linear transformation (Ell,El,E) = GC * (B,G,R)
	for (i=0; i < taille; i++)
	{
		for (c=0; c < nChG; c++) couleur[c] = dataI[nChG*i+c];
		for (l=0; l < nChG; l++)
		{
			dataI[nChG*i+l] = 0;
			for (c=0; c < nChG; c++) dataI[nChG*i+l] += GC[l][c]*couleur[c];
		}
	}
	cvNormalizeImage (I);

	// separate gaussian color components
	luma = cvCreateImage (SizI, IPL_DEPTH_64F, 1);
	chroma1 = cvCreateImage (SizI, IPL_DEPTH_64F, 1);
	chroma2 = cvCreateImage (SizI, IPL_DEPTH_64F, 1);
	double* dataL = (double*) luma->imageData;
	double* dataC1 = (double*) chroma1->imageData;
	double* dataC2 = (double*) chroma2->imageData;

	for (i=0; i < taille ; i++)
	{
		dataL[i] = dataI[nChG*i];
		dataC1[i] = dataI[nChG*i+1];
		dataC2[i] = dataI[nChG*i+2];
	}
#ifdef _DEBUG_
	{
		double minL  = dataL[0];
		double maxL  = dataL[0];
		double minC1 = dataC1[0];
		double maxC1 = dataC1[0];
		double minC2 = dataC2[0];
		double maxC2 = dataC2[0];
		for (i=1; i < taille; i++)
		{
			if (dataL[i]<minL) minL = dataL[i];
			else if (dataL[i]>maxL) maxL = dataL[i];
			if (dataC1[i]<minC1) minC1 = dataC1[i];
			else if (dataC1[i]>maxC1) maxC1 = dataC1[i];
			if (dataC2[i]<minC2) minC2 = dataC2[i];
			else if (dataC2[i]>maxC2) maxC2 = dataC2[i];
		}
		cout <<"min max de Gauss ";
		cout <<":"<<minL<<":"<< maxL;
		cout <<" :"<<minC1<<":"<< maxC1;
		cout <<" :"<<minC2<<":"<< maxC2<<endl;
	}
#endif
	cvReleaseImage (&I);
	cvNormalizeImage (luma);
	cvNormalizeImage (chroma1);
	cvNormalizeImage (chroma2);
	if (visu == 'O')
	{
		cvShowAnyImage (luma, "Luma Gaussian image");
		cvShowAnyImage (chroma1, "Chroma 1 Gaussian image");
		cvShowAnyImage (chroma2, "Chroma 2 Gaussian image");
	}


#ifdef _DEBUG_
	cvShowAnyImage (luma, "E image");
	cvShowAnyImage (chroma1, "El image");
	cvShowAnyImage (chroma2, "Ell image");
	for (i=0; i < taille; i++)
	{
		if (dataL[i]<0 || dataL[i]>1) cerr <<"Alert luma normalization\n";
		if (dataC1[i]<0 || dataC1[i]>1) cerr <<"Alert chroma C1 normalization\n";
		if (dataC2[i]<0 || dataC2[i]>1) cerr <<"Alert chroma C2 normalization\n";
	}
#endif
}

IplImage* cvResizeImage( IplImage* I, int Jmax )
{
	const int c = pow (2, Jmax);
	const int newWidth = ((I->width)/c)*c;
	const int newHeight = ((I->height)/c)*c;
	const int nChannels = I->nChannels;
	const int step = I->widthStep/sizeof(double);
	const CvSize Sz = cvSize( newWidth, newHeight );
	IplImage* rescaledI = cvCreateImage( Sz, IPL_DEPTH_64F, nChannels );
	double* dataRI = (double*) (rescaledI->imageData);
	double* dataI = (double*) (I->imageData);
	const int newStep = rescaledI->widthStep/sizeof(double);

	for (int row=0; row < newHeight; row++)
	{
		for (int col=0; col < newWidth; col++)
		{
			for (int ch=0; ch < nChannels; ch++)
			{
				dataRI[row*newStep+col*nChannels+ch] =
				dataI[row*step+col*nChannels+ch];
			}
		}
	}
	cvNormalizeImage (rescaledI);
#ifdef _DEBUG_
	cout <<"initial width="<<I->width<<" and height="<<I->height<<endl;
	cout <<"rescaling width="<<newWidth<<" and height="<<newHeight<<endl;
	cvShowAnyImage (I, "image before rescaling");
	cvShowAnyImage (rescaledI, "Rescaled image");
#endif
	return rescaledI;
}

void minmax( IplImage* X )
{
	const int nc = X->nChannels;
	const int taille = X->width*X->height;
	double* dataX = (double*) X->imageData;
	double min[nc], max[nc];

	for(int c=0; c<nc; c++ )
	{
		min[c] = dataX[c];
		max[c] = dataX[c];
	}
	for( int i=0; i<taille; i++ )
	{
		for( int c=0; c<nc; c++ )
		{
			if( min[c]>dataX[i*nc+c] )
			{
				min[c] = dataX[i*nc+c];
			}
			else if( max[c]<dataX[i*nc+c] )
			{
				max[c] = dataX[i*nc+c];
			}
		}
	}
	for( int c=0; c<nc; c++ )
	{
		cout <<"min["<<c<<"]="<<min[c]<<" :: ";
		cout <<"max["<<c<<"]="<<max[c]<<endl;
	}
}

void cvCvtGray( IplImage* I, IplImage* G )
{
	int i,j;
	double* dataI = (double*) I->imageData;
	double* dataG = (double*) G->imageData;
	for( i=0; i<I->height; i++ )
	{
		for( j=0; j<I->width; j++ )
		{
			dataG[i*I->width+j] =
			0.2989*dataI[(i*I->width+j)*I->nChannels+2] +
			0.5870*dataI[(i*I->width+j)*I->nChannels+1] +
			0.1140*dataI[(i*I->width+j)*I->nChannels];
		}
	}
	cvScale( G, G, 1.0, 0.0);
}

void cvErrMsg ( string fct_name, string msg_error )
{
	cvError(  CV_StsUnmatchedSizes, fct_name.c_str(), msg_error.c_str(), __FILE__, __LINE__ );
}

void cvShiftDFT(CvArr * src, CvArr * dst )
{
	CvMat q1stub, q2stub, q3stub, q4stub;
	CvSize src_size = cvGetSize( src );
	CvSize dst_size = cvGetSize( dst );

	if( dst_size.width != src_size.width || dst_size.height != src_size.height ) cvErrMsg(  "cvShiftDFT", "Source et Destination doivent avoir meme taille" );

	int cx = src_size.width/2;
	int cy = src_size.height/2; // image center

	CvMat * q1 = cvGetSubRect( src, &q1stub, cvRect(  0,  0, cx, cy ) );
	CvMat * q2 = cvGetSubRect( src, &q2stub, cvRect( cx,  0, cx, cy ) );
	CvMat * q3 = cvGetSubRect( src, &q3stub, cvRect( cx, cy, cx, cy ) );
	CvMat * q4 = cvGetSubRect( src, &q4stub, cvRect(  0, cy, cx, cy ) );

	if( src != dst )
	{
		CvMat d1stub, d2stub, d3stub, d4stub;
		CvMat * d1 = cvGetSubRect( dst, &d1stub, cvRect(  0,  0, cx, cy ) );

		if( !CV_ARE_TYPES_EQ( q1, d1 ) ) cvErrMsg(  "cvShiftDFT", "Source et Destination doivent avoir meme taille" );

		CvMat * d2 = cvGetSubRect( dst, &d2stub, cvRect( cx,  0, cx, cy ) );
		CvMat * d3 = cvGetSubRect( dst, &d3stub, cvRect( cx, cy, cx, cy ) );
		CvMat * d4 = cvGetSubRect( dst, &d4stub, cvRect(  0, cy, cx, cy ) );

		cvCopy( q3, d1, 0 );
		cvCopy( q4, d2, 0 );
		cvCopy( q1, d3, 0 );
		cvCopy( q2, d4, 0 );
	}
	else
	{
		CvMat * tmp = cvCreateMat(src_size.height/2, src_size.width/2, cvGetElemType( src ) );

		cvCopy(  q3, tmp, 0 );
		cvCopy(  q1,  q3, 0 );
		cvCopy( tmp,  q1, 0 );
		cvCopy(  q4, tmp, 0 );
		cvCopy(  q2,  q4, 0 );
		cvCopy( tmp,  q2, 0 );
	}
}

CvMat * cvForwardDFT( IplImage * I )
{
	// création de l'image d'entrée au format complexe
	IplImage * Re = cvCreateImage( cvGetSize( I ), IPL_DEPTH_64F, 1 );
	IplImage * Im = cvCreateImage( cvGetSize( I ), IPL_DEPTH_64F, 1 );
	IplImage * C = cvCreateImage( cvGetSize( I ), IPL_DEPTH_64F, 2 );
	double m, M;
	// partie réelle
	cvScale(I, Re, 1.0, 0.0);
	cvMinMaxLoc( Re, &m, &M, NULL, NULL, NULL);
	// partie imaginaire
	cvZero( Im );
	// regrouper les deux parties
	cvMerge(Re, Im, NULL, NULL, C );
	cvReleaseImage( &Re );
	cvReleaseImage( &Im );

	// construction de la matrice fréquentielle
	int dft_M = cvGetOptimalDFTSize( C->height-1 );
	int dft_N = cvGetOptimalDFTSize( C->width-1 );
	CvMat * F = cvCreateMat( dft_M, dft_N, CV_64FC2 );
	CvMat tmp;

	cvGetSubRect( F, &tmp, cvRect( 0, 0, C->width, C->height ));
	cvCopy( C, &tmp, NULL );
	if( F->cols > C->width )
	{
		cvGetSubRect( F, &tmp, cvRect( C->width, 0, F->cols-C->width, C->height ));
		cvZero( &tmp );
	}
	cvDFT( F, F, CV_DXT_FORWARD, C->height );

	return F;
}
