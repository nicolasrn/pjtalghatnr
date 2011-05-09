#include "lib.h"
#include "math.h"

void cvShowAnyImageYUV(string s, IplImage * I, int x, int y)
{
    cvShowAnyImage(I, s, x, y);
}

void cvShowAnyImage(string s, IplImage *I, int x, int y)
{
    // create a window
	cvNamedWindow(s.c_str(), CV_WINDOW_AUTOSIZE);
	cvMoveWindow(s.c_str(), x, y);

	// show image
	cvShowImage (s.c_str (), I);
	cvWaitKey(0);
}

IplImage * cvBGR2YUV( IplImage * BGR )
{
    IplImage * YUV = cvCreateImage( cvGetSize(BGR), IPL_DEPTH_64F, 3 );
    CvScalar pxBGR, pxYUV;

    // conversion
    for( int ligne=0; ligne<BGR->height; ligne++ ) // pour chaque ligne
        for( int colonne=0; colonne<BGR->width; colonne++ ) // pour chaque colonne
        {
            // attention ici on a trois valeurs (BGR) par pixel
            pxBGR = cvGet2D( BGR, ligne, colonne );
            pxYUV.val[2] = pxBGR.val[0] - pxBGR.val[2]; // V
            pxYUV.val[1] = pxBGR.val[1] - pxBGR.val[2] - ceil(pxYUV.val[2]/2 ); // U
            pxYUV.val[0] = pxBGR.val[1] - floor(pxYUV.val[1]/2); // Y
            cvSet2D( YUV, ligne, colonne, pxYUV );
        }
    return YUV;
}

void separateComponents( IplImage * YUV, IplImage *&Y, IplImage *&U, IplImage *&V )
{
    Y = cvCreateImage(cvGetSize(YUV), YUV->depth, 1 );
    U = cvCreateImage(cvGetSize(YUV), YUV->depth, 1 );
    V = cvCreateImage(cvGetSize(YUV), YUV->depth, 1 );
    CvScalar pxYUV, px1Comp;

    for( int ligne=0; ligne<Y->height; ligne ++ )
    {
        for( int colonne=0; colonne<Y->width; colonne++ )
        {
            pxYUV = cvGet2D( YUV, ligne, colonne );
            px1Comp.val[0] = pxYUV.val[0];
            cvSet2D( Y, ligne, colonne, px1Comp );
            px1Comp.val[0] = pxYUV.val[1];
            cvSet2D( U, ligne, colonne, px1Comp );
            px1Comp.val[0] = pxYUV.val[2];
            cvSet2D( V, ligne, colonne, px1Comp );
        }
    }
}

IplImage* unifiateComponents(IplImage *Y, IplImage *U, IplImage *V)
{
    IplImage *YUV = cvCreateImage(cvGetSize(Y), IPL_DEPTH_64F, 3 );

    CvScalar px, pximg;

    for( int ligne=0; ligne<Y->height; ligne ++ )
    {
        for( int colonne=0; colonne<Y->width; colonne++ )
        {
            pximg = cvGet2D(YUV, ligne, colonne);
            px = cvGet2D(Y, ligne, colonne);
            pximg.val[0] = px.val[0];
            px = cvGet2D(U, ligne, colonne);
            pximg.val[1] = px.val[0];
            px = cvGet2D(V, ligne, colonne);
            pximg.val[2] = px.val[0];

            cvSet2D(YUV, ligne, colonne, pximg);
        }
    }

    return YUV;
}

IplImage * cvYUV2BGR( IplImage * YUV )
{
    IplImage * BGR = cvCreateImage( cvGetSize(YUV), YUV->depth, 3 );
    CvScalar pxYUV, pxBGR;

    for( int ligne=0; ligne<BGR->height; ligne++ )
    {
        for( int colonne=0; colonne<BGR->width; colonne++ )
        {
            pxYUV = cvGet2D( YUV, ligne, colonne );
            pxBGR.val[1] = pxYUV.val[0] + floor(pxYUV.val[1]/2);
            pxBGR.val[2] = pxBGR.val[1] - pxYUV.val[1] - ceil(pxYUV.val[2]/2);
            pxBGR.val[0] = pxYUV.val[2] + pxBGR.val[2];
            cvSet2D( BGR, ligne, colonne, pxBGR );
        }
    }
    return BGR;
}

int getMultiple16(int nb)
{
    int mul = 0;

    while((nb+mul)%16 != 0)
          mul++;

    return mul + nb;
}

IplImage* ajustementImage(IplImage *image)
{
    int widthAjuste = image->width, heightAjuste = image->height;

    if (image->width % 16 != 0) //ajustement de la width
        widthAjuste = getMultiple16(image->width);

    if (image->height % 16 != 0) //ajustement de la height
        heightAjuste = getMultiple16(image->height);

    //std::cout << widthAjuste << ", " << heightAjuste << std::endl;
    //creation de l'image ajustée
    IplImage *A = cvCreateImage(cvSize(widthAjuste, heightAjuste), IPL_DEPTH_64F, image->nChannels);
    CvScalar px;

    //cout << "taille origine : " << A->width << ", " << A->height << endl;
    //cout << "taille ajuste : " << A->width << ", " << A->height << endl;

    //remplissage avec du noir de l'image
    for(int i = 0; i < image->height; i++)
    {
        for(int j = 0; j < image->width; j++)
        {
            px = cvGet2D(image, i, j);
            cvSet2D(A, i, j, px);
        }
    }

    return A;
}

IplImage* recouvrement(IplImage *image, int taille)
{
    IplImage *A = cvCreateImage(cvSize(image->width - taille, image->height - taille), IPL_DEPTH_64F, image->nChannels);
    CvScalar px;

    //cout << "recouvrement : " << A->width << ", " << A->height << endl;

    for(int i = taille/2; i < image->height - taille/2; i++)
    {
        for(int j = taille/2; j < image->width - taille/2; j++)
        {
            px = cvGet2D(image, i, j);
            cvSet2D(A, i - taille/2, j - taille/2, px);
        }
    }

    return A;
}

IplImage* mergedRecouvrement(IplImage *im, IplImage *r, int taille)
{
	CvScalar ia, ra;
	for(int y = taille/2; y < im->height - taille/2; y++)
	{
		for(int x = taille/2; x < im->width - taille/2; x++)
		{
            ia = cvGet2D(im, y, x);
            ra = cvGet2D(r, y+taille/2, x+taille/2);
            ra.val[0] = (ia.val[0] + ra.val[0])/2;
            ra.val[1] = (ia.val[1] + ra.val[1])/2;
            ra.val[2] = (ia.val[2] + ra.val[2])/2;
            cvSet2D(im, y, x, ra);
		}
	}
	return im;
}

void quantification(IplImage * I, int QP, int* R, int i, int ** q, int x, int y) //y = l
{
    CvScalar px;

    //cout << "DEB" << endl;
	//Remplissage manuel de x en triant selon le modèle
	px = cvGet2D( I, 0, 0);
	q[y][x] = floor(px.val[0]/(pow(2., QP+R[i])));
	//cout << px.val[0] << " ? " << (q[y][x] * pow(2., QP+R[i])) << endl;

	px = cvGet2D( I, 0, 1);
	q[y][x+1] = floor(px.val[0]/(pow(2, QP+R[i])));
	//cout << px.val[0] << " ? " << (q[y][x+1] * pow(2., QP+R[i])) << endl;

	px = cvGet2D( I, 1, 0);
	q[y+1][x] = floor(px.val[0]/(pow(2, QP+R[i])));
	//cout << px.val[0] << " ? " << (q[y+1][x] * pow(2., QP+R[i])) << endl;

	px = cvGet2D( I, 0, 2);
	q[y][x+2] = floor(px.val[0]/(pow(2, QP+R[i])));
	//cout << px.val[0] << " ? " << (q[y][x+2] * pow(2., QP+R[i])) << endl;

	px = cvGet2D( I, 2, 0);
	q[y+2][x] = floor(px.val[0]/(pow(2, QP+R[i])));
	//cout << px.val[0] << " ? " << (q[y+2][x] * pow(2., QP+R[i])) << endl;

	px = cvGet2D( I, 0, 3);
	q[y][x+3] = floor(px.val[0]/(pow(2, QP+R[i])));
	//cout << px.val[0] << " ? " << (q[y][x+3] * pow(2., QP+R[i])) << endl;

	px = cvGet2D( I, 3, 0);
	q[y+3][x] = floor(px.val[0]/(pow(2, QP+R[i])));
	//cout << px.val[0] << " ? " << (q[y+3][x] * pow(2., QP+R[i])) << endl;

	px = cvGet2D( I, 1, 1);
	q[y+1][x+1] = floor(px.val[0]/(pow(2, QP+R[i])));
	//cout << px.val[0] << " ? " << (q[y+1][x+1] * pow(2., QP+R[i])) << endl;

	px = cvGet2D( I, 1, 2);
	q[y+1][x+2] = floor(px.val[0]/(pow(2, QP+R[i])));
	//cout << px.val[0] << " ? " << (q[y+1][x+2] * pow(2., QP+R[i])) << endl;

	px = cvGet2D( I, 2, 1);
	q[y+2][x+1] = floor(px.val[0]/(pow(2, QP+R[i])));
	//cout << px.val[0] << " ? " << (q[y+2][x+1] * pow(2., QP+R[i])) << endl;

	px = cvGet2D( I, 3, 1);
	q[y+3][x+1] = floor(px.val[0]/(pow(2, QP+R[i])));
	//cout << px.val[0] << " ? " << (q[y+3][x+1] * pow(2., QP+R[i])) << endl;

	px = cvGet2D( I, 2, 2);
	q[y+2][x+2] = floor(px.val[0]/(pow(2, QP+R[i])));
	//cout << px.val[0] << " ? " << (q[y+2][x+2] * pow(2., QP+R[i])) << endl;

	px = cvGet2D( I, 1, 3);
	q[y+1][x+3] = floor(px.val[0]/(pow(2, QP+R[i])));
	//cout << px.val[0] << " ? " << (q[y+1][x+3] * pow(2., QP+R[i])) << endl;

	px = cvGet2D( I, 2, 3);
	q[y+2][x+3] = floor(px.val[0]/(pow(2, QP+R[i])));
	//cout << px.val[0] << " ? " << (q[y+2][x+3] * pow(2., QP+R[i])) << endl;

	px = cvGet2D( I, 3, 2);
	q[y+3][x+2] = floor(px.val[0]/(pow(2, QP+R[i])));
	//cout << px.val[0] << " ? " << (q[y+3][x+2] * pow(2., QP+R[i])) << endl;

	px = cvGet2D( I, 3, 3);
	q[y+3][x+3] = floor(px.val[0]/(pow(2, QP+R[i])));

	/*if (y+3 == 63 && x+3 == 63)
    {
        cout << "px = " << px.val[0] << " " << QP << " " << R[i] << endl;
        cout << px.val[0] << "/(" << 2 << "^(" << QP << "+" << R[i] << "))" << endl;
        cout << "quantif : (" << y+3 << ", " << x+3 << ") : " << q[y+3][x+3] << " ? " << floor(px.val[0] / pow(2., QP+R[i])) << endl;
    }*/

	//cout << "FIN" << endl;
}

int **applyQuantification( IplImage *I, int QP, int* R, int i)
{
    //int **tab = (int **)malloc (sizeof(int*)* I->width * I->height);

    //declaration mode c
    int **tab = (int **)malloc (sizeof(int*)* I->height);
    for(int k = 0; k < I->height; k++)
        tab[k] = (int*)malloc(sizeof(int)*I->width);

    //déclaration mode c++
    //int **tab = new int*[I->height];
    //for(int k = 0; k < I->height; k++)
    //    tab[k] = new int[I->width];

    //penser a copier l'image I
    IplImage *A = cvCreateImage(cvSize(I->width, I->height), IPL_DEPTH_64F, 1);
    cvCopy(I, A);

    for(int y = 0; y < A->height; y += 4)
    {
        for(int x = 0; x < A->width; x += 4)
        {
            //cout << "traitement de : (" << x << ", " << y << ")" << endl;
            cvSetImageROI(A, cvRect(x, y, 4, 4));
            quantification(A, QP, R, i, tab, x, y);
            cvResetImageROI(A);
        }
    }

    return tab;
}

IplImage * ReverseApplyQuantification(int ** q, int QP, int* R, int i, int width, int height)
{
    IplImage * I = cvCreateImage(cvSize(width, height), IPL_DEPTH_64F, 1 );

    for(int y = 0; y < I->height; y += 4)
    {
        for(int x = 0; x < I->width; x += 4)
        {
            cvSetImageROI(I, cvRect(x, y, 4, 4));
            ReverseQuantification (I,QP,R,i,q,x,y);
            cvResetImageROI(I);
        }
    }
    return I;
}

void ReverseQuantification (IplImage * I, int QP, int* R, int i, int ** q, int x, int y) //l = y
{
    CvScalar px;
    px.val[0] = px.val[1] = px.val[2] = px.val[3] = px.val[4] = 0.;

    //Remplissage manuel de x en triant selon le modèle
    //px = cvGet2D( I, 0, 0);
    //double tmp = 0;
    px.val[0] = q[y][x] * pow(2., QP+R[i]);
    cvSet2D(I, 0, 0, px);

    //px = cvGet2D( I, 0, 1);
    px.val[0] = q[y][x+1] * pow(2., QP+R[i]);
    cvSet2D(I, 0, 1, px);

    //px = cvGet2D( I, 1, 0);
    px.val[0] = q[y+1][x] * pow(2., QP+R[i]);
    cvSet2D(I, 1, 0, px);

    //px = cvGet2D( I, 0, 2);
    px.val[0] = q[y][x+2] * pow(2., QP+R[i]);
    cvSet2D(I, 0, 2, px);

    //px = cvGet2D( I, 2, 0);
    px.val[0] = q[y+2][x] * pow(2., QP+R[i]);
    cvSet2D(I, 2, 0, px);

    //px = cvGet2D( I, 0, 3);
    px.val[0] = q[y][x+3] * pow(2., QP+R[i]);
    cvSet2D(I, 0, 3, px);

    //px = cvGet2D( I, 3, 0);
    px.val[0] = q[y+3][x] * pow(2., QP+R[i]);
    cvSet2D(I, 3, 0, px);

    //px = cvGet2D( I, 1, 1);
    px.val[0] = q[y+1][x+1] * pow(2., QP+R[i]);
    cvSet2D(I, 1, 1, px);

    //px = cvGet2D( I, 1, 2);
    px.val[0] = q[y+1][x+2] * pow(2., QP+R[i]);
    cvSet2D(I, 1, 2, px);

    //px = cvGet2D( I, 2, 1);
    px.val[0] = q[y+2][x+1] * pow(2., QP+R[i]);
    cvSet2D(I, 2, 1, px);

    //px = cvGet2D( I, 3, 1);
    px.val[0] = q[y+3][x+1] * pow(2., QP+R[i]);
    cvSet2D(I, 3, 1, px);

    //px = cvGet2D( I, 2, 2);
    px.val[0] = q[y+2][x+2] * pow(2., QP+R[i]);
    cvSet2D(I, 2, 2, px);

    //px = cvGet2D( I, 1, 3);
    px.val[0] = q[y+1][x+3] * pow(2., QP+R[i]);
    cvSet2D(I, 1, 3, px);

    //px = cvGet2D( I, 2, 3);
    px.val[0] = q[y+2][x+3] * pow(2., QP+R[i]);
    cvSet2D(I, 2, 3, px);

    //px = cvGet2D( I, 3, 2);
    px.val[0] = q[y+3][x+2] * pow(2., QP+R[i]);
    cvSet2D(I, 3, 2, px);

    //px = cvGet2D( I, 3, 3);
    px.val[0] = q[y+3][x+3] * pow(2., QP+R[i]);
    cvSet2D(I, 3, 3, px);
    /*if (y+3 == 63 && x+3 == 63)
    {
        cout << "q[y+3][x+3] = " << q[y+3][x+3] << " " << QP << " " << R[i] << endl;
        cout << q[y+3][x+3] << "*(" << 2 << "^(" << QP << "+" << R[i] << ")) = " << q[y+3][x+3] * pow(2., QP+R[i]) << endl;
        cout << "reverse : (" << y+3 << ", " << x+3 << ") : " << px.val[0] << " ? " << q[y+3][x+3] * pow(2., QP+R[i]) << endl;
    }*/
}

void intializeStrategie(int **&strategie, int width, int height)
{
    strategie = new int*[width];
    for(int i = 0; i < width; i++)
        strategie[i] = new int[height];

    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++)
            strategie[j][i] = 0;
}

IplImage * predictImage(IplImage * I, int taille, int **&strategie)
{
    //On copie l'image à prédire dans l'image prédite
    IplImage * Ipred = cvCreateImage(cvSize(I->width, I->height), IPL_DEPTH_64F, 1);
    cvCopy(I, Ipred);

    //On met à 0
    intializeStrategie(strategie, I->width, I->height);

    //On parcourt les zones de 4x4
    for(int i = 0; i < Ipred->height; i += taille)
    {
        for(int j = 0; j < Ipred->width; j += taille)
        {
            predictZone(i, j, I, Ipred, taille, strategie);
        }
    }

    return Ipred;
 }

void predictZone(int ligne, int colonne, IplImage * I, IplImage *&Ipred, int taille, int **&strategie)
{
	if (!(ligne == 0 && colonne == 0))
	{
		//prediction du pixel DC de l'image
		predictPixel(ligne, colonne, I, Ipred, taille, strategie);

		//On fait la prédiction sur la ligne haute des AD
		for(int k = 1; k < taille; k++)
		{
			predictPixelAD(ligne, colonne + k, I, Ipred, taille, strategie,strategie[colonne][ligne]);
		}

		//On fait la prédiction sur la ligne gauche des AD
		for(int k = 1; k < taille; k++)
		{
			predictPixelAD(ligne + k, colonne, I, Ipred, taille, strategie,strategie[colonne][ligne]);
		}
	}
}

void predictPixel(int ligne, int colonne, IplImage * I, IplImage *&Ipred, int taille, int **&strategie)
{
    //Pixel pour l'image a prédire, et pour l'image prédite
    CvScalar pxITop, pxILeft, pxI, pxIpred, pxBest;
    bool choixTop = true, choixLeft = true;

    //On récupère la valeur du pixel à prédire
    pxI = cvGet2D( I, ligne, colonne );

    //On vérifie qu'on n'est pas sur une zone de bord haut
    if(ligne - taille < 0){
        pxITop.val[0] = pxI.val[0];
		choixTop = false;
    }
    else
        pxITop = cvGet2D( I, ligne - taille, colonne);

    //On vérifie qu'on n'est pas sur une zone de bord gauche
    if(colonne - taille < 0){
        pxILeft.val[0] = pxI.val[0];
		choixLeft = false;
    }
    else
        pxILeft = cvGet2D( I, ligne, colonne - taille);

    //On recherche la meilleur stratégie pour le pixel
    if((pxI.val[0] - pxITop.val[0] <= pxI.val[0] - pxILeft.val[0] && choixTop) || !choixLeft){
        pxBest.val[0] = pxITop.val[0];
        strategie[colonne][ligne] = 1;
    }
    else if ((pxI.val[0] - pxITop.val[0] > pxI.val[0] - pxILeft.val[0] && choixLeft) || !choixTop){
        pxBest.val[0] = pxILeft.val[0];
        strategie[colonne][ligne] = 2;
    }

    pxIpred.val[0] = pxI.val[0] - pxBest.val[0];
    cvSet2D( Ipred, ligne, colonne, pxIpred);
}

void predictPixelAD(int ligne, int colonne, IplImage * I, IplImage *&Ipred, int taille, int** &strategie, int stratDC){
      //Pixel pour l'image a prédire, et pour l'image prédite
    CvScalar pxITop, pxILeft, pxI, pxIpred;


    //On récupère la valeur du pixel à prédire
    pxI = cvGet2D( I, ligne, colonne );

    if(stratDC == 1){
    // stratégie haut
    pxITop = cvGet2D( I, ligne - taille, colonne);
    strategie[colonne][ligne] = 1;
    pxIpred.val[0] = pxI.val[0] - pxITop.val[0];
    }
    else if ( stratDC == 2 ){
    //stratégie de gauche
    pxILeft = cvGet2D( I, ligne, colonne - taille);
    strategie[colonne][ligne] = 2;
     pxIpred.val[0] = pxI.val[0] - pxILeft.val[0];
    }
    else{
    //pas de stratégie à appliquer
    strategie[colonne][ligne] = 0;
    pxIpred.val[0] = pxI.val[0];

    }
    cvSet2D( Ipred, ligne, colonne, pxIpred);

   /* //On vérifie qu'on n'est pas sur une zone de bord haut
    if(ligne - taille < 0){
        pxITop.val[0] = pxI.val[0];
		choixTop = false;
    }
    else
        pxITop = cvGet2D( I, ligne - taille, colonne);

    //On vérifie qu'on n'est pas sur une zone de bord gauche
    if(colonne - taille < 0){
        pxILeft.val[0] = pxI.val[0];
		choixLeft = false;
    }
    else
        pxILeft = cvGet2D( I, ligne, colonne - taille);

    //On recherche la meilleur stratégie pour le pixel
    if((pxI.val[0] - pxITop.val[0] <= pxI.val[0] - pxILeft.val[0] && choixTop) || !choixLeft){
        pxBest.val[0] = pxITop.val[0];
        strategie[colonne][ligne] = 1;
    }
    else if ((pxI.val[0] - pxITop.val[0] > pxI.val[0] - pxILeft.val[0] && choixLeft) || !choixTop){
        pxBest.val[0] = pxILeft.val[0];
        strategie[colonne][ligne] = 2;
    }

    pxIpred.val[0] = pxI.val[0] - pxBest.val[0];*/


}

IplImage * ReversepredictImage(IplImage * I, int taille, int** &strategie)
{
    IplImage * Ipred = cvCreateImage(cvSize(I->width, I->height), IPL_DEPTH_64F, 1);
    cvCopy(I, Ipred);

    //On parcourt les zones de 4x4
    for(int i = 0; i < Ipred->height; i += taille)
    {
        for(int j = 0; j < Ipred->width; j += taille)
        {
            ReversepredictZone(i, j, I, Ipred, taille, strategie);
        }
    }

    return Ipred;
 }

void ReversepredictZone(int ligne, int colonne, IplImage * I, IplImage *&Ipred, int taille, int** &strategie)
{
     //prediction du pixel DC de l'image
    ReversepredictPixel(ligne, colonne, I, Ipred, taille, strategie);

    //On fait la prédiction sur la ligne haute des AD
    for(int k = 1; k < taille; k++)
    {
        ReversepredictPixel(ligne, colonne + k, I, Ipred, taille, strategie);
    }

    //On fait la prédiction sur la ligne gauche des AD
    for(int k = 1; k < taille; k++)
    {
        ReversepredictPixel(ligne + k, colonne, I, Ipred, taille, strategie);
    }
}

void ReversepredictPixel(int ligne, int colonne, IplImage * I, IplImage *&Ipred, int taille, int** &strategie)
{
	//Pixel pour l'image a déprédire, et pour l'image déprédite
    CvScalar pxITop, pxILeft, pxI, pxIpred;
    //bool choixTop = true, choixLeft = true;

    //On récupère la valeur du pixel à déprédire
    pxI = cvGet2D( I, ligne, colonne );

    //On vérifie qu'on n'est pas sur une zone de bord haut
    if(ligne - taille < 0){
        pxITop.val[0] = pxI.val[0];
    }
    else
        pxITop = cvGet2D( Ipred, ligne - taille, colonne);

    //On vérifie qu'on n'est pas sur une zone de bord gauche
    if(colonne - taille < 0){
        pxILeft.val[0] = pxI.val[0];
    }
    else
        pxILeft = cvGet2D( Ipred, ligne, colonne - taille);

    //On applique la meilleur stratégie pour le pixel
    if ( strategie[colonne][ligne] == 1){
        pxIpred.val[0] = pxI.val[0] + pxITop.val[0];
    }
    else if (strategie[colonne][ligne] == 2)
    {
        pxIpred.val[0] = pxI.val[0] + pxILeft.val[0];
    }
    else
		pxIpred.val[0] = pxI.val[0] ;
    /*if(pxI.val[0] - pxITop.val[0] < pxI.val[0] - pxILeft.val[0]){
        pxBest.val[0] = pxI.val[0] - pxITop.val[0];
        strategie[ligne][colonne] = 1;
    }
    else if (pxI.val[0] - pxITop.val[0] > pxI.val[0] - pxILeft.val[0]){
        pxBest.val[0] = pxI.val[0] - pxILeft.val[0];
        strategie[ligne][colonne] = 2;
    }*/


    cvSet2D( Ipred, ligne, colonne, pxIpred);

}
