#include "DCT.h"

void razFrequence(IplImage *I, int x, int y, int taille)
{
	//mise a zero des hautes fr�quence
	double *data = (double*)I->imageData;

	int xdeb, ydeb;
	xdeb = ydeb = taille/2; // "taux" de pr�cision voulu

	for(int i = 0; i < taille; i++)
	{
		for(int j = 0; j < taille; j++)
		{
			if (i >= xdeb || j >= ydeb)
			{
				data[(i+y)*I->width + (j+x)] = 0;
			}
		}
	}
}

int DCT3(IplImage *I, int taille)
{
    //penser a copier l'image I
    IplImage *A = cvCreateImage(cvSize(I->width, I->height), IPL_DEPTH_64F, 1);
    //cvCvtGray(I, I);

    for(int i = 0; i < A->width; i += taille)
	{
		for(int j = 0; j < A->height; j += taille)
		{
			cout << "(i, j) : (" << i << ", " << j << ") => (" << taille << ", " << taille << ")" << endl;
			cvSetImageROI(I, cvRect(i, j, taille, taille));
			cvDCT(I, I, CV_DXT_FORWARD);
			//razFrequence(A, i, j, taille);
			cvResetImageROI(I);
		}
	}

	cvNamedWindow("DCTA", CV_WINDOW_AUTOSIZE);
	cvShowImage("DCTA", I);

	cvWaitKey(0);

	cvDestroyWindow("DCTA");
	cvReleaseImage(&I);

	return EXIT_SUCCESS;
}
