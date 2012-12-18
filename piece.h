/*****************************************************************************************
 En-tete du programme
 =====================
 fichier : piece.h
 Auteur : Alexandre Beaulieu et Vincent Philippon
 Date de creation : 27 avril 2010
 Fichier d'ent�te de l'objet pi�ce
 ******************************************************************************************/

//Directive au pr�-processeur
#ifndef PIECE_H_
#define PIECE_H_
#include <iostream>
#include <SDL/SDL_image.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <assert.h>
using namespace std;

//D�claration avanc� de la structure
struct plateau;

class piece
{
  protected:
    //Propri�t�s
    int _equipe;  //Indicateur de l'�quipe de la pi�ce
    SDL_Surface *_image;  //Image de la pi�ce
    bool _bouger;  //Si la pi�ce a �t� boug�

  public:
    //M�thodes
    piece(int equipe = 0, SDL_Surface *image = NULL, bool bouger = false);  //Constructeur
    piece(piece &p);  //Constructeur de copy
    virtual ~piece();  //Destucteur
    void init(int equipe = 0, SDL_Surface *image = NULL, bool bouger = false);  //Initialisateur

    //Getteur
    int getEquipe() const;
    SDL_Surface* getImage() const;
    bool getBouger() const;

    //Setteur
    void setEquipe(int equipe);
    void setImage(SDL_Surface *image);
    void setBouger(bool bouger);

    //Surcharge d'op�rateur
    const piece& operator=(const piece &p);

    //M�thode virtuel
    virtual bool destination(plateau sTabPlateau[8][8], int iXPiece,
                             int iYPiece, int iXDeplacement, int iYDeplacement,
                             int iTour);  //S�lection de la dstination
    virtual char type() const;  //Retourne le type de piece
};

//Structure du plateau de jeu
struct plateau
{
    piece *pieceEchec;  //Pi�ce du jeu d'�chec sur la case
    Uint32 fond;  //Couleur de la case (fond)
    SDL_Rect posCase;  //Position du rectangle de la case
};

#endif
