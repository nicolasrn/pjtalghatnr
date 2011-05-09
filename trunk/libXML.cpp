#include "libXML.h"
#include "tinystr.h"
#include "tinyxml.h"
#include <vector>

using namespace std;

/**
 *@param name: Nom du Fichier qu'on enregistre
 *@param Y: quantification pour Y
 *@param U: quantification pour U
 *@param V : quantification pour V
 *@param stratY : stratégie pour Y
 *@param stratU : stratégie pour U
 *@param stratV : stratégie pour V
 *@param heightVector : hauteur pour chaque partie enregistrée
 *@param widthVector : largeur pour chaque partie enregistrée
 */
void saveXML(const char * name, std::vector<int **> Y, std::vector<int **> U, std::vector<int **> V , std::vector<int **> stratY, std::vector<int **> stratU, std::vector<int **> stratV, std::vector<int> heightVector, std::vector<int> widthVector, int QP){

    TiXmlDocument doc;
    TiXmlElement * Ligne;
    TiXmlElement * Colonne;

    //Root du fichier XML pour la balise <Recouvrement>
    TiXmlElement * Parametre = new TiXmlElement("Parametre");
    Parametre->SetAttribute("QP", QP);

    if(Y.size() == 2)
        Parametre->SetAttribute("recouvrement", "0");
    else
        Parametre->SetAttribute("recouvrement", "1");

    doc.LinkEndChild(Parametre);

    //Root du fichier XML pour la balise <OriginalImage>
    TiXmlElement * OriginalImage = new TiXmlElement("OriginalImage");
    doc.LinkEndChild(OriginalImage);

    //Balise BigOne pour l'image normal <BigOne width=XX height=XX>
    TiXmlElement * BigOneOriginal = new TiXmlElement("BigOne");
    OriginalImage->LinkEndChild(BigOneOriginal);
    BigOneOriginal->SetAttribute("width", widthVector[0]);
    BigOneOriginal->SetAttribute("height", heightVector[0]);

    for(int h = 0; h < heightVector[0]; h++){
        //On ajoute une ligne à l'image <Ligne>
        Ligne = new TiXmlElement("Ligne");
        BigOneOriginal->LinkEndChild(Ligne);

        for(int w = 0; w < widthVector[0]; w++){
            //On ajoute une Colonne à la Ligne <Colonne Y=XX U=XX V=XX StratY=XX StratU=XX StratV=XX>
            Colonne = new TiXmlElement("Colonne");
            Ligne->LinkEndChild(Colonne);
            Colonne->SetAttribute("Y", Y[0][w][h]);
            Colonne->SetAttribute("U", U[0][w][h]);
            Colonne->SetAttribute("V", V[0][w][h]);
            Colonne->SetAttribute("StratY", stratY[0][w][h]);
            Colonne->SetAttribute("StratU", stratU[0][w][h]);
            Colonne->SetAttribute("StratV", stratV[0][w][h]);
        }
    }

    //Balise LittleOne pour l'image normal <LittleOne width=XX height=XX>
    TiXmlElement * LittleOneOriginal = new TiXmlElement("LittleOne");
    OriginalImage->LinkEndChild(LittleOneOriginal);
    LittleOneOriginal->SetAttribute("width", widthVector[1]);
    LittleOneOriginal->SetAttribute("height", heightVector[1]);

    for(int h = 0; h < heightVector[1]; h++){
        //On ajoute une ligne à l'image <Ligne>
        Ligne = new TiXmlElement("Ligne");
        LittleOneOriginal->LinkEndChild(Ligne);

        for(int w = 0; w < widthVector[1]; w++){
            //On ajoute une Colonne à la Ligne <Colonne Y=XX U=XX V=XX StratY=XX StratU=XX StratV=XX>
            Colonne = new TiXmlElement("Colonne");
            Ligne->LinkEndChild(Colonne);
            Colonne->SetAttribute("Y", Y[1][w][h]);
            Colonne->SetAttribute("U", U[1][w][h]);
            Colonne->SetAttribute("V", V[1][w][h]);
            Colonne->SetAttribute("StratY", stratY[1][w][h]);
            Colonne->SetAttribute("StratU", stratU[1][w][h]);
            Colonne->SetAttribute("StratV", stratV[1][w][h]);
        }
    }


    if(Y.size() == 4){
        //Root du fichier XML pour la balise <RecoveryImage>
        TiXmlElement * RecoveryImage = new TiXmlElement("RecoveryImage");
        doc.LinkEndChild(RecoveryImage);

        //Balise BigOne pour l'image de recouvrement <BigOne width=XX height=XX>
        TiXmlElement * BigOneRecovery = new TiXmlElement("BigOne");
        RecoveryImage->LinkEndChild(BigOneRecovery);
        BigOneRecovery->SetAttribute("width", widthVector[2]);
        BigOneRecovery->SetAttribute("height", heightVector[2]);

        for(int h = 0; h < heightVector[2]; h++){
            //On ajoute une ligne à l'image <Ligne>
            Ligne = new TiXmlElement("Ligne");
            BigOneRecovery->LinkEndChild(Ligne);

            for(int w = 0; w < widthVector[2]; w++){
                //On ajoute une Colonne à la Ligne <Colonne Y=XX U=XX V=XX StratY=XX StratU=XX StratV=XX>
                Colonne = new TiXmlElement("Colonne");
                Ligne->LinkEndChild(Colonne);
                Colonne->SetAttribute("Y", Y[2][w][h]);
                Colonne->SetAttribute("U", U[2][w][h]);
                Colonne->SetAttribute("V", V[2][w][h]);
                Colonne->SetAttribute("StratY", stratY[2][w][h]);
                Colonne->SetAttribute("StratU", stratU[2][w][h]);
                Colonne->SetAttribute("StratV", stratV[2][w][h]);
            }
        }

        //Balise LittleOne pour l'image de recouvrement <LittleOne width=XX height=XX>
        TiXmlElement * LittleOneRecovery = new TiXmlElement("LittleOne");
        RecoveryImage->LinkEndChild(LittleOneRecovery);
        LittleOneRecovery->SetAttribute("width", widthVector[3]);
        LittleOneRecovery->SetAttribute("height", heightVector[3]);

        for(int h = 0; h < heightVector[3]; h++){
            //On ajoute une ligne à l'image <Ligne>
            Ligne = new TiXmlElement("Ligne");
            LittleOneRecovery->LinkEndChild(Ligne);

            for(int w = 0; w < widthVector[3]; w++){
                //On ajoute une Colonne à la Ligne <Colonne Y=XX U=XX V=XX StratY=XX StratU=XX StratV=XX>
                Colonne = new TiXmlElement("Colonne");
                Ligne->LinkEndChild(Colonne);
                Colonne->SetAttribute("Y", Y[3][w][h]);
                Colonne->SetAttribute("U", U[3][w][h]);
                Colonne->SetAttribute("V", V[3][w][h]);
                Colonne->SetAttribute("StratY", stratY[3][w][h]);
                Colonne->SetAttribute("StratU", stratU[3][w][h]);
                Colonne->SetAttribute("StratV", stratV[3][w][h]);

            }
        }
    }

    doc.SaveFile(name);
}

/**
 *@param name: Nom du Fichier qu'on charge
 *@param Y: quantification pour Y
 *@param U: quantification pour U
 *@param V : quantification pour V
 *@param stratY : stratégie pour Y
 *@param stratU : stratégie pour U
 *@param stratV : stratégie pour V
 *@param heightVector : hauteur pour chaque partie enregistrée
 *@param widthVector : largeur pour chaque partie enregistrée
 */
void loadXML(const char* name, std::vector<int **> &Y, std::vector<int **> &U, std::vector<int **> &V , std::vector<int **> &stratY, std::vector<int **> &stratU, std::vector<int **> &stratV, std::vector<int> &heightVector, std::vector<int> &widthVector, int &QP){

    //On ouvrir le fichier "name"
    TiXmlDocument doc(name);

    TiXmlElement * parametre;
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
    int attrRecouvert;
    int attrStratY;
    int attrStratU;
    int attrStratV;
    int attrY;
    int attrU;
    int attrV;

    bool loadOkay = doc.LoadFile();
    //Si on a pas réussi à charger le fichier
    if (!loadOkay)
    {
         printf("Failed to load file \"%s\"\n", name);
         std::cout << "Error #" << doc.ErrorId() << ": " << doc.ErrorDesc() << endl;
         exit(1);
    }
    //Si on a réussi à charger le fichier

    //On crée un Handle
    TiXmlHandle docHandle( &doc );

    //On accède au premier élément, cad la première balise appelée "Recouvrement"
    parametre = docHandle.FirstChildElement().Element();
    if(!parametre){
        printf("Failed to read file. Can't reach '<Parametre>' \n");
        exit(1);
    }
    parametre->QueryIntAttribute("QP",&QP);
    parametre->QueryIntAttribute("recouvrement",&attrRecouvert);

    //On accède au premier élément, cad la première balise appelée "OriginalImage"
    originalImage =  parametre->NextSiblingElement();
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
        printf("Failed to read file. Can't reach First '<BigOne>' \n");
        exit(1);
    }

    bigOne->QueryIntAttribute("height", &height);
    bigOne->QueryIntAttribute("width", &width);
    heightVector.push_back(height);
    widthVector.push_back(width);

    //On crée les tableau de quantification Y, U, V pour BigOne
    int **Y0 = new int*[width];
        for(int i = 0; i < width; i++)
            Y0[i] = new int[height];

    int **U0 = new int*[width];
        for(int i = 0; i < width; i++)
            U0[i] = new int[height];

    int **V0 = new int*[width];
        for(int i = 0; i < width; i++)
            V0[i] = new int[height];

    //On associe aux vecteurs les images correspondantes
    Y.push_back(Y0);
    U.push_back(U0);
    V.push_back(V0);

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

            colonne->QueryIntAttribute("Y", &attrY);
            colonne->QueryIntAttribute("U", &attrU);
            colonne->QueryIntAttribute("V", &attrV);
            colonne->QueryIntAttribute("StratY", &attrStratY);
            colonne->QueryIntAttribute("StratU", &attrStratU);
            colonne->QueryIntAttribute("StratV", &attrStratV);

            //On affecte aux images les valeurs de pixel
            Y[0][nbColonne][nbLigne] = attrY;
            U[0][nbColonne][nbLigne] = attrU;
            V[0][nbColonne][nbLigne] = attrV;

            //On affect aux stratégies les valeurs des stratégies
            stratY[0][nbColonne][nbLigne] = attrStratY;
            stratU[0][nbColonne][nbLigne] = attrStratU;
            stratV[0][nbColonne][nbLigne] = attrStratV;

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

    littleOne ->QueryIntAttribute("height", &height);
    littleOne ->QueryIntAttribute("width", &width);
    heightVector.push_back(height);
    widthVector.push_back(width);

    //On crée les tableau de quantification Y, U, V pour LittleOne
    int **Y1 = new int*[width];
        for(int i = 0; i < width; i++)
            Y1[i] = new int[height];

    int **U1 = new int*[width];
        for(int i = 0; i < width; i++)
            U1[i] = new int[height];

    int **V1 = new int*[width];
        for(int i = 0; i < width; i++)
            V1[i] = new int[height];

    //On associe aux vecteurs les images correspondantes
    Y.push_back(Y1);
    U.push_back(U1);
    V.push_back(V1);

    //On crée les stratégies strategieY, strategieU, strategieV pour LittleOne
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
            colonne->QueryIntAttribute("Y", &attrY);
            colonne->QueryIntAttribute("U", &attrU);
            colonne->QueryIntAttribute("V", &attrV);
            colonne->QueryIntAttribute("StratY", &attrStratY);
            colonne->QueryIntAttribute("StratU", &attrStratU);
            colonne->QueryIntAttribute("StratV", &attrStratV);

            //On affecte aux images les valeurs de pixel
            Y[1][nbColonne][nbLigne] = attrY;
            U[1][nbColonne][nbLigne] = attrU;
            V[1][nbColonne][nbLigne] = attrV;

            //On affect aux stratégies les valeurs des stratégies
            stratY[1][nbColonne][nbLigne] = attrStratY;
            stratU[1][nbColonne][nbLigne] = attrStratU;
            stratV[1][nbColonne][nbLigne] = attrStratV;

            colonne = colonne->NextSiblingElement();
            nbColonne++;
        }
        ligne = ligne->NextSiblingElement();
        nbLigne++;
    }

    if(attrRecouvert == 1){

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
            printf("Failed to read file. Can't reach Second '<BigOne>' \n");
            exit(1);
        }

        bigOne->QueryIntAttribute("height", &height);
        bigOne->QueryIntAttribute("width", &width);
        heightVector.push_back(height);
        widthVector.push_back(width);

        //On crée les tableau de quantification Y, U, V pour BigOne
        int **Y2 = new int*[width];
            for(int i = 0; i < width; i++)
                Y2[i] = new int[height];

        int **U2 = new int*[width];
            for(int i = 0; i < width; i++)
                U2[i] = new int[height];

        int **V2 = new int*[width];
            for(int i = 0; i < width; i++)
                V2[i] = new int[height];

        //On associe aux vecteurs les images correspondantes
        Y.push_back(Y2);
        U.push_back(U2);
        V.push_back(V2);

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
                colonne->QueryIntAttribute("Y", &attrY);
                colonne->QueryIntAttribute("U", &attrU);
                colonne->QueryIntAttribute("V", &attrV);
                colonne->QueryIntAttribute("StratY", &attrStratY);
                colonne->QueryIntAttribute("StratU", &attrStratU);
                colonne->QueryIntAttribute("StratV", &attrStratV);

                //On affecte aux images les valeurs de pixel
                Y[2][nbColonne][nbLigne] = attrY;
                U[2][nbColonne][nbLigne] = attrU;
                V[2][nbColonne][nbLigne] = attrV;

                //On affect aux stratégies les valeurs des stratégies
                stratY[2][nbColonne][nbLigne] = attrStratY;
                stratU[2][nbColonne][nbLigne] = attrStratU;
                stratV[2][nbColonne][nbLigne] = attrStratV;

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

        littleOne ->QueryIntAttribute("height", &height);
        littleOne ->QueryIntAttribute("width", &width);
        heightVector.push_back(height);
        widthVector.push_back(width);

       //On crée les tableau de quantification Y, U, V pour LittleOne
        int **Y3 = new int*[width];
            for(int i = 0; i < width; i++)
                Y3[i] = new int[height];

        int **U3 = new int*[width];
            for(int i = 0; i < width; i++)
                U3[i] = new int[height];

        int **V3 = new int*[width];
            for(int i = 0; i < width; i++)
                V3[i] = new int[height];

        //On associe aux vecteurs les images correspondantes
        Y.push_back(Y3);
        U.push_back(U3);
        V.push_back(V3);

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
                colonne->QueryIntAttribute("Y", &attrY);
                colonne->QueryIntAttribute("U", &attrU);
                colonne->QueryIntAttribute("V", &attrV);
                colonne->QueryIntAttribute("StratY", &attrStratY);
                colonne->QueryIntAttribute("StratU", &attrStratU);
                colonne->QueryIntAttribute("StratV", &attrStratV);

                //On affecte aux images les valeurs de pixel
                Y[3][nbColonne][nbLigne] = attrY;
                U[3][nbColonne][nbLigne] = attrU;
                V[3][nbColonne][nbLigne] = attrV;

                //On affect aux stratégies les valeurs des stratégies
                stratY[3][nbColonne][nbLigne] = attrStratY;
                stratU[3][nbColonne][nbLigne] = attrStratU;
                stratV[3][nbColonne][nbLigne] = attrStratV;

                colonne = colonne->NextSiblingElement();
                nbColonne++;
            }
            ligne = ligne->NextSiblingElement();
            nbLigne++;
        }
    }
}
