/*****************************************************************************************
 En-tete du programme
 =====================
 Programme : cavalier.cpp
 Auteur : Alexandre Beaulieu et Vincent Philippon
 Date de creation : 07 mai 2010
 But du programme : M�thode de l'objet cavalier
 ******************************************************************************************/

//Directives au pr�-processeur
#include "cavalier.h"

//Detructeur
cavalier::~cavalier()
{
  init();
}

//Le joueur s�lectionne la destination de sa pi�ce
bool cavalier::destination(plateau sTabPlateau[8][8], int iXPiece, int iYPiece,
                           int iXDeplacement, int iYDeplacement, int iTour)
{
  //D�claration des variables

  bool bAction = 0;  //Si on performe l'action ou non

  //Si ce n'est pas la m�me �quipe
  if (sTabPlateau[iYDeplacement][iXDeplacement].pieceEchec->getEquipe() != iTour)

    //Si le carr� des diff�rences donne 9 (donc mouvement en L)
    if (((iYDeplacement - iYPiece) * (iXDeplacement - iXPiece)) * ((iYDeplacement
        - iYPiece)
                                                                   * (iXDeplacement - iXPiece))
        == 4)
      bAction = 1;

  return bAction;
}

//Retourne le type de la pi�ce
char cavalier::type() const
{
  return 'C';
}
