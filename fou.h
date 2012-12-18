/*****************************************************************************************
 En-tete du programme
 =====================
 fichier : fou.h
 Auteur : Alexandre Beaulieu et Vincent Philippon
 Date de creation : 07 mai 2010
 Fichier d'ent�te de l'objet fou
 ******************************************************************************************/

//Directive au pr�-processeur
#ifndef FOU_H_
#define FOU_H_
#include "piece.h"

class fou: public piece
{
  public:
    //M�thodes
    fou(int equipe = 0, SDL_Surface *image = NULL, bool bouger = false) :
        piece(equipe, image, bouger)
    {
    }  //Constructeur

    fou(piece &p) :
        piece(p)
    {
    }  //Constructeur de copy

    ~fou();  //Destucteur

    bool destination(plateau sTabPlateau[8][8], int iXPiece, int iYPiece,
                     int iXDeplacement, int iYDeplacement, int iTour);  //S�lection de la destination
    char type() const;  //Retourne le type de la pi�ce
};

#endif
