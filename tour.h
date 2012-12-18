/*****************************************************************************************
 En-tete du programme
 =====================
 fichier : tour.h
 Auteur : Alexandre Beaulieu et Vincent Philippon
 Date de creation : 30 avril 2010
 Fichier d'ent�te de l'objet tour
 ******************************************************************************************/

//Directive au pr�-processeur
#ifndef TOUR_H_
#define TOUR_H_
#include "piece.h"

class tour: public piece
{
  public:
    //M�thodes
    tour(int equipe = 0, SDL_Surface *image = NULL, bool bouger = false) :
        piece(equipe, image, bouger)
    {
    }  //Constructeur

    tour(piece &p) :
        piece(p)
    {
    }  //Constructeur de copy

    ~tour();  //Destucteur

    bool destination(plateau sTabPlateau[8][8], int iXPiece, int iYPiece,
                     int iXDeplacement, int iYDeplacement, int iTour);  //S�lection de la destination
    char type() const;  //Retourne le type de la pi�ce
};

#endif
