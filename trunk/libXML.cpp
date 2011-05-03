#include "libXML.h"

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
