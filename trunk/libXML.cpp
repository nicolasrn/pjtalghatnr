#include "libXML.h"
#include "tinystr.h"
#include "tinyxml.h"
#include <vector>

using namespace std;

/**
 *@param name: Nom du Fichier qu'on lit
 *@param Y: image quantifié pour Y
 *@param U: image quantifié pour U
 *@param V : image quantifié pour V
 *@param stratY : stratégie pour Y
 *@param stratU : stratégie pour U
 *@param stratV : stratégie pour V
 */
void loadXMLOriginal(const char* name, std::vector<IplImage *> &Y, std::vector<IplImage *> &U, std::vector<IplImage *> &V , std::vector<int **> &stratY, std::vector<int **> &stratU, std::vector<int **> &stratV){

    //On ouvrir le fichier "name"
    TiXmlDocument doc(name);

    TiXmlElement * originalImage;
    TiXmlElement * bigOne;
    TiXmlElement * ligne;
    TiXmlElement * colonne;
    TiXmlElement * littleOne;
    TiXmlElement * recoveryImage;
    int width;
    int height;
    int nbLigne;
    int nbColonne;
    int attrStratY;
    int attrStratU;
    int attrStratV;
    CvScalar attrY;
    CvScalar attrU;
    CvScalar attrV;

    bool loadOkay = doc.LoadFile();
    //Si on a pas réussi à charger le fichier
    if (!loadOkay)
    {
         printf("Failed to load file \"%s\"\n", name);
         exit(1);
    }
    //Si on a réussi à charger le fichier

    //On crée un Handle
    TiXmlHandle docHandle( &doc );

    //On accède au premier élément, cad la première balise appelée "OriginalImage"
    originalImage = docHandle.FirstChildElement().Element();//docHandle.FirstChildElement().FirstChildElement().Element();
    //On test la valeur de originalImage
    if(!originalImage){
        printf("Failed to read file. Can't reach '<OriginalImage>' \n");
        exit(1);
    }


    /*
     * PREMIERE PARTIE : OriginalImage -> BigOne
     */

    bigOne = originalImage->FirstChildElement("BigOne");
    //On test la valeur de bigOne
    if(!bigOne){
        printf("Failed to read file. Can't reach '<BigOne>' \n");
        exit(1);
    }
    bigOne->QueryIntAttribute("width", &width);
    bigOne->QueryIntAttribute("height", &height);

    //On crée les images Y, U, V pour BigOne
    IplImage *iplY0 = cvCreateImage(cvSize(width, height), IPL_DEPTH_64F, 1);
    IplImage *iplU0 = cvCreateImage(cvSize(width, height), IPL_DEPTH_64F, 1);
    IplImage *iplV0 = cvCreateImage(cvSize(width, height), IPL_DEPTH_64F, 1);

    //On associe aux vecteurs les images correspondantes
    Y.push_back(iplY0);
    U.push_back(iplU0);
    V.push_back(iplV0);

    //On crée les stratégies strategieY, strategieU, strategieV pour BigOne
    int **strategieY0 = new int*[width];
        for(int i = 0; i < width; i++)
            strategieY0[i] = new int[height];
    int **strategieU0 = new int*[width];
        for(int i = 0; i < width; i++)
            strategieU0[i] = new int[height];
    int **strategieV0 = new int*[width];
        for(int i = 0; i < width; i++)
            strategieV0[i] = new int[height];

    //On associe aux vecteurs les stratégies correspondantes
    stratY.push_back(strategieY0);
    stratU.push_back(strategieU0);
    stratV.push_back(strategieV0);

    ligne = bigOne->FirstChildElement("Ligne");
    nbLigne = 0;
    while(ligne){
        colonne = ligne->FirstChildElement("Colonne");
        nbColonne = 0;
        while(colonne){
            colonne->QueryDoubleAttribute("Y", &attrY.val[0]);
            colonne->QueryDoubleAttribute("U", &attrU.val[0]);
            colonne->QueryDoubleAttribute("V", &attrV.val[0]);
            colonne->QueryIntAttribute("stratY", &attrStratY);
            colonne->QueryIntAttribute("stratU", &attrStratU);
            colonne->QueryIntAttribute("stratV", &attrStratV);

            //On affecte aux images les valeurs de pixel
            cvSet2D( Y[0], nbLigne, nbColonne, attrY);
            cvSet2D( U[0], nbLigne, nbColonne, attrU);
            cvSet2D( V[0], nbLigne, nbColonne, attrV);

            //On affect aux stratégies les valeurs des stratégies
            stratY[0][nbLigne][nbColonne]=attrStratY;
            stratU[0][nbLigne][nbColonne]=attrStratU;
            stratV[0][nbLigne][nbColonne]=attrStratV;

            colonne = colonne->NextSiblingElement();
            nbColonne++;
        }
        ligne = ligne->NextSiblingElement();
        nbLigne++;
    }//fin de while ligne


    /*
     * DEUXIEME PARTIE : OriginalImage -> LittleOne
     */
    littleOne = bigOne->NextSiblingElement();
    //On test la valeur de bigOne
    if(!littleOne){
        printf("Failed to read file. Can't reach '<littleOne>' \n");
        exit(1);
    }
    littleOne->QueryIntAttribute("width", &width);
    littleOne->QueryIntAttribute("height", &height);

    //On crée les images Y, U, V pour BigOne
    IplImage *iplY1 = cvCreateImage(cvSize(width, height), IPL_DEPTH_64F, 1);
    IplImage *iplU1 = cvCreateImage(cvSize(width, height), IPL_DEPTH_64F, 1);
    IplImage *iplV1 = cvCreateImage(cvSize(width, height), IPL_DEPTH_64F, 1);

    //On associe aux vecteurs les images correspondantes
    Y.push_back(iplY1);
    U.push_back(iplU1);
    V.push_back(iplV1);

    //On crée les stratégies strategieY, strategieU, strategieV pour BigOne
    int **strategieY1 = new int*[width];
        for(int i = 0; i < width; i++)
            strategieY1[i] = new int[height];
    int **strategieU1 = new int*[width];
        for(int i = 0; i < width; i++)
            strategieU1[i] = new int[height];
    int **strategieV1 = new int*[width];
        for(int i = 0; i < width; i++)
            strategieV1[i] = new int[height];

    //On associe aux vecteurs les stratégies correspondantes
    stratY.push_back(strategieY1);
    stratU.push_back(strategieU1);
    stratV.push_back(strategieV1);

    ligne = littleOne->FirstChildElement("Ligne");
    nbLigne = 0;
    while(ligne){
        colonne = ligne->FirstChildElement("Colonne");
        nbColonne = 0;
        while(colonne){
            colonne->QueryDoubleAttribute("Y", &attrY.val[0]);
            colonne->QueryDoubleAttribute("U", &attrU.val[0]);
            colonne->QueryDoubleAttribute("V", &attrV.val[0]);
            colonne->QueryIntAttribute("stratY", &attrStratY);
            colonne->QueryIntAttribute("stratU", &attrStratU);
            colonne->QueryIntAttribute("stratV", &attrStratV);

            //On affecte aux images les valeurs de pixel
            cvSet2D( Y[1], nbLigne, nbColonne, attrY);
            cvSet2D( U[1], nbLigne, nbColonne, attrU);
            cvSet2D( V[1], nbLigne, nbColonne, attrV);

            //On affect aux stratégies les valeurs des stratégies
            stratY[1][nbLigne][nbColonne]=attrStratY;
            stratU[1][nbLigne][nbColonne]=attrStratU;
            stratV[1][nbLigne][nbColonne]=attrStratV;

            colonne = colonne->NextSiblingElement();
            nbColonne++;
        }
        ligne = ligne->NextSiblingElement();
        nbLigne++;
    }


     //On accède a la balise appelée "RecoveryImage"
    recoveryImage = originalImage->NextSiblingElement();//docHandle.FirstChildElement("RecoveryImage").Element();
    //On test la valeur de recoveryImage
    if(!recoveryImage){
        printf("Failed to read file. Can't reach '<RecoveryImage>' \n");
        exit(1);
    }

    /*
     * TROISIEME PARTIE : RecoveryImage -> BigOne
     */

    bigOne = recoveryImage->FirstChildElement("BigOne");
    //On test la valeur de bigOne
    if(!bigOne){
        printf("Failed to read file. Can't reach '<BigOne>' \n");
        exit(1);
    }
    bigOne->QueryIntAttribute("width", &width);
    bigOne->QueryIntAttribute("height", &height);

    //On crée les images Y, U, V pour BigOne
    IplImage *iplY2 = cvCreateImage(cvSize(width, height), IPL_DEPTH_64F, 1);
    IplImage *iplU2 = cvCreateImage(cvSize(width, height), IPL_DEPTH_64F, 1);
    IplImage *iplV2 = cvCreateImage(cvSize(width, height), IPL_DEPTH_64F, 1);

    //On associe aux vecteurs les images correspondantes
    Y.push_back(iplY2);
    U.push_back(iplU2);
    V.push_back(iplV2);

    //On crée les stratégies strategieY, strategieU, strategieV pour BigOne
    int **strategieY2 = new int*[width];
        for(int i = 0; i < width; i++)
            strategieY2[i] = new int[height];
    int **strategieU2 = new int*[width];
        for(int i = 0; i < width; i++)
            strategieU2[i] = new int[height];
    int **strategieV2 = new int*[width];
        for(int i = 0; i < width; i++)
            strategieV2[i] = new int[height];

    //On associe aux vecteurs les stratégies correspondantes
    stratY.push_back(strategieY2);
    stratU.push_back(strategieU2);
    stratV.push_back(strategieV2);

    ligne = bigOne->FirstChildElement("Ligne");
    nbLigne = 0;
    while(ligne){
        colonne = ligne->FirstChildElement("Colonne");
        nbColonne = 0;
        while(colonne){
            colonne->QueryDoubleAttribute("Y", &attrY.val[0]);
            colonne->QueryDoubleAttribute("U", &attrU.val[0]);
            colonne->QueryDoubleAttribute("V", &attrV.val[0]);
            colonne->QueryIntAttribute("stratY", &attrStratY);
            colonne->QueryIntAttribute("stratU", &attrStratU);
            colonne->QueryIntAttribute("stratV", &attrStratV);

            //On affecte aux images les valeurs de pixel
            cvSet2D( Y[2], nbLigne, nbColonne, attrY);
            cvSet2D( U[2], nbLigne, nbColonne, attrU);
            cvSet2D( V[2], nbLigne, nbColonne, attrV);

            //On affect aux stratégies les valeurs des stratégies
            stratY[2][nbLigne][nbColonne]=attrStratY;
            stratU[2][nbLigne][nbColonne]=attrStratU;
            stratV[2][nbLigne][nbColonne]=attrStratV;

            colonne = colonne->NextSiblingElement();
            nbColonne++;
        }
        ligne = ligne->NextSiblingElement();
        nbLigne++;
    }


    /*
     * QUATRIEME PARTIE : RecoveryImage -> LittleOne
     */

    littleOne = bigOne->NextSiblingElement();
    //On test la valeur de bigOne
    if(!littleOne){
        printf("Failed to read file. Can't reach '<littleOne>' \n");
        exit(1);
    }
    littleOne->QueryIntAttribute("width", &width);
    littleOne->QueryIntAttribute("height", &height);

    //On crée les images Y, U, V pour BigOne
    IplImage *iplY3 = cvCreateImage(cvSize(width, height), IPL_DEPTH_64F, 1);
    IplImage *iplU3 = cvCreateImage(cvSize(width, height), IPL_DEPTH_64F, 1);
    IplImage *iplV3 = cvCreateImage(cvSize(width, height), IPL_DEPTH_64F, 1);

    //On associe aux vecteurs les images correspondantes
    Y.push_back(iplY3);
    U.push_back(iplU3);
    V.push_back(iplV3);

    //On crée les stratégies strategieY, strategieU, strategieV pour BigOne
    int **strategieY3 = new int*[width];
        for(int i = 0; i < width; i++)
            strategieY3[i] = new int[height];
    int **strategieU3 = new int*[width];
        for(int i = 0; i < width; i++)
            strategieU3[i] = new int[height];
    int **strategieV3 = new int*[width];
        for(int i = 0; i < width; i++)
            strategieV3[i] = new int[height];

    //On associe aux vecteurs les stratégies correspondantes
    stratY.push_back(strategieY3);
    stratU.push_back(strategieU3);
    stratV.push_back(strategieV3);

    ligne = littleOne->FirstChildElement("Ligne");
    nbLigne = 0;
    while(ligne){
        colonne = ligne->FirstChildElement("Colonne");
        nbColonne = 0;
        while(colonne){
            colonne->QueryDoubleAttribute("Y", &attrY.val[0]);
            colonne->QueryDoubleAttribute("U", &attrU.val[0]);
            colonne->QueryDoubleAttribute("V", &attrV.val[0]);
            colonne->QueryIntAttribute("stratY", &attrStratY);
            colonne->QueryIntAttribute("stratU", &attrStratU);
            colonne->QueryIntAttribute("stratV", &attrStratV);

            //On affecte aux images les valeurs de pixel
            cvSet2D( Y[3], nbLigne, nbColonne, attrY);
            cvSet2D( U[3], nbLigne, nbColonne, attrU);
            cvSet2D( V[3], nbLigne, nbColonne, attrV);

            //On affect aux stratégies les valeurs des stratégies
            stratY[3][nbLigne][nbColonne]=attrStratY;
            stratU[3][nbLigne][nbColonne]=attrStratU;
            stratV[3][nbLigne][nbColonne]=attrStratV;

            colonne = colonne->NextSiblingElement();
            nbColonne++;
        }
        ligne = ligne->NextSiblingElement();
        nbLigne++;
    }
}



void saveXML(const char * name, std::vector<IplImage *> Y, std::vector<IplImage *> U, std::vector<IplImage *> V , std::vector<int **> stratY, std::vector<int **> stratU, std::vector<int **> stratV){

    TiXmlDocument doc;
    TiXmlElement * Ligne;
    TiXmlElement * Colonne;

    //Root du fichier XML pour la balise <OriginalImage>
    TiXmlElement * OriginalImage = new TiXmlElement("OriginalImage");
    doc.LinkEndChild(OriginalImage);

    //Balise BigOne pour l'image normal <BigOne width=XX height=XX>
    TiXmlElement * BigOneOriginal = new TiXmlElement("BigOne");
    OriginalImage->LinkEndChild(BigOneOriginal);
    BigOneOriginal->SetAttribute("width", Y[0]->width);
    BigOneOriginal->SetAttribute("height", Y[0]->height);

    for(int h = 0; h < Y[0]->height; h++){
        //On ajoute une ligne à l'image <Ligne>
        Ligne = new TiXmlElement("Ligne");
        BigOneOriginal->LinkEndChild(Ligne);

        for(int w = 0; w < Y[0]->width; w++){
            //On ajoute une Colonne à la Ligne <Colonne Y=XX U=XX V=XX StratY=XX StratU=XX StratV=XX>
            Colonne = new TiXmlElement("Colonne");
            Ligne->LinkEndChild(Colonne);
            Colonne->SetAttribute("Y", cvGet2D( Y[0], h, w ).val[0]);
            Colonne->SetAttribute("U", cvGet2D( U[0], h, w ).val[0]);
            Colonne->SetAttribute("V", cvGet2D( V[0], h, w ).val[0]);
            Colonne->SetAttribute("StratY", stratY[0][h][w]);
            Colonne->SetAttribute("StratU", stratU[0][h][w]);
            Colonne->SetAttribute("StratV", stratV[0][h][w]);
        }
    }

    //Balise LittleOne pour l'image normal <LittleOne width=XX height=XX>
    TiXmlElement * LittleOneOriginal = new TiXmlElement("LittleOne");
    OriginalImage->LinkEndChild(LittleOneOriginal);
    LittleOneOriginal->SetAttribute("width", Y[1]->width);
    LittleOneOriginal->SetAttribute("height", Y[1]->height);

    for(int h = 0; h < Y[1]->height; h++){
        //On ajoute une ligne à l'image <Ligne>
        Ligne = new TiXmlElement("Ligne");
        LittleOneOriginal->LinkEndChild(Ligne);

        for(int w = 0; w < Y[1]->width; w++){
            //On ajoute une Colonne à la Ligne <Colonne Y=XX U=XX V=XX StratY=XX StratU=XX StratV=XX>
            Colonne = new TiXmlElement("Colonne");
            Ligne->LinkEndChild(Colonne);
            Colonne->SetAttribute("Y", cvGet2D( Y[1], h, w ).val[0]);
            Colonne->SetAttribute("U", cvGet2D( U[1], h, w ).val[0]);
            Colonne->SetAttribute("V", cvGet2D( V[1], h, w ).val[0]);
            Colonne->SetAttribute("StratY", stratY[1][h][w]);
            Colonne->SetAttribute("StratU", stratU[1][h][w]);
            Colonne->SetAttribute("StratV", stratV[1][h][w]);
        }
    }

    //Root du fichier XML pour la balise <RecoveryImage>
    TiXmlElement * RecoveryImage = new TiXmlElement("RecoveryImage");
    doc.LinkEndChild(RecoveryImage);

    //Balise BigOne pour l'image de recouvrement <BigOne width=XX height=XX>
    TiXmlElement * BigOneRecovery = new TiXmlElement("BigOne");
    RecoveryImage->LinkEndChild(BigOneRecovery);
    BigOneRecovery->SetAttribute("width", Y[2]->width);
    BigOneRecovery->SetAttribute("height", Y[2]->height);

    for(int h = 0; h < Y[2]->height; h++){
        //On ajoute une ligne à l'image <Ligne>
        Ligne = new TiXmlElement("Ligne");
        BigOneRecovery->LinkEndChild(Ligne);

        for(int w = 0; w < Y[2]->width; w++){
            //On ajoute une Colonne à la Ligne <Colonne Y=XX U=XX V=XX StratY=XX StratU=XX StratV=XX>
            Colonne = new TiXmlElement("Colonne");
            Ligne->LinkEndChild(Colonne);
            Colonne->SetAttribute("Y", cvGet2D( Y[2], h, w ).val[0]);
            Colonne->SetAttribute("U", cvGet2D( U[2], h, w ).val[0]);
            Colonne->SetAttribute("V", cvGet2D( V[2], h, w ).val[0]);
            Colonne->SetAttribute("StratY", stratY[2][h][w]);
            Colonne->SetAttribute("StratU", stratU[2][h][w]);
            Colonne->SetAttribute("StratV", stratV[2][h][w]);
        }
    }

    //Balise LittleOne pour l'image de recouvrement <LittleOne width=XX height=XX>
    TiXmlElement * LittleOneRecovery = new TiXmlElement("LittleOne");
    RecoveryImage->LinkEndChild(LittleOneRecovery);
    LittleOneRecovery->SetAttribute("width", Y[3]->width);
    LittleOneRecovery->SetAttribute("height", Y[3]->height);

    for(int h = 0; h < Y[3]->height; h++){
        //On ajoute une ligne à l'image <Ligne>
        Ligne = new TiXmlElement("Ligne");
        LittleOneRecovery->LinkEndChild(Ligne);

        for(int w = 0; w < Y[3]->width; w++){
            //On ajoute une Colonne à la Ligne <Colonne Y=XX U=XX V=XX StratY=XX StratU=XX StratV=XX>
            Colonne = new TiXmlElement("Colonne");
            Ligne->LinkEndChild(Colonne);
            Colonne->SetAttribute("Y", cvGet2D( Y[3], h, w ).val[0]);
            Colonne->SetAttribute("U", cvGet2D( U[3], h, w ).val[0]);
            Colonne->SetAttribute("V", cvGet2D( V[3], h, w ).val[0]);
            Colonne->SetAttribute("StratY", stratY[3][h][w]);
            Colonne->SetAttribute("StratU", stratU[3][h][w]);
            Colonne->SetAttribute("StratV", stratV[3][h][w]);
        }
    }

    doc.SaveFile(name);
}

