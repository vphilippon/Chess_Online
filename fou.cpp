/*****************************************************************************************
 En-tete du programme
 =====================
 Programme : fou.cpp
 Auteur : Alexandre Beaulieu et Vincent Philippon
 Date de creation : 07 mai 2010
 But du programme : M�thode de l'objet fou
 ******************************************************************************************/

//Directives au pr�-processeur
#include "fou.h"

//Detructeur
fou::~fou()
{
  init();
}

//Le joueur s�lectionne la destination de sa pi�ce
bool fou::destination(plateau sTabPlateau[8][8], int iXPiece, int iYPiece,
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

    //Si c'est en diagonal
    if (abs(iDifferenceY) == abs(iDifferenceX))
    {
      //Si c'est coll� sur la pi�ce
      if (abs(iDifferenceX) == 1)
      {
        bAction = 1;
      }

      //Si X+, Y+
      else if (iDifferenceX > 0 && iDifferenceY > 0)
      {
        for (int i = 1; i < abs(iDifferenceX); i++)
        {

          //Si l'espace n'est pas vide
          if (sTabPlateau[iYPiece + i][iXPiece + i].pieceEchec->getEquipe() != 0)
          {
            bAction = 0;
            break;
          }

          //Si l'espace est vide
          else
          {
            bAction = 1;
          }
        }
      }
      //Si X+, Y-
      else if (iDifferenceX > 0 && iDifferenceY < 0)
      {
        for (int i = 1; i < abs(iDifferenceX); i++)
        {
          //Si l'espace n'est pas vide
          if (sTabPlateau[iYPiece - i][iXPiece + i].pieceEchec->getEquipe() != 0)
          {
            bAction = 0;
            break;
          }
          //Si l'espace est vide
          else
          {
            bAction = 1;
          }
        }
      }
      //Si X-, Y+
      else if (iDifferenceX < 0 && iDifferenceY > 0)
      {
        for (int i = 1; i < abs(iDifferenceX); i++)
        {
          //Si l'espace n'est pas vide
          if (sTabPlateau[iYPiece + i][iXPiece - i].pieceEchec->getEquipe() != 0)
          {
            bAction = 0;
            break;
          }
          //Si l'espace est vide
          else
          {
            bAction = 1;
          }
        }
      }
      //Si X-, Y-
      else
      {
        for (int i = 1; i < abs(iDifferenceX); i++)
        {
          //Si l'espace n'est pas vide
          if (sTabPlateau[iYPiece - i][iXPiece - i].pieceEchec->getEquipe() != 0)
          {
            bAction = 0;
            break;
          }
          //Si l'espace est vide
          else
          {
            bAction = 1;
          }
        }
      }
    }
  }
  return bAction;
}

//Retourne le type de la pi�ce
char fou::type() const
{
  return 'F';
}
