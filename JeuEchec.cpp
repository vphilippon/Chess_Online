/*****************************************************************************************
 En-tete du programme
 =====================
 Programme : JeuEchec.cpp
 Auteur : Alexandre Beaulieu et Vincent Philippon
 Date de creation : 27 avril 2010
 But du programme :    Un jeu d'�chec jouable en r�seau.
 ******************************************************************************************/

//Directives au pr�processeur
#include <iostream>
#include <pthread.h>
#include "piece.h"
#include "tour.h"
#include "cavalier.h"
#include "fou.h"
#include "reine.h"
#include "roi.h"
#include "pion.h"
#include "passif.h"
#include "actif.h"
#include "sockets.h"
#include "affichage.h"
#include <SDL/SDL_image.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
using namespace std;

int main(int argc, char *argv[])
{
  //D�claration des variables
  plateau sTabPlateau[8][8] =
    {NULL};  //Tableau du plateau

  piece *temp = NULL;  //Pi�ce temporaire pour �change

  int iMenu = 1,  //Choix du joueur dans le menu de d�part
      iJoueur[2] =
        {0},  //Si le joueur joue les blancs ou les noirs
      iTour,  //Joueur en cours
      iXPiece,  //Coordon�e de la pi�ce dans les abscisses
      iYPiece,  //Coordon�e de la pi�ce dans les ordonn�es
      iXDestination,  //Coordon�e de la destination dans les abscisses
      iYDestination,  //Coordon�e de la destination dans les ordonn�es
      iXRoiBlanc,  //X du roi blanc
      iYRoiBlanc,  //Y du roi blanc
      iXRoiNoir,  //X du roi noir
      iYRoiNoir,  //Y du roi noir
      iAction,  //Fait-on l'action (donn�e transmise)
      iPromotion,  //Piece choisi pour la promotion
      iNbPiece,  //Nb de pieces sur le jeu
      iXPassant = -1,  //X de la case "prenable en passant"
      iYPassant = -1;  //Y de la case "prenable en passant"

  bool bPiecePrete = 1,  //Si les pi�ces sont pretes
      bZoneSaisie = 0,  //Si c'est le moment de la saisie
      bAction,  //Fait-on l'action
      bEchec,  //Le joueur est-il echec
      bBouger,  //Le joueur peut-il faire un mouvement
      bFinPartie = 0;  //Si c'est la fin de la partie

  char strJoueur[9];  //Joueur actuel

  SDL_Event event;  //�v�nement capt� par SDL_WaitEvent

  SDL_Surface *ecran = NULL,  //pointeur de l'�cran
      *joueurActuel = NULL;  //Surface pour le texte du joueur actuel

  SDL_Rect positionTexte;  //O� est affich� le joueur actuel

  SDL_Color couleur =
    {136, 0, 21};  //Couleur pour le texte en jeu

  Uint32 couleurFond = NULL;  //Couleur de fond pour le texte

  pthread_t threadAccept,  //Thread pour accepter la connexion
      threadReception;  //Thread pour la r�ception de donn�e

  socketClientRecue client;  //Structure pour le thread d'acceptation

  receptionCoords coords;  //Structure pour le thread de r�ception de coordonn�e

  receptionProm prom;  //Structure pour le thread de r�ception de promotion

  //Contexte d'adressage
  SOCKADDR_IN sin,  //du serveur
      cSin;  //du client

  //Num�ro de la socket
  SOCKET sock,  //du serveur
      cSock,  //du client
      sockTransmition;  //Socket pour les transmitions de donn�es

  socklen_t recsize = sizeof(sin); /*Grandeur du contexte d'adressage
   du serveur    */

  //Chargement
  if (!initialiserLibSocket())
  {
    cout << "Erreur lors de l'initialisation des sockets";
    fflush(stdin);
    cin.get();
    exit(EXIT_FAILURE);
  }

  if (configurationServeur(sin, sock, recsize) == -1)
  {
    cout << "Erreur lors de la configuration des sockets serveur";
    fflush(stdin);
    cin.get();
    exit(EXIT_FAILURE);
  }

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) == -1 || TTF_Init() == -1)  //evenement d'�chec de sdl
  {
    cout << "Erreur lors de l'initialisation de l'une des librairies SDL", SDL_GetError();
    fflush(stdin);
    cin.get();
    exit(EXIT_FAILURE);
  }

  //Positions des cadres
  positionTexte.x = 600;
  positionTexte.y = 200;
  positionTexte.h = 80;
  positionTexte.w = 200;

  preparerPieces(sTabPlateau, iXRoiBlanc, iYRoiBlanc, iXRoiNoir, iYRoiNoir);

  client.sock = &sock;
  client.cRecsize = sizeof(client.cSin);

  //D�finition de l'icone
  SDL_WM_SetIcon(IMG_Load("roi_blanc.png"), NULL);

  //Param�tre du mode vid�o
  ecran = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

  couleurFond = SDL_MapRGB(ecran->format, 192, 192, 192);

  //Changer le nom de la fenetre
  SDL_WM_SetCaption("Jeu d'Echec", NULL);

  afficherImage(ecran, "Menu.PNG");

  //Affectation des adresse dans les variables des structures des threads
  client.iJoueur[0] = &iJoueur[0];
  client.iJoueur[1] = &iJoueur[1];
  client.sockTransmition = &sockTransmition;
  client.bZoneSaisie = &bZoneSaisie;
  client.ecran = ecran;
  client.cSock = &cSock;
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
      coords.sTabPlateau[i][j] = &sTabPlateau[i][j];
  coords.ecran = ecran;
  coords.bAction = &bAction;
  coords.iAction = &iAction;
  coords.iXDestination = &iXDestination;
  coords.iXPiece = &iXPiece;
  coords.iYDestination = &iYDestination;
  coords.iYPiece = &iYPiece;
  coords.sockTransmition = prom.sockTransmition = &sockTransmition;
  prom.iPromotion = &iPromotion;

  //Boucle du menu du jeu
  do
  {
    SDL_WaitEvent(&event);
    switch (event.type)
    {
      case SDL_QUIT:  //Sur le x de la fenetre, on arr�te le programme
        iMenu = 0;
        break;

      case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
          iMenu = 0;
        break;

      case SDL_MOUSEBUTTONUP:  //Lorsque l'utilisateur clique

        //Si c'est le bouton gauche
        if (event.button.button == SDL_BUTTON_LEFT)
        {
          //Si c'est Jouer
          if (event.button.x > 300 && event.button.x < 480
              && event.button.y > 180 && event.button.y < 240)
            iMenu = 2;

          //Si c'est Instruction
          else if (event.button.x > 270 && event.button.x < 520
                   && event.button.y > 270 && event.button.y < 320)
            iMenu = 3;

          //Si c'est Quitter
          else if (event.button.x > 310 && event.button.x < 490
                   && event.button.y > 400 && event.button.y < 450)
            iMenu = 0;
        }

    }

    if (iMenu == 2)  //Si l'option jouer est choisie
    {
      bZoneSaisie = 1;
      pthread_create(&threadAccept, NULL, accepterConnexion, &client);
      cSock = socket(AF_INET, SOCK_STREAM, 0);  //Cr�e la socket cliente

      //Connexion, quitte si le joueur a fait quitter
      if (connecterAuServeur(ecran, cSin, cSock, sockTransmition, iJoueur,
                             threadAccept)
          == 0)
        break;
      iMenu = 1;
      iTour = 1;
      bFinPartie = 0;

      SDL_FillRect(ecran, NULL, couleurFond);
      SDL_FillRect(ecran, NULL, couleurFond);
      creerDamier(ecran, sTabPlateau, iXRoiBlanc, iYRoiBlanc, iXRoiNoir,
                  iYRoiNoir, bPiecePrete);

      do
      {
        //Assigner le bon nom
        if (iTour == 1)
          strcpy(strJoueur, "Blanc");
        else
          strcpy(strJoueur, "Noir");

        afficherTexte(ecran, strJoueur, "papyrus.ttf", 40, couleur,
                      positionTexte, couleurFond);

        //V�rification si le joueur est en �chec et/ou ne peut plus faire de mouvement(echec, mat et pat)
        //Blanc
        if (iTour > 0)
          bEchec = verifierEchec(sTabPlateau, iTour, iXRoiBlanc, iYRoiBlanc);

        //Noir
        else
          bEchec = verifierEchec(sTabPlateau, iTour, iXRoiNoir, iYRoiNoir);

        bBouger = verifierBouger(sTabPlateau, iTour, iXRoiBlanc, iYRoiBlanc,
                                 iXRoiNoir, iYRoiNoir, iXPassant, iYPassant);

        iNbPiece = compterPiece(sTabPlateau);

        //S'il est echec et mat
        if (bEchec && !bBouger)
        {
          bFinPartie = 1;
          afficherEtat(ecran, "Echec et mat", false);
        }

        //Si il est juste �chec
        else if (bEchec)
          afficherEtat(ecran, "Echec", false);

        //Si c'est nul
        else if (!bBouger || iNbPiece == 2)
        {
          bFinPartie = 1;
          afficherEtat(ecran, "Partie Nulle", true);
        }

        else
          afficherEtat(ecran, "Tour au", false);

        SDL_Flip(ecran);

        //Si ce n'est pas la fin de la partie
        if (!bFinPartie)
        {
          do
          {
            iYDestination = -1;
            do
            {
              //Si c'est le tour du joueur courant
              if (iTour == iJoueur[0] || iTour == iJoueur[1])
              {
                selectionPiece(ecran, sTabPlateau, iXPiece, iYPiece, iTour,
                               sockTransmition, sock, cSock);

                selectionDestination(ecran, sTabPlateau, iXPiece, iYPiece,
                                     iXDestination, iYDestination, iTour,
                                     bAction, sockTransmition, sock, cSock);
              }
              else
              {
                iAction = -1;
                pthread_create(&threadReception, NULL, recevoirCoordonnee,
                               &coords);
                //Tant que iAction n'a pa �t� re�u
                while (iAction == -1)
                {
                  SDL_PollEvent(&event);
                  switch (event.type)
                  {
                    case SDL_QUIT:
                      quitter(sTabPlateau, sock, cSock);
                      break;

                    case SDL_KEYDOWN:
                      if (event.key.keysym.sym == SDLK_ESCAPE)
                        quitter(sTabPlateau, sock, cSock);
                      break;
                  }
                }
              }

              effacerSurlignement(
                  ecran, sTabPlateau[iYPiece][iXPiece].posCase,
                  sTabPlateau[iYPiece][iXPiece].fond,
                  sTabPlateau[iYPiece][iXPiece].pieceEchec->getImage());

              //Si bAction est vrais
              if (bAction)
                //Tant que iYDestination n,a pa �t� re�u
                while (iYDestination == -1)
                {

                  SDL_PollEvent(&event);
                  switch (event.type)
                  {
                    case SDL_QUIT:
                      quitter(sTabPlateau, sock, cSock);
                      break;

                    case SDL_KEYDOWN:
                      if (event.key.keysym.sym == SDLK_ESCAPE)
                        quitter(sTabPlateau, sock, cSock);
                      break;
                  }
                }

            }while (!bAction || iYDestination == -1);  //Tant que le joueur annule ou clique � un mauvais endroit ou

            //Si prise en passant
            if (sTabPlateau[iYPiece][iXPiece].pieceEchec->type() == 'P' && sTabPlateau[iYDestination][iXDestination].pieceEchec->getEquipe()
                == 0
                && iYDestination == iYPiece - iTour
                && abs(iXDestination - iXPiece) == 1)
            {
              if (iXDestination == iXPassant && iYDestination == iYPassant)
              {
                temp = copierPiece(
                    sTabPlateau[iYDestination][iXDestination].pieceEchec);
                bAction = verifierMouvement(sTabPlateau, iXPiece, iYPiece,
                                            iXDestination, iYDestination, iTour,
                                            temp, iXRoiBlanc, iYRoiBlanc,
                                            iXRoiNoir, iYRoiNoir);
                if (bAction)
                {
                  sTabPlateau[iYDestination + iTour][iXDestination].pieceEchec =
                      new piece;
                  SDL_FillRect(
                      ecran,
                      &sTabPlateau[iYDestination + iTour][iXDestination].posCase,
                      sTabPlateau[iYDestination + iTour][iXDestination].fond);
                  SDL_BlitSurface(
                      sTabPlateau[iYDestination + iTour][iXDestination].pieceEchec->getImage(),
                      NULL,
                      ecran,
                      &sTabPlateau[iYDestination + iTour][iXDestination].posCase);
                }
              }
              else
                bAction = false;
            }

            //Si Roque
            else if (sTabPlateau[iYPiece][iXPiece].pieceEchec->type() == 'K'
                && sTabPlateau[iYDestination][iXDestination].pieceEchec->type() == 'T'
                && sTabPlateau[iYDestination][iXDestination].pieceEchec->getEquipe() == iTour
                && sTabPlateau[iYDestination][iXDestination].pieceEchec->getBouger() == false
                && sTabPlateau[iYPiece][iXPiece].pieceEchec->getBouger() == false)
              bAction = roque(ecran, sTabPlateau, iTour, iXPiece, iYPiece,
                              iXDestination, iYDestination, iXRoiBlanc,
                              iYRoiBlanc, iXRoiNoir, iYRoiNoir);

            //Si c'est un mouvement normal
            else
            {
              temp = copierPiece(
                  sTabPlateau[iYDestination][iXDestination].pieceEchec);
              bAction = verifierMouvement(sTabPlateau, iXPiece, iYPiece,
                                          iXDestination, iYDestination, iTour,
                                          temp, iXRoiBlanc, iYRoiBlanc,
                                          iXRoiNoir, iYRoiNoir);
            }
          }while (!bAction);  //Tant que le joueur se trouve en �chec apres le mouvement

          conclureDeplacement(ecran, sTabPlateau, iXPiece, iYPiece,
                              iXDestination, iYDestination);

          //Si le pion est � l'extr�mit� adverse
          //conversion en int pour arrondir � l'unit� inf�rieure
          if (iYDestination == (3.5 - 3.5 * iTour))
          {
            //Si la pi�ce est un pion
            if (sTabPlateau[iYDestination][iXDestination].pieceEchec->type() == 'P')
            {
              //Si c'est le tour du joueur courant
              if (iTour == iJoueur[0] || iTour == iJoueur[1])
              {
                iPromotion = choixPromotion(ecran, iTour, sockTransmition,
                                            sTabPlateau, sock, cSock);
                envoyerInt(sockTransmition, iPromotion);
              }
              else
              {
                iPromotion = -1;
                pthread_create(&threadReception, NULL, recevoirPromotion,
                               &prom);
                //Tant que iPromotion n'a pa �t� re�u
                while (iPromotion == -1)
                {
                  SDL_PollEvent(&event);
                  switch (event.type)
                  {
                    case SDL_QUIT:
                      quitter(sTabPlateau, sock, cSock);
                      break;

                    case SDL_KEYDOWN:
                      if (event.key.keysym.sym == SDLK_ESCAPE)
                        quitter(sTabPlateau, sock, cSock);
                      break;
                  }
                }
              }

              appliquerPromotion(ecran,
                                 sTabPlateau[iYDestination][iXDestination],
                                 iPromotion);

            }

          }

          //Si un pion vient d'avancer de 2, s'ouvrant � une prise en passant
          if (sTabPlateau[iYDestination][iXDestination].pieceEchec->type() == 'P' && abs(
              iYDestination - iYPiece)
                                                                                     == 2
              && iXDestination == iXPiece)
          {
            iXPassant = iXDestination;
            iYPassant = iYDestination + iTour;
          }
          //Sinon
          else
            iXPassant = iYPassant = -1;

          iTour *= -1;
        }

      }while (!bFinPartie);  //Tant que pas la fin

      bPiecePrete = 0;
      bZoneSaisie = 0;
      attenteClique(sTabPlateau, sock, cSock);
      afficherImage(ecran, "Menu.PNG");
      deconnecterSocket(sock, cSock);
      if (iJoueur[0] == 0)
      {
        cSock = accept(sock, (SOCKADDR*)&cSin, &recsize);
        deconnecterSocket(sock, cSock);
      }
      iJoueur[0] = 0;
      iJoueur[1] = 0;
      configurationServeur(sin, sock, recsize);
    }

    else if (iMenu == 3)
    {
      iMenu = 1;
      afficherImage(ecran, "Instructions.PNG");
      attenteClique(sTabPlateau, sock, cSock);
      afficherImage(ecran, "Menu.PNG");

    }

  }while (iMenu);  //Tant que pas choisi quitter

  //Fermeture ------------------------- 2012
  quitter(sTabPlateau, sock, cSock);
  return EXIT_SUCCESS;
}
