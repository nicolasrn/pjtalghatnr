#include "DCT.h"

void razFrequence(IplImage *I, int x, int y, int taille)
{
	//mise a zero des hautes fréquence
	double *data = (double*)I->imageData;

	int xdeb, ydeb;
	xdeb = ydeb = taille/2; // "taux" de précision voulu

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

int DCT3(IplImage *I, int taille, std::string id)
{
    //penser a copier l'image I
    IplImage *A = cvCreateImage(cvSize(I->width, I->height), IPL_DEPTH_64F, 1);
    cvCopy(I, A);
    //cvCvtGray(A, A);

    for(int i = 0; i < A->width; i += taille)
	{
		for(int j = 0; j < A->height; j += taille)
		{
			//cout << "(i, j) : (" << i << ", " << j << ") => (" << taille << ", " << taille << ")" << endl;
			cvSetImageROI(A, cvRect(i, j, taille, taille));
			cvDCT(A, A, CV_DXT_FORWARD);
			//razFrequence(A, i, j, taille);
			cvResetImageROI(A);
		}
	}

	cvShowAnyImage(A, id.c_str());

	cvDestroyWindow(id.c_str());
	cvReleaseImage(&A);

	return EXIT_SUCCESS;
}
