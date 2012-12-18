/*****************************************************************************************
 En-tete du programme
 =====================
 fichier : affichage.h
 Auteur : Alexandre Beaulieu et Vincent Philippon
 Date de creation : 13 mai 2010
 Fichier d'ent�te des actions d'affichage
 ******************************************************************************************/

#ifndef AFFICHAGE_H_
#define AFFICHAGE_H_
#include <SDL/SDL_image.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
using namespace std;

//Affiche une image
void afficherImage(SDL_Surface *ecran, const char *strNom, SDL_Rect *position =
    NULL)
{
  //D�clarations des variables
  SDL_Surface *image;  //Surface pour l'image

  image = IMG_Load(strNom);
  SDL_BlitSurface(image, NULL, ecran, position);
  SDL_Flip(ecran);
  SDL_FreeSurface(image);
}

//Affiche du texte
void afficherTexte(SDL_Surface *ecran, const char *message,
                   const char *nomPolice, int iTaille, SDL_Color couleur,
                   SDL_Rect position, Uint32 couleurFond, int style = 0)
{
  //D�clarations des variables
  SDL_Surface *texte;  //Texte affich�
  TTF_Font *police;  //Police

  police = TTF_OpenFont(nomPolice, iTaille);
  TTF_SetFontStyle(police, style);
  texte = TTF_RenderText_Blended(police, message, couleur);
  SDL_FillRect(ecran, &position, couleurFond);
  SDL_BlitSurface(texte, NULL, ecran, &position);
  SDL_Flip(ecran);
  TTF_CloseFont(police);
  SDL_FreeSurface(texte);
}

//Affiche l'etat du joueur
void afficherEtat(SDL_Surface *ecran, const char *strEtat, bool bNulle)
{
  //D�clarations des variables
  SDL_Rect positionEtat,  //Position du message d'�tat
      positionMasque;  //position du masque si match null

  SDL_Color couleurEtat =
    {136, 0, 21};  //Couleur pour le texte de l'�tat

  //positions des cadres
  positionEtat.x = 600;
  positionEtat.y = 150;
  positionEtat.h = 50;
  positionEtat.w = 200;

  positionMasque.x = 600;
  positionMasque.y = 200;
  positionMasque.h = 80;
  positionMasque.w = 200;

  //Si partie nulle
  if (bNulle)
    afficherTexte(ecran, "", "papyrus.ttf", 40, couleurEtat, positionMasque,
                  SDL_MapRGB(ecran->format, 192, 192, 192));

  afficherTexte(ecran, strEtat, "papyrus.ttf", 35, couleurEtat, positionEtat,
                SDL_MapRGB(ecran->format, 192, 192, 192));
}

#endif
