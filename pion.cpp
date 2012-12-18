/*****************************************************************************************
 En-tete du programme
 =====================
 Programme : pion.cpp
 Auteur : Alexandre Beaulieu et Vincent Philippon
 Date de creation : 07 mai 2010
 But du programme : M�thode de l'objet pion
 ******************************************************************************************/

//Directives au pr�-processeur
#include "pion.h"

//Detructeur
pion::~pion()
{
  init();
}

//Le joueur s�lectionne la destination de sa pi�ce
bool pion::destination(plateau sTabPlateau[8][8], int iXPiece, int iYPiece,
                       int iXDeplacement, int iYDeplacement, int iJoueur)
{
  //D�claration des variables

  bool bAction = 0;  //Si on performe l'action ou non

  //Si ce n'est pas la m�me �quipe
  if (sTabPlateau[iYDeplacement][iXDeplacement].pieceEchec->getEquipe() != iJoueur)
  {
    //Si le mouvement est droit
    if (iXDeplacement == iXPiece)
    {
      //Si c'est une case par en avant vide
      if (iYDeplacement == iYPiece - iJoueur && sTabPlateau[iYDeplacement][iXDeplacement].pieceEchec->getEquipe()
          == 0)
        bAction = 1;

      //Si C'est le premier mouvement, 2 case vide par en avant
      else if (getBouger() == 0
          && iYDeplacement == iYPiece - iJoueur * 2
          && sTabPlateau[iYDeplacement + iJoueur][iXDeplacement].pieceEchec->getEquipe() == 0
          && sTabPlateau[iYDeplacement][iXDeplacement].pieceEchec->getEquipe() == 0)
        bAction = 1;
    }

    //Si c'est en diagonal en avant de 1
    else if (iYDeplacement == iYPiece - iJoueur
        && abs(iXDeplacement - iXPiece) == 1)
      bAction = 1;
  }
  return bAction;
}

//Retourne le type de la pi�ce
char pion::type() const
{
  return 'P';
}
