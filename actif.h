/******************************************************************************
 En-tete du programme
 =====================
 fichier : actif.h
 Auteur : Alexandre Beaulieu et Vincent Philippon
 Date de creation : 27 avril 2010
 Fichier d'entete des actions actives
 *****************************************************************************/

//Directive au pre-processeur
#ifndef ACTIF_H_
#define ACTIF_H_
#include "passif.h"
#include "affichage.h"
using namespace std;

//Le joueur choisi la piece avec laquelle il veut jouer
void selectionPiece(SDL_Surface *ecran, plateau sTabPlateau[8][8], int &x,
                    int &y, int iTour, SOCKET sockEnvoi, SOCKET &sock,
                    SOCKET &cSock)
{
  //Declaration des variables
  SDL_Event event;  //evenement capte par SDL_WaitEvent

  bool bSelection = false;  //Si le joueur a selectionne une piece

  do
  {
    SDL_WaitEvent(&event);
    switch (event.type)
    {
      case SDL_QUIT:  //Sur le x de la fenetre, on arrete le programme
        quitter(sTabPlateau, sock, cSock);
        break;

      case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
          quitter(sTabPlateau, sock, cSock);
        break;

      case SDL_MOUSEBUTTONUP:  //Lorsque l'utilisateur clique

        //Si c'est le bouton gauche
        if (event.button.button == SDL_BUTTON_LEFT)
        {
          //Conversion coord �cran pour coord tableau si dans le damier
          if (cliqueDansDamier(event.button.x, event.button.y))
          {
            x = event.button.x / 60 - 1;
            y = event.button.y / 60 - 1;

            //Si la pi�ce est � lui
            if (sTabPlateau[y][x].pieceEchec->getEquipe() * iTour > 0)
            {
              bSelection = true;
              afficherImage(ecran, "Selection.PNG", &sTabPlateau[y][x].posCase);
              envoyerInt(sockEnvoi, x);
              envoyerInt(sockEnvoi, y);
            }
          }
        }
        break;
    }
  }while (!bSelection);
}

//Le joueur s�lectionne la destination de sa pi�ce
void selectionDestination(SDL_Surface *ecran, plateau sTabPlateau[8][8],
                          int &iXPiece, int &iYPiece, int &iXDeplacement,
                          int &iYDeplacement, int iTour, bool &bAction,
                          SOCKET sockEnvoi, SOCKET &sock, SOCKET &cSock)
{
  //D�claration des variables
  SDL_Event event;  //�v�nement capt� par SDL_WaitEvent
  bool bContinuer = 1;  //Continuer la boucle ou non
  int iAction;  //Si on fait l'action ou non

  iAction = bAction = 0;

  do
  {
    SDL_WaitEvent(&event);
    switch (event.type)
    {
      case SDL_QUIT:  //Sur le x de la fenetre, on arr�te le programme
        quitter(sTabPlateau, sock, cSock);
        break;

      case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
          quitter(sTabPlateau, sock, cSock);
        break;

      case SDL_MOUSEBUTTONUP:  //Lorsque l'utilisateur clique

        //Si c'est le bouton gauche
        if (event.button.button == SDL_BUTTON_LEFT)
        {
          //Conversion coord �cran pour coord tableau si dans le damier
          if (cliqueDansDamier(event.button.x, event.button.y))
          {
            iXDeplacement = event.button.x / 60 - 1;
            iYDeplacement = event.button.y / 60 - 1;

            bAction = sTabPlateau[iYPiece][iXPiece].pieceEchec->destination(
                sTabPlateau, iXPiece, iYPiece, iXDeplacement, iYDeplacement,
                iTour);
            iAction = bAction;
            if (bAction)
            {
              envoyerInt(sockEnvoi, iAction);
              envoyerInt(sockEnvoi, iXDeplacement);
              envoyerInt(sockEnvoi, iYDeplacement);
            }
          }

          bContinuer = false;
        }

        //Si c'est le bouton droit
        else if (event.button.button == SDL_BUTTON_RIGHT)
          bContinuer = false;
        break;
    }
  }while (bContinuer);
  if (!bAction)
    envoyerInt(sockEnvoi, iAction);
}

void attenteClique(plateau sTabPlateau[8][8], SOCKET &sock, SOCKET &cSock)
{
  //D�clarations des variables
  int iContinuer = 1;  //Attente

  SDL_Event event;  //�venement SDL

  //Boucle d'attente
  do
  {
    SDL_WaitEvent(&event);
    switch (event.type)
    {
      case SDL_QUIT:  //Sur le x de la fenetre, on arr�te le programme
        quitter(sTabPlateau, sock, cSock);
        break;

      case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
          quitter(sTabPlateau, sock, cSock);
        break;

      case SDL_MOUSEBUTTONUP:  //Lorsque l'utilisateur clique
        iContinuer = 0;
    }

  }while (iContinuer);
}

//Choix de la piece pour la promotion
int choixPromotion(SDL_Surface *ecran, int iTour, int sockTransmition,
                   plateau sTabPlateau[8][8], int sock, int cSock)
{
  //D�clarations des variables
  int iChoix = 0;  //Choix de piece

  SDL_Event event;  //Evenement capt�

  SDL_Rect posTexte,  //Position du texte d'indication
      posTour,  //Rectangle du tour
      posCavalier,  //Rectangle du cavalier
      posFou,  //Rectangle du fou
      posReine;  //Rectangle du reine

  SDL_Color couleur =
    {136, 0, 21};  //Couleur pour le texte en jeu

  Uint32 couleurFond = 12632256;  //Couleur de fond pour le texte

  posTexte.x = 610;
  posTexte.y = 350;

  posReine.x = 610;
  posReine.y = 400;
  posReine.h = 60;
  posReine.w = 60;

  posTour.x = 670;
  posTour.y = 400;
  posTour.h = 60;
  posTour.w = 60;

  posFou.x = 610;
  posFou.y = 460;
  posFou.h = 60;
  posFou.w = 60;

  posCavalier.x = 670;
  posCavalier.y = 460;
  posCavalier.h = 60;
  posCavalier.w = 60;

  afficherTexte(ecran, "Cliquer pour choisir :", "papyrus.ttf", 20, couleur,
                posTexte, couleurFond);

  //Si tour au blanc
  if (iTour > 0)
  {
    afficherImage(ecran, "reine_blanc.png", &posReine);
    afficherImage(ecran, "tour_blanc.png", &posTour);
    afficherImage(ecran, "fou_blanc.png", &posFou);
    afficherImage(ecran, "cavalier_blanc.png", &posCavalier);
  }

  //Si tour au noir
  else
  {
    afficherImage(ecran, "reine_noir.png", &posReine);
    afficherImage(ecran, "tour_noir.png", &posTour);
    afficherImage(ecran, "fou_noir.png", &posFou);
    afficherImage(ecran, "cavalier_noir.png", &posCavalier);
  }

  do
  {
    SDL_WaitEvent(&event);
    switch (event.type)
    {
      case SDL_QUIT:  //Sur le x de la fenetre, on arr�te le programme
        quitter(sTabPlateau, sock, cSock);
        break;

      case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
          quitter(sTabPlateau, sock, cSock);
        break;

      case SDL_MOUSEBUTTONUP:  //Lorsque l'utilisateur clique

        //Si c'est le bouton gauche
        if (event.button.button == SDL_BUTTON_LEFT)
        {
          //Si la reine
          if (event.button.x > posReine.x && event.button.x
              < posReine.x + posReine.w
              && event.button.y > posReine.y
              && event.button.y < posReine.y + posReine.h)
            iChoix = 1;

          //Si c'est la tour
          else if (event.button.x > posTour.x
              && event.button.x < posTour.x + posTour.w
              && event.button.y > posTour.y
              && event.button.y < posTour.y + posTour.h)
            iChoix = 2;

          //Si c'est le fou
          else if (event.button.x > posFou.x
              && event.button.x < posFou.x + posFou.w
              && event.button.y > posFou.y
              && event.button.y < posFou.y + posFou.h)
            iChoix = 3;

          //Si c'est la cavalier
          else if (event.button.x > posCavalier.x
              && event.button.x < posCavalier.x + posCavalier.w
              && event.button.y > posCavalier.y
              && event.button.y < posCavalier.y + posCavalier.h)
            iChoix = 4;
        }

    }
  }while (iChoix == 0);  //Tant qu'il n'y a pas de choix

  afficherTexte(ecran, "", "papyrus.ttf", 20, couleur, posTexte, couleurFond);
  SDL_FillRect(ecran, &posReine, couleurFond);
  SDL_FillRect(ecran, &posTour, couleurFond);
  SDL_FillRect(ecran, &posFou, couleurFond);
  SDL_FillRect(ecran, &posCavalier, couleurFond);
  SDL_Flip(ecran);

  return iChoix;
}

#endif
