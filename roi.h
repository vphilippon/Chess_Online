/*****************************************************************************************
 En-tete du programme
 =====================
 fichier : roi.h
 Auteur : Alexandre Beaulieu et Vincent Philippon
 Date de creation : 07 mai 2010
 Fichier d'ent�te de l'objet roi
 ******************************************************************************************/

//Directive au pr�-processeur
#ifndef ROI_H_
#define ROI_H_
#include "piece.h"

class roi: public piece
{
  public:
    //M�thodes
    roi(int equipe = 0, SDL_Surface *image = NULL, bool bouger = false) :
        piece(equipe, image, bouger)
    {
    }  //Constructeur

    roi(piece &p) :
        piece(p)
    {
    }  //Constructeur de copy

    ~roi();  //Destucteur

    bool destination(plateau sTabPlateau[8][8], int iXPiece, int iYPiece,
                     int iXDeplacement, int iYDeplacement, int iTour);  //S�lection de la destination
    char type() const;  //Retourne le type de la pi�ce
};

#endif
