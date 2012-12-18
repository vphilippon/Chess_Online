/*****************************************************************************************
 En-tete du programme
 =====================
 fichier : pion.h
 Auteur : Alexandre Beaulieu et Vincent Philippon
 Date de creation : 07 mai 2010
 Fichier d'ent�te de l'objet pion
 ******************************************************************************************/

//Directive au pr�-processeur
#ifndef PION_H_
#define PION_H_
#include "piece.h"

class pion: public piece
{
  public:
    //M�thodes
    pion(int equipe = 0, SDL_Surface *image = NULL, bool bouger = false) :
        piece(equipe, image, bouger)
    {
    }  //Constructeur

    pion(piece &p) :
        piece(p)
    {
    }  //Constructeur de copy

    ~pion();  //Destucteur

    bool destination(plateau sTabPlateau[8][8], int iXPiece, int iYPiece,
                     int iXDeplacement, int iYDeplacement, int iTour);  //S�lection de la destination
    char type() const;  //Retourne le type de la pi�ce
};

#endif
