#include "libXML.h"
#include "tinystr.h"
#include "tinyxml.h"
#include <vector>

using namespace std;

/**
 *@param index: index dans le vecteur pour sauvegarder l'image
 *@param Balise: Balise mère de l'arborescence de l'image
 *@param heightVector : hauteur pour chaque partie enregistrée
 *@param widthVector : largeur pour chaque partie enregistrée
 */
void saveTailleImage(int index, TiXmlElement * &Balise, std::vector<int> heightVector, std::vector<int> widthVector){
    Balise->SetAttribute("width", widthVector[index]);
    Balise->SetAttribute("height", heightVector[index]);
}

/**
 *@param index: index dans le vecteur pour sauvegarder l'image
 *@param Balise: Balise mère de l'arborescence de l'image
 *@param Y: quantification pour Y
 *@param U: quantification pour U
 *@param V : quantification pour V
 *@param stratY : stratégie pour Y
 *@param stratU : stratégie pour U
 *@param stratV : stratégie pour V
 *@param height : hauteur pour chaque partie enregistrée
 *@param width : largeur pour chaque partie enregistrée
 */
void saveImage(int index, TiXmlElement * &Balise, std::vector<double **> Y, std::vector<double **> U, std::vector<double **> V , std::vector<int **> stratY, std::vector<int **> stratU, std::vector<int **> stratV, int height, int width){

    TiXmlElement * Ligne;
    TiXmlElement * Colonne;

    for(int h = 0; h < height; h++){
        //On ajoute une ligne à l'image <Ligne>
        Ligne = new TiXmlElement("Ligne");
        Balise->LinkEndChild(Ligne);

        for(int w = 0; w < width; w++){
            //printf("(%d, %d)", h, w);
            //On ajoute une Colonne à la Ligne <Colonne Y=XX U=XX V=XX StratY=XX StratU=XX StratV=XX>
            Colonne = new TiXmlElement("Colonne");
            Ligne->LinkEndChild(Colonne);
            Colonne->SetDoubleAttribute("Y", Y[index][h][w]);
            Colonne->SetDoubleAttribute("U", U[index][h][w]);
            Colonne->SetDoubleAttribute("V", V[index][h][w]);
            //cout << "dans la sauvegarde : " << Y[index][h][w] << endl;

            Colonne->SetAttribute("StratY", stratY[index][w][h]);
            Colonne->SetAttribute("StratU", stratU[index][w][h]);
            Colonne->SetAttribute("StratV", stratV[index][w][h]);
        }
    }
}

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
void saveXML(const char * name, std::vector<double **> Y, std::vector<double **> U, std::vector<double **> V , std::vector<int **> stratY, std::vector<int **> stratU, std::vector<int **> stratV, std::vector<int> heightVector, std::vector<int> widthVector, int QP){

    TiXmlDocument doc;

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

    saveTailleImage(0, BigOneOriginal, heightVector, widthVector);

    saveImage(0, BigOneOriginal, Y, U, V, stratY, stratU, stratV, heightVector[0], widthVector[0]);

    //Balise LittleOne pour l'image normal <LittleOne width=XX height=XX>
    TiXmlElement * LittleOneOriginal = new TiXmlElement("LittleOne");
    OriginalImage->LinkEndChild(LittleOneOriginal);

    saveTailleImage(1, LittleOneOriginal, heightVector, widthVector);

    saveImage(1, LittleOneOriginal, Y, U, V, stratY, stratU, stratV, heightVector[1], widthVector[1]);

    if(Y.size() == 4){
        //Root du fichier XML pour la balise <RecoveryImage>
        TiXmlElement * RecoveryImage = new TiXmlElement("RecoveryImage");
        doc.LinkEndChild(RecoveryImage);

        //Balise BigOne pour l'image de recouvrement <BigOne width=XX height=XX>
        TiXmlElement * BigOneRecovery = new TiXmlElement("BigOne");
        RecoveryImage->LinkEndChild(BigOneRecovery);

        saveTailleImage(2, BigOneRecovery, heightVector, widthVector);

        saveImage(2, BigOneRecovery, Y, U, V, stratY, stratU, stratV, heightVector[2], widthVector[2]);

        //Balise LittleOne pour l'image de recouvrement <LittleOne width=XX height=XX>
        TiXmlElement * LittleOneRecovery = new TiXmlElement("LittleOne");
        RecoveryImage->LinkEndChild(LittleOneRecovery);

        saveTailleImage(3, LittleOneRecovery, heightVector, widthVector);

        saveImage(3, LittleOneRecovery, Y, U, V, stratY, stratU, stratV, heightVector[3], widthVector[3]);
    }

    doc.SaveFile(name);
}

/**
 *@param width: largeur à instancier
 *@param height: hauteur à instancier
 */
double ** instanciateTabQuantif(int width, int height){
    double **tab = new double*[height];
        for(int i = 0; i < height; i++)
            tab[i] = new double[width];
    return tab;
}

/**
 *@param width: largeur à instancier
 *@param height: hauteur à instancier
 */
int ** instanciateTabStrat(int width, int height){
    int **tab = new int*[width];
        for(int i = 0; i < width; i++)
            tab[i] = new int[height];
    return tab;
}

/**
 *@param index: index dans le vecteur pour charger l'image
 *@param Balise: Balise mère de l'arborescence de l'image
 *@param Y: quantification pour Y
 *@param U: quantification pour U
 *@param V : quantification pour V
 *@param stratY : stratégie pour Y
 *@param stratU : stratégie pour U
 *@param stratV : stratégie pour V
 */
void chargeImage(int index, TiXmlElement * &Balise, std::vector<double **> Y, std::vector<double **> U, std::vector<double **> V , std::vector<int **> stratY, std::vector<int **> stratU, std::vector<int **> stratV){

    TiXmlElement * ligne, * colonne;
    int nbLigne, nbColonne, attrStratY, attrStratU, attrStratV;
    double attrY, attrU, attrV;

    ligne = Balise->FirstChildElement("Ligne");
    nbLigne = 0;
    while(ligne){
        colonne = ligne->FirstChildElement("Colonne");
        nbColonne = 0;

        while(colonne){

            colonne->QueryDoubleAttribute("Y", &attrY);
            colonne->QueryDoubleAttribute("U", &attrU);
            colonne->QueryDoubleAttribute("V", &attrV);

            colonne->QueryIntAttribute("StratY", &attrStratY);
            colonne->QueryIntAttribute("StratU", &attrStratU);
            colonne->QueryIntAttribute("StratV", &attrStratV);

            //On affecte aux images les valeurs de pixel
            Y[index][nbLigne][nbColonne] = attrY;
            U[index][nbLigne][nbColonne] = attrU;
            V[index][nbLigne][nbColonne] = attrV;

            //cout << "dans le chargement : " << Y[index][nbLigne][nbColonne] << endl;

            //On affect aux stratégies les valeurs des stratégies
            stratY[index][nbColonne][nbLigne] = attrStratY;
            stratU[index][nbColonne][nbLigne] = attrStratU;
            stratV[index][nbColonne][nbLigne] = attrStratV;

            colonne = colonne->NextSiblingElement();
            nbColonne++;
        }
        ligne = ligne->NextSiblingElement();
        nbLigne++;
    }//fin de while ligne
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
void loadXML(const char* name, std::vector<double **> &Y, std::vector<double **> &U, std::vector<double **> &V , std::vector<int **> &stratY, std::vector<int **> &stratU, std::vector<int **> &stratV, std::vector<int> &heightVector, std::vector<int> &widthVector, int &QP){

    //On ouvrir le fichier "name"
    TiXmlDocument doc(name);

    TiXmlElement * parametre, * originalImage, * bigOne, * littleOne, * recoveryImage;
    int width, height, attrRecouvert;

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

    //On instancie dans les vecteurs les tableaux de quantification
    Y.push_back(instanciateTabQuantif(width, height));
    U.push_back(instanciateTabQuantif(width, height));
    V.push_back(instanciateTabQuantif(width, height));

    //On instancie dans les vecteurs les tableaux de stratégie
    stratY.push_back(instanciateTabStrat(width, height));
    stratU.push_back(instanciateTabStrat(width, height));
    stratV.push_back(instanciateTabStrat(width, height));

    chargeImage(0, bigOne, Y, U, V , stratY, stratU, stratV);

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

    //On instancie dans les vecteurs les tableaux de quantification
    Y.push_back(instanciateTabQuantif(width, height));
    U.push_back(instanciateTabQuantif(width, height));
    V.push_back(instanciateTabQuantif(width, height));

    //On instancie dans les vecteurs les tableaux de stratégie
    stratY.push_back(instanciateTabStrat(width, height));
    stratU.push_back(instanciateTabStrat(width, height));
    stratV.push_back(instanciateTabStrat(width, height));

    chargeImage(1, littleOne, Y, U, V , stratY, stratU, stratV);

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

        //On instancie dans les vecteurs les tableaux de quantification
        Y.push_back(instanciateTabQuantif(width, height));
        U.push_back(instanciateTabQuantif(width, height));
        V.push_back(instanciateTabQuantif(width, height));

        //On instancie dans les vecteurs les tableaux de stratégie
        stratY.push_back(instanciateTabStrat(width, height));
        stratU.push_back(instanciateTabStrat(width, height));
        stratV.push_back(instanciateTabStrat(width, height));

        chargeImage(2, bigOne, Y, U, V , stratY, stratU, stratV);

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

       //On instancie dans les vecteurs les tableaux de quantification
        Y.push_back(instanciateTabQuantif(width, height));
        U.push_back(instanciateTabQuantif(width, height));
        V.push_back(instanciateTabQuantif(width, height));

        //On instancie dans les vecteurs les tableaux de stratégie
        stratY.push_back(instanciateTabStrat(width, height));
        stratU.push_back(instanciateTabStrat(width, height));
        stratV.push_back(instanciateTabStrat(width, height));

        chargeImage(3, littleOne, Y, U, V , stratY, stratU, stratV);
    }
}
