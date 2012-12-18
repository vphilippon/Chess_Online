/*****************************************************************************************
 En-tete du programme
 =====================
 Programme : tour.cpp
 Auteur : Alexandre Beaulieu et Vincent Philippon
 Date de creation : 04 mai 2010
 But du programme : M�thode de l'objet tour
 ******************************************************************************************/

//Directives au pr�-processeur
#include "tour.h"

//Detructeur
tour::~tour()
{
  init();
}

//Le joueur s�lectionne la destination de sa pi�ce
bool tour::destination(plateau sTabPlateau[8][8], int iXPiece, int iYPiece,
                       int iXDeplacement, int iYDeplacement, int iTour)
{
  //D�claration des variables

  bool bAction = 0;  //Si on performe l'action ou non

  //Si ce n'est pas la m�me �quipe
  if (sTabPlateau[iYDeplacement][iXDeplacement].pieceEchec->getEquipe() != iTour)
  {
    //Si c'est en vertical
    if (iYDeplacement == iYPiece)
    {
      //Si c'est coll� sur la pi�ce
      if (abs(iXDeplacement - iXPiece) == 1)
      {
        bAction = 1;
      }
      //S'il faut aller vers l'arri�re
      else if (iXDeplacement < iXPiece)
      {
        for (int i = iXPiece - 1; i > iXDeplacement; i--)
        {

          //Si l'espace n'est pas vide
          if (sTabPlateau[iYPiece][i].pieceEchec->getEquipe() != 0)
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
      //S'il faut aller vers l'avant
      else
      {
        for (int i = iXPiece + 1; i < iXDeplacement; i++)
        {
          //Si l'espace n'est pas vide
          if (sTabPlateau[iYPiece][i].pieceEchec->getEquipe() != 0)
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
    //Si c'est � l'horizontal
    else if (iXDeplacement == iXPiece)
    {
      //Si c'est coll� sur la pi�ce
      if (abs(iYDeplacement - iYPiece) == 1)
      {
        bAction = 1;
      }
      //S'il faut aller vers l'arri�re
      else if (iYDeplacement < iYPiece)
      {
        for (int i = iYPiece - 1; i > iYDeplacement; i--)
        {
          //Si l'espace n'est pas vide
          if (sTabPlateau[i][iXPiece].pieceEchec->getEquipe() != 0)
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
      //S'il faut aller vers l'avant
      else
      {
        for (int i = iYPiece + 1; i < iYDeplacement; i++)
        {
          //Si l'espace n'est pas vide
          if (sTabPlateau[i][iXPiece].pieceEchec->getEquipe() != 0)
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
char tour::type() const
{
  return 'T';
}
