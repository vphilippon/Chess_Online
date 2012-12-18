/*****************************************************************************************
 En-tete du programme
 =====================
 Programme : roi.cpp
 Auteur : Alexandre Beaulieu et Vincent Philippon
 Date de creation : 07 mai 2010
 But du programme : M�thode de l'objet roi
 ******************************************************************************************/

//Directives au pr�-processeur
#include "roi.h"

//Detructeur
roi::~roi()
{
  init();
}

//Le joueur s�lectionne la destination de sa pi�ce
bool roi::destination(plateau sTabPlateau[8][8], int iXPiece, int iYPiece,
                      int iXDeplacement, int iYDeplacement, int iTour)
{
  //D�claration des variables

  bool bAction = 0;  //Si on performe l'action ou non

  int iDifferenceX,  //Diff�rence des X
      iDifferenceY;  //Diff�rence des Y

  //Si ce n'est pas la m�me �quipe
  if (sTabPlateau[iYDeplacement][iXDeplacement].pieceEchec->getEquipe() != iTour)
  {

    iDifferenceX = iXDeplacement - iXPiece;
    iDifferenceY = iYDeplacement - iYPiece;

    //Si le mouvement n'est que de 1
    if ((abs(iDifferenceX) == 1 || abs(iDifferenceX) == 0) && (abs(iDifferenceY)
        == 1
                                                               || abs(
                                                                   iDifferenceY)
                                                                  == 0))
      bAction = 1;

  }

  //Le Roque
  else if (getBouger() == false
      && sTabPlateau[iYDeplacement][iXDeplacement].pieceEchec->getBouger() == false
      && sTabPlateau[iYDeplacement][iXDeplacement].pieceEchec->getEquipe() == iTour
      && sTabPlateau[iYDeplacement][iXDeplacement].pieceEchec->type() == 'T')
  {
    //Si Petit Roque
    if (iXDeplacement == 7)
      for (int i = iXPiece + 1; i < iXDeplacement; i++)
        //Si non vide
        if (sTabPlateau[iYPiece][i].pieceEchec->getEquipe() != 0)
        {
          bAction = 0;
          break;
        }

        //Si elle est vide
        else
          bAction = 1;

    //Si Grand Roque
    if (iXDeplacement == 0)
      for (int i = iXPiece - 1; i > iXDeplacement; i--)
        //Si non vide
        if (sTabPlateau[iYPiece][iXPiece - i].pieceEchec->getEquipe() != 0)
        {
          bAction = 0;
          break;
        }

        //Si elle est vide
        else
          bAction = 1;
  }

  return bAction;
}

//Retourne le type de la pi�ce
char roi::type() const
{
  return 'K';
}
