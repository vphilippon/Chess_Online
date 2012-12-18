/*****************************************************************************************
 En-tete du programme
 =====================
 Programme : piece.cpp
 Auteur : Alexandre Beaulieu et Vincent Philippon
 Date de creation : 27 avril 2010
 But du programme : M�thode de l'objet pi�ce
 ******************************************************************************************/

//Directives au pr�-processeur
#include "piece.h"
using namespace std;

//Constructeur
piece::piece(int equipe, SDL_Surface *image, bool bouger)
{
  init(equipe, image, bouger);
}

//Constructeur de copy
piece::piece(piece &p)
{
  init(p.getEquipe(), p.getImage(), p.getBouger());
}

//Destructeur
piece::~piece()
{
  init();
}

//Initialisateur
void piece::init(int equipe, SDL_Surface *image, bool bouger)
{
  setEquipe(equipe);
  setImage(image);
  setBouger(bouger);
}

//Setteur
void piece::setEquipe(int equipe)
{
  assert(equipe > -2 && equipe < 2);
  _equipe = equipe;
}

void piece::setImage(SDL_Surface *image)
{
  _image = image;
}

void piece::setBouger(bool bouger)
{
  _bouger = bouger;
}

//Getteur
int piece::getEquipe() const
{
  return _equipe;
}

SDL_Surface* piece::getImage() const
{
  return _image;
}

bool piece::getBouger() const
{
  return _bouger;
}

//Surcharge de l'op�rateur =
const piece& piece::operator=(const piece &p)
{
  init(p.getEquipe(), p.getImage(), p.getBouger());
  return *this;
}

//M�thode virtuel de destination
bool piece::destination(plateau sTabPlateau[8][8], int iXPiece, int iYPiece,
                        int iXDeplacement, int iYDeplacement, int iTour)
{
  return false;
}

//M�thode virtuel de type, retourne le type de la pi�ce
char piece::type() const
{
  return 'V';
}
