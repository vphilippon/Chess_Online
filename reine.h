/*****************************************************************************************
 En-tete du programme
 =====================
 fichier : reine.h
 Auteur : Alexandre Beaulieu et Vincent Philippon
 Date de creation : 07 mai 2010
 Fichier d'ent�te de l'objet reine
 ******************************************************************************************/

//Directive au pr�-processeur
#ifndef REINE_H_
#define REINE_H_
#include "piece.h"

class reine: public piece
{
  public:
    //M�thodes
    reine(int equipe = 0, SDL_Surface *image = NULL, bool bouger = false) :
        piece(equipe, image, bouger)
    {
    }  //Constructeur

    reine(piece &p) :
        piece(p)
    {
    }  //Constructeur de copy

    ~reine();  //Destucteur

    bool destination(plateau sTabPlateau[8][8], int iXPiece, int iYPiece,
                     int iXDeplacement, int iYDeplacement, int iTour);  //S�lection de la destination
    char type() const;  //Retourne le type de la pi�ce
};

#endif
