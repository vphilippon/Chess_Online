/*****************************************************************************************
 En-tete du programme
 =====================
 fichier : passif.h
 Auteur : Alexandre Beaulieu et Vincent Philippon
 Date de creation : 27 avril 2010
 Fichier d'ent�te des actions passives
 ******************************************************************************************/

//Directive au pr�-processeur
#ifndef PASSIF_H_
#define PASSIF_H_
#include <iostream>
#include <SDL/SDL_image.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "affichage.h"
#include "sockets.h"
using namespace std;

void preparerPieces(plateau sTabPlateau[8][8], int &iXRoiBlanc, int &iYRoiBlanc,
                    int &iXRoiNoir, int &iYRoiNoir)
{
  static Uint32 couleurNoire = 0,  //Couleur noire
      couleurBlanche = 16777215;  //Couleur blanche

  //Section Blanc
  sTabPlateau[7][0].pieceEchec = new tour(1, IMG_Load("tour_blanc.png"));
  sTabPlateau[7][7].pieceEchec = new tour(1, IMG_Load("tour_blanc.png"));
  sTabPlateau[7][1].pieceEchec = new cavalier(1,
                                              IMG_Load("cavalier_blanc.png"));
  sTabPlateau[7][6].pieceEchec = new cavalier(1,
                                              IMG_Load("cavalier_blanc.png"));
  sTabPlateau[7][2].pieceEchec = new fou(1, IMG_Load("fou_blanc.png"));
  sTabPlateau[7][5].pieceEchec = new fou(1, IMG_Load("fou_blanc.png"));
  sTabPlateau[7][3].pieceEchec = new reine(1, IMG_Load("reine_blanc.png"));
  sTabPlateau[7][4].pieceEchec = new roi(1, IMG_Load("roi_blanc.png"));
  for (int i = 0; i < 8; i++)
    sTabPlateau[6][i].pieceEchec = new pion(1, IMG_Load("pion_blanc.png"));

  //Section Noir
  sTabPlateau[0][0].pieceEchec = new tour(-1, IMG_Load("tour_noir.png"));
  sTabPlateau[0][7].pieceEchec = new tour(-1, IMG_Load("tour_noir.png"));
  sTabPlateau[0][1].pieceEchec = new cavalier(-1,
                                              IMG_Load("cavalier_noir.png"));
  sTabPlateau[0][6].pieceEchec = new cavalier(-1,
                                              IMG_Load("cavalier_noir.png"));
  sTabPlateau[0][2].pieceEchec = new fou(-1, IMG_Load("fou_noir.png"));
  sTabPlateau[0][5].pieceEchec = new fou(-1, IMG_Load("fou_noir.png"));
  sTabPlateau[0][3].pieceEchec = new reine(-1, IMG_Load("reine_noir.png"));
  sTabPlateau[0][4].pieceEchec = new roi(-1, IMG_Load("roi_noir.png"));
  for (int i = 0; i < 8; i++)
    sTabPlateau[1][i].pieceEchec = new pion(-1, IMG_Load("pion_noir.png"));

  //Section Vide
  for (int i = 2; i < 6; i++)
    for (int j = 0; j < 8; j++)
      sTabPlateau[i][j].pieceEchec = new piece();

  //Les fonds
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
      if (i % 2 == 0)  //Si pair

        if (j % 2 == 0)  //Si pair
          sTabPlateau[i][j].fond = couleurBlanche;
        else
          //Si impair
          sTabPlateau[i][j].fond = couleurNoire;

      else  //Si impair

      if (j % 2 == 0)  //Si pair
        sTabPlateau[i][j].fond = couleurNoire;
      else
        //Si impair
        sTabPlateau[i][j].fond = couleurBlanche;

  //Positions des rois
  iXRoiBlanc = iXRoiNoir = 4;
  iYRoiBlanc = 7;
  iYRoiNoir = 0;

  //Initialisation des rectangles
  for (int j = 0; j < 8; j++)
    for (int i = 0; i < 8; i++)
    {
      sTabPlateau[i][j].posCase.x = 60 * (j + 1);
      sTabPlateau[i][j].posCase.y = 60 * (i + 1);
      sTabPlateau[i][j].posCase.h = 60;
      sTabPlateau[i][j].posCase.w = 60;
    }
}

//Pr�pare le damier au d�but du jeu
void creerDamier(SDL_Surface *ecran, plateau sTabPlateau[8][8], int &iXRoiBlanc,
                 int &iYRoiBlanc, int &iXRoiNoir, int &iYRoiNoir,
                 bool piecePrete)
{
  //D�claration des variables
  SDL_Surface *damier;  //Surface pour le damier

  if (!piecePrete)
    preparerPieces(sTabPlateau, iXRoiBlanc, iYRoiBlanc, iXRoiNoir, iYRoiNoir);

  damier = IMG_Load("Damier.png");
  SDL_BlitSurface(damier, NULL, ecran, NULL);

  //Boucle de Blitz
  for (int j = 0; j < 8; j++)
    for (int i = 0; i < 8; i++)
      SDL_BlitSurface(sTabPlateau[j][i].pieceEchec->getImage(), NULL, ecran,
                      &sTabPlateau[j][i].posCase);

  SDL_Flip(ecran);
  SDL_FreeSurface(damier);
}

//V�rifier se le clique � eu lieu dans le damier
bool cliqueDansDamier(Uint16 x, Uint16 y)
{
  if (x <= 540 && x >= 60 &&  //Si dans le damier
      y <= 540 && y >= 60)
  {
    return true;
  }
  else  //Si pas dans le damier
  {
    return false;
  }
}

//Copie la pi�ce selon le type
piece* copierPiece(piece *piece2)
{
  //D�claration des variables
  char (piece::*type)() const = &piece::type;

  switch ((piece2->*type)())
  {
    //Si vide
    case 'V':
      return new piece();
      break;

      //Si pion
    case 'P':
      return new pion(*piece2);
      break;

      //Si tour
    case 'T':
      return new tour(*piece2);
      break;

      //Si cavalier
    case 'C':
      return new cavalier(*piece2);
      break;

      //Si fou
    case 'F':
      return new fou(*piece2);
      break;

      //Si reine
    case 'Q':
      return new reine(*piece2);
      break;

      //Si roi
    case 'K':
      return new roi(*piece2);
      break;

      //Autre (eliminer warning)
    default:
      return new piece();
  }
}

//Fait le d�placement de la pi�ce
void deplacerPiece(plateau sTabPlateau[8][8], int iXPiece, int iYPiece,
                   int iXDestination, int iYDestination, int &iXRoiBlanc,
                   int &iYRoiBlanc, int &iXRoiNoir, int &iYRoiNoir)
{
  //Mise � jour de la position des roi
  if (iXPiece == iXRoiBlanc && iYPiece == iYRoiBlanc)
  {
    iXRoiBlanc = iXDestination;
    iYRoiBlanc = iYDestination;
  }
  else if (iXPiece == iXRoiNoir && iYPiece == iYRoiNoir)
  {
    iXRoiNoir = iXDestination;
    iYRoiNoir = iYDestination;
  }

  //Si la case contient une pi�ce ennemi (non vide)
  if (sTabPlateau[iYDestination][iXDestination].pieceEchec->getEquipe() != 0)
    sTabPlateau[iYDestination][iXDestination].pieceEchec = new piece();

  swap(sTabPlateau[iYPiece][iXPiece].pieceEchec,
       sTabPlateau[iYDestination][iXDestination].pieceEchec);
}

//R�tablie la position des pi�ces avant le d�placement
void retablirPiece(plateau sTabPlateau[8][8], int iXPiece, int iYPiece,
                   int iXDestination, int iYDestination, piece *temp,
                   int &iXRoi, int &iYRoi)
{
  //Mise � jour de la position du roi
  if (iXDestination == iXRoi && iYDestination == iYRoi)
  {
    iXRoi = iXPiece;
    iYRoi = iYPiece;
  }

  swap(sTabPlateau[iYPiece][iXPiece].pieceEchec,
       sTabPlateau[iYDestination][iXDestination].pieceEchec);

  sTabPlateau[iYDestination][iXDestination].pieceEchec = copierPiece(temp);
}

//Afficher le deplacement et l'officialiser
void conclureDeplacement(SDL_Surface *ecran, plateau sTabPlateau[8][8],
                         int iXPiece, int iYPiece, int iXDestination,
                         int iYDestination)
{
  //Si la pi�ce n'a pas d�ja �t� boug�
  if (sTabPlateau[iYDestination][iXDestination].pieceEchec->getBouger() == false)
    sTabPlateau[iYDestination][iXDestination].pieceEchec->setBouger(true);

  SDL_FillRect(ecran, &sTabPlateau[iYPiece][iXPiece].posCase,
               sTabPlateau[iYPiece][iXPiece].fond);

  SDL_FillRect(ecran, &sTabPlateau[iYDestination][iXDestination].posCase,
               sTabPlateau[iYDestination][iXDestination].fond);

  SDL_BlitSurface(
      sTabPlateau[iYDestination][iXDestination].pieceEchec->getImage(), NULL,
      ecran, &sTabPlateau[iYDestination][iXDestination].posCase);

  SDL_BlitSurface(sTabPlateau[iYPiece][iXPiece].pieceEchec->getImage(), NULL,
                  ecran, &sTabPlateau[iYPiece][iXPiece].posCase);

  SDL_Flip(ecran);
}

//V�rifie si le roi se trouve ou se trouverait en �chec
bool verifierEchec(plateau sTabPlateau[8][8], int iTour, int iXRoi, int iYRoi)
{
  //D�clarations des variables
  bool bEchec = 0;  //Si une pi�ce met en echec le roi

  //Boucle de v�rification dans le tableau
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
    {
      //Si la pi�ce appartient � l'autre �quipe
      if (sTabPlateau[i][j].pieceEchec->getEquipe() == iTour * -1)
      {
        bEchec = sTabPlateau[i][j].pieceEchec->destination(sTabPlateau, j, i,
                                                           iXRoi, iYRoi,
                                                           iTour * -1);
        //Si une pi�ce met le roi en echec
        if (bEchec)
          return bEchec;
      }

    }

  return bEchec;
}

//V�rifie si le mouvement ne cause pas de bris de r�gle (mise en echec personnel)
bool verifierMouvement(plateau sTabPlateau[8][8], int iXPiece, int iYPiece,
                       int iXDestination, int iYDestination, int iTour,
                       piece *temp, int &iXRoiBlanc, int &iYRoiBlanc,
                       int &iXRoiNoir, int &iYRoiNoir)
{
  //D�clarations des variables
  bool bBouger = 1;  //Si la piece peut bouger

  deplacerPiece(sTabPlateau, iXPiece, iYPiece, iXDestination, iYDestination,
                iXRoiBlanc, iYRoiBlanc, iXRoiNoir, iYRoiNoir);

  //V�rification si le joueur est en �chec
  //Blanc
  if (iTour > 0 && verifierEchec(sTabPlateau, iTour, iXRoiBlanc, iYRoiBlanc))
  {
    retablirPiece(sTabPlateau, iXPiece, iYPiece, iXDestination, iYDestination,
                  temp, iXRoiBlanc, iYRoiBlanc);
    bBouger = false;
  }
  //Noir
  else if (iTour < 0 && verifierEchec(sTabPlateau, iTour, iXRoiNoir, iYRoiNoir))
  {
    retablirPiece(sTabPlateau, iXPiece, iYPiece, iXDestination, iYDestination,
                  temp, iXRoiNoir, iYRoiNoir);
    bBouger = false;
  }

  return bBouger;
}

//V�rifie si le joueur peur faire un mouvement l�gal
bool verifierBouger(plateau sTabPlateau[8][8], int iTour, int iXRoiBlanc,
                    int iYRoiBlanc, int iXRoiNoir, int iYRoiNoir, int iXPassant,
                    int iYPassant)
{
  //D�clarations des variables
  bool bBouger = 0;  //Si la piece peut bouger

  int i,  //Itt�rateur du y de la piece
      j,  //Itt�rateur du x de la piece
      k,  //Itt�rateur du y de la destination
      l;  //Itt�rateur du x de la destination

  piece * temp = NULL;  //Piece temporaire

  //Boucle de v�rification dans le tableau
  for (i = 0; i < 8; i++)
    for (j = 0; j < 8; j++)
    {
      //Si la pi�ce appartient au joueur
      if (sTabPlateau[i][j].pieceEchec->getEquipe() == iTour)
      {
        for (k = 0; k < 8; k++)
          for (l = 0; l < 8; l++)
          {
            //Si la destination est correcte
            if (sTabPlateau[i][j].pieceEchec->destination(sTabPlateau, j, i, l,
                                                          k, iTour))
            {
              //Si prise en passant
              if (sTabPlateau[i][j].pieceEchec->type() == 'P' && sTabPlateau[k][l].pieceEchec->getEquipe()
                  == 0
                  && k == i - iTour && abs(l - j) == 1)
              {
                if (l == iXPassant && k == iYPassant)
                {
                  temp = copierPiece(sTabPlateau[k][l].pieceEchec);
                  bBouger = verifierMouvement(sTabPlateau, j, i, l, k, iTour,
                                              temp, iXRoiBlanc, iYRoiBlanc,
                                              iXRoiNoir, iYRoiNoir);
                }
                else
                  bBouger = false;
              }

              //Si c'est un mouvement normal
              else
              {
                temp = copierPiece(sTabPlateau[k][l].pieceEchec);
                bBouger = verifierMouvement(sTabPlateau, j, i, l, k, iTour,
                                            temp, iXRoiBlanc, iYRoiBlanc,
                                            iXRoiNoir, iYRoiNoir);
              }

              //Si une pi�ce peut bouger
              if (bBouger)
              {
                //Blanc
                if (iTour > 0)
                  retablirPiece(sTabPlateau, j, i, l, k, temp, iXRoiBlanc,
                                iYRoiBlanc);
                //Noir
                else
                  retablirPiece(sTabPlateau, j, i, l, k, temp, iXRoiNoir,
                                iYRoiNoir);

                return bBouger;
              }
            }
          }
      }

    }

  return bBouger;
}

//Compte le nombre de pi�ce sur le jeu
int compterPiece(plateau sTabPlateau[8][8])
{
  //D�clarations des variables
  int iNb = 0;  //Nb de pieces

  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
      //Si case non-vide
      if (sTabPlateau[i][j].pieceEchec->type() != 'V')
        iNb++;

  return iNb;
}

//Efface le surlgignement de la pi�ce
void effacerSurlignement(SDL_Surface *ecran, SDL_Rect posCase, Uint32 fond,
                         SDL_Surface *image)
{
  SDL_FillRect(ecran, &posCase, fond);
  SDL_BlitSurface(image, NULL, ecran, &posCase);
  SDL_Flip(ecran);
}

//Mouvement du Roque
bool roque(SDL_Surface *ecran, plateau sTabPlateau[8][8], int iTour,
           int iXPiece, int iYPiece, int &iXDestination, int &iYDestination,
           int &iXRoiBlanc, int &iYRoiBlanc, int &iXRoiNoir, int &iYRoiNoir)
{
  //Si Petit Roque
  if (iXDestination == 7)
  {
    for (int i = iXPiece + 1; i < iXDestination; i++)
      //Si est vis�e par l'ennemi
      if (verifierEchec(sTabPlateau, iTour, i, iYPiece))
        return false;

    //Si il y a un pion de l'�quipe oppos� devant la tour(echec au roi)
    if (sTabPlateau[iYPiece - iTour][7].pieceEchec->getEquipe() == -iTour
        && sTabPlateau[iYPiece - iTour][7].pieceEchec->type() == 'P')
      return false;

    //Si le mouvement est l�gal
    else
    {
      //D�placer la tour
      deplacerPiece(sTabPlateau, iXDestination, iYDestination, iXPiece + 1,
                    iYPiece, iXRoiBlanc, iYRoiBlanc, iXRoiNoir, iYRoiNoir);
      conclureDeplacement(ecran, sTabPlateau, iXDestination, iYDestination,
                          iXPiece + 1, iYPiece);
      //D�placer le roi
      iXDestination -= 1;
      deplacerPiece(sTabPlateau, iXPiece, iYPiece, iXDestination, iYDestination,
                    iXRoiBlanc, iYRoiBlanc, iXRoiNoir, iYRoiNoir);
      return true;
    }
  }

  //Si Grand Roque
  if (iXDestination == 0)
  {
    for (int i = iXPiece - 1; i > iXDestination; i--)
      //Si est vis�e par l'ennemi
      if (verifierEchec(sTabPlateau, iTour, i, iYPiece))
        return false;

    //Si il y a un pion de l'�quipe oppos� devant la tour(echec au roi)
    if (sTabPlateau[iYPiece - iTour][0].pieceEchec->getEquipe() == -iTour && sTabPlateau[iYPiece
        - iTour][0].pieceEchec->type()
                                                                             == 'P')
      return false;

    //Si le mouvement est l�gal
    else
    {
      //D�placer la tour
      deplacerPiece(sTabPlateau, iXDestination, iYDestination, iXPiece - 1,
                    iYPiece, iXRoiBlanc, iYRoiBlanc, iXRoiNoir, iYRoiNoir);
      conclureDeplacement(ecran, sTabPlateau, iXDestination, iYDestination,
                          iXPiece - 1, iYPiece);
      //D�placer le roi
      iXDestination += 2;
      deplacerPiece(sTabPlateau, iXPiece, iYPiece, iXDestination, iYDestination,
                    iXRoiBlanc, iYRoiBlanc, iXRoiNoir, iYRoiNoir);
      return true;
    }
  }
}

//Appliquer la promotion
void appliquerPromotion(SDL_Surface *ecran, plateau &laCase, int iPromotion)
{
  //Selon le choix
  switch (iPromotion)
  {
    case 1:  //Si Reine
      laCase.pieceEchec = new reine(*laCase.pieceEchec);

      //Si Blanc
      if (laCase.pieceEchec->getEquipe() > 0)
        laCase.pieceEchec->setImage(IMG_Load("reine_blanc.png"));
      //Si Noir
      else
        laCase.pieceEchec->setImage(IMG_Load("reine_noir.png"));

      break;

    case 2:  //Si Tour
      laCase.pieceEchec = new tour(*laCase.pieceEchec);

      //Si Blanc
      if (laCase.pieceEchec->getEquipe() > 0)
        laCase.pieceEchec->setImage(IMG_Load("tour_blanc.png"));
      //Si Noir
      else
        laCase.pieceEchec->setImage(IMG_Load("tour_noir.png"));

      break;

    case 3:  //Si Fou
      laCase.pieceEchec = new fou(*laCase.pieceEchec);

      //Si Blanc
      if (laCase.pieceEchec->getEquipe() > 0)
        laCase.pieceEchec->setImage(IMG_Load("fou_blanc.png"));
      //Si Noir
      else
        laCase.pieceEchec->setImage(IMG_Load("fou_noir.png"));

      break;

    case 4:  //Si Cavalier
      laCase.pieceEchec = new cavalier(*laCase.pieceEchec);

      //Si Blanc
      if (laCase.pieceEchec->getEquipe() > 0)
        laCase.pieceEchec->setImage(IMG_Load("cavalier_blanc.png"));
      //Si Noir
      else
        laCase.pieceEchec->setImage(IMG_Load("cavalier_noir.png"));

      break;
  }

  SDL_FillRect(ecran, &laCase.posCase, laCase.fond);
  SDL_BlitSurface(laCase.pieceEchec->getImage(), NULL, ecran, &laCase.posCase);
  SDL_Flip(ecran);
}

//Quitte le programme
void quitter(plateau sTabPlateau[8][8], SOCKET sock, SOCKET cSock)
{
  for (int j = 0; j < 8; j++)
    for (int i = 0; i < 8; i++)
      SDL_FreeSurface(sTabPlateau[j][i].pieceEchec->getImage());

  fermetureSocket(sock, cSock);
  TTF_Quit();
  SDL_Quit();
  exit(EXIT_SUCCESS);
}

#endif
