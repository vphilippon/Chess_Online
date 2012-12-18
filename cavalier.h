/*****************************************************************************************
 En-tete du programme
 =====================
 fichier : cavalier.h
 Auteur : Alexandre Beaulieu et Vincent Philippon
 Date de creation : 07 mai 2010
 Fichier d'ent�te de l'objet cavalier
 ******************************************************************************************/

//Directive au pr�-processeur
#ifndef CAVALIER_H_
#define CAVALIER_H_
#include "piece.h"

class cavalier: public piece
{
  public:
    //M�thodes
    cavalier(int equipe = 0, SDL_Surface *image = NULL, bool bouger = false) :
        piece(equipe, image, bouger)
    {
    }  //Constructeur

    cavalier(piece &p) :
        piece(p)
    {
    }  //Constructeur de copy

    ~cavalier();  //Destucteur

    bool destination(plateau sTabPlateau[8][8], int iXPiece, int iYPiece,
                     int iXDeplacement, int iYDeplacement, int iTour);  //S�lection de la destination
    char type() const;  //Retourne le type de la pi�ce
};

#endif
