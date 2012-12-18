/*****************************************************************************************
 En-tete du programme
 =====================
 fichier : sockets.h
 Auteur : Alexandre Beaulieu et Vincent Philippon
 Date de creation : 7 mai 2010
 Fichier d'ent�te des actions reli�es au sockets
 ******************************************************************************************/

#ifndef SOCKETS_H_
#define SOCKETS_H_

//Si on est sous windows
#if defined (WIN32)
  #include <winsock2.h>
  typedef int socklen_t;

//Si on est sous linux
#elif defined (linux)
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <unistd.h>
  #include <netdb.h>
  #define INVALID_SOCKET -1
  #define SOCKET_ERROR -1
  #define closesocket(s) close(s)
  typedef int SOCKET;
  typedef struct sockaddr_in SOCKADDR_IN;
  typedef struct sockaddr SOCKADDR;
  typedef struct hostent HOSTENT;
#endif

#include "affichage.h"
#include "piece.h"

using namespace std;
#define PORT 9001

//Param�tres pour le thread pour accepter la connexion
struct socketClientRecue
{
    SOCKET *cSock;  //Socket du client
    SOCKET *sock;  //Socket du serveur
    SOCKET *sockTransmition;  //Socket pour la transmition
    SOCKADDR_IN cSin;  //Contexte d'adressage du client
    socklen_t cRecsize;  //Place pris en m�moire par le contexte d'adressage
    int *iJoueur[2];  //D�termine si le joueur est les blancs ou non
    bool *bZoneSaisie;  //Si la zone de saisie de texte est visible
    SDL_Surface *ecran;  //Pointeur sur l'�cran
};

//Param�tres pour le thread pour se connecter
struct connexionAuServeur
{
    SOCKET *sock;  //Socket du client
    SOCKADDR_IN *sin;  //Contexte d'adressage du client
    int connexion,  //Si la connexion a r�ussi
        *iJoueur[2];  //D�termine si le joueur est les blancs ou non

};

//Parametres pour le thread de la r�ception de coordonn�e
struct receptionCoords
{
    SOCKET *sockTransmition;  //Par quelle socket les donn�e sont re�ues
    int *iXPiece;  //Coordon�e de la pi�ce dans les abscisses
    int *iYPiece;  //Coordon�e de la pi�ce dans les ordonn�es
    int *iXDestination;  //Coordon�e de la destination dans les abscisses
    int *iYDestination;  //Coordon�e de la destination dans les ordonn�es
    int *iAction;  //Fait-on l'action (donn�e transmise)
    bool *bAction;  //Fait-on l'action
    SDL_Surface *ecran;  //Pointeur sur l'�cran
    plateau *sTabPlateau[8][8];  //Plateau de jeu
};

//Parametres pour le thread de la r�ception de promotion
struct receptionProm
{
    SOCKET *sockTransmition;  //Par quelle socket les donn�e sont re�ues
    int *iPromotion;  //Promotion de la pi�ce
};

//Initialise Winsock sous windows
int initialiserLibSocket()
{
  //Si on est sous windows
#if defined (WIN32)
  WSADATA WSAData;
  int erreur = WSAStartup(MAKEWORD(2,2), &WSAData);
#else
  int erreur = 0;
#endif

  return !erreur;
}

//Configure la partie serveur
int configurationServeur(SOCKADDR_IN &sin, SOCKET &sock, socklen_t &recsize)
{
  sock = socket(AF_INET, SOCK_STREAM, 0);  //Cr�e la socket serveur

  //Si la socket est valide
  if (sock != INVALID_SOCKET)
  {
    sin.sin_addr.s_addr = htonl(INADDR_ANY );  // Adresse IP automatique
    sin.sin_family = AF_INET;  // Protocole familial (IP)
    sin.sin_port = htons(PORT);  // Listage du port
    if (bind(sock, (SOCKADDR*)&sin, recsize) != SOCKET_ERROR)
      return listen(sock, 5);
  }
  return SOCKET_ERROR;
}

//Configure la connexion client
void configurationClient(SOCKADDR_IN &sin, SOCKET &sock, const char* IP)
{
  //Si la socket est valide
  if (sock != INVALID_SOCKET)
  {
    sin.sin_addr.s_addr = inet_addr(IP);
    sin.sin_family = AF_INET;  // Protocole familial (IP)
    sin.sin_port = htons(PORT);  // Listage du port
  }
}

//Accepte la connexion du client
void* accepterConnexion(void *parametre)
{
  SDL_Rect positionTexte;  //Position du texte
  Uint32 couleurFondTexte = 12632256;  //Couleur du fond du texte
  SDL_Color couleurTexte =
    {128, 0, 0};  //Couleur du texte

  //Transforme le void* en socketClientRecue*
  socketClientRecue* client = (socketClientRecue*)parametre;

  positionTexte.x = 227;
  positionTexte.y = 245;
  positionTexte.w = 340;
  positionTexte.h = 34;

  *client->cSock = accept(*client->sock, (SOCKADDR*)&client->cSin,
                          &client->cRecsize);

  *client->sockTransmition = *client->cSock;
  *client->iJoueur[0] = 1;

  if (*client->bZoneSaisie)
    afficherTexte(client->ecran, "Un joueur s'est connect�, "
                  "veuillez appuyer sur une touche",
                  "papyrus.ttf", 13, couleurTexte, positionTexte,
                  couleurFondTexte, 0);

  return NULL;
}

//Se connecte au serveur
void* connecter(void *parametre)
{
  //Transforme le void* en connexionAuServeur*
  connexionAuServeur *client = (connexionAuServeur*)parametre;
  if (*client->iJoueur[0] != 1)
  {
    client->connexion = connect(*client->sock, (SOCKADDR*)client->sin,
                                sizeof(*client->sin));
    if (client->connexion != SOCKET_ERROR)
      *client->iJoueur[1] = -1;
  }
  return NULL;
}

//Trouve l'adresse IP de la machine h�te
const char* trouverIPLocal()
{

  char host[100];  //Nom de l'ordinateur
  HOSTENT* ip_resolue;  //IP sortie par gethostbyname
  SOCKADDR_IN valip;  //Adresse ip convertie a un format normal
  int i;  //Compteur

  //Recupere le nom d'h�te de l'ordinateur
  gethostname(host, sizeof(host));
  ip_resolue = gethostbyname(host);

  //Si on est connect�
  if (ip_resolue != NULL)
  {
    //Recuperation du dernier entier 'i' tel que ip_resolue->h_addr_list[compteur] n'est pas null
    for (i = 0; ip_resolue->h_addr_list[i + 1] != NULL; i++)
    {
    }
    memcpy(&valip.sin_addr.s_addr, ip_resolue->h_addr_list[i],
           ip_resolue->h_length);

    //Retourne l'adresse IP de la machine
    return inet_ntoa(valip.sin_addr);
  }

  //Retourne l'adresse de boucle locale
  return "127.0.0.1";
}

//Configure le rectangle et la couleur pour la zone de saisie de texte
void configurerTexteZoneSaisie(SDL_Rect &position, Uint32 &fond)
{
  fond = 16777215;  //blanc
  position.x = 265;
  position.y = 315;
  position.w = 269;
}

//Configure le rectangle et la couleur pour la zone de messages a l'utilisateur
void configurerTexteMessage(SDL_Rect &position, Uint32 &fond)
{
  fond = 12632256;  //gris
  position.x = 227;
  position.y = 245;
  position.w = 340;
}

//Se connecte au serveur    Retourne 0 si l'utilisateur quitte ou
//1 si l'ordinateur s'est connect�
bool connecterAuServeur(SDL_Surface *ecran, SOCKADDR_IN &sin, SOCKET &sock,
                        SOCKET &sockTransmition, int iJoueur[],
                        pthread_t threadAccept)
{
  SDL_Rect positionMessage;  //Position du message

  SDL_Color couleurTexte =
    {0, 0, 255};  //Couleur du texte

  Uint32 couleurFondTexte = 12632256;  //Arriere plan gris du texte

  SDL_Event event;  //�v�nements

  bool continuer = 1;  //Si on continue

  char toucheCpy[2] =
    {'\0'},  //Variable pour strcat
      ip[16] =
        {'\0'};  //Adresse IP saisie

  int cpt,  //Compteur
      touche;  //Touche appuy�e

  pthread_t thread;  //Variable pour le thread

  connexionAuServeur client;  //Param�tres du thread

  //Donn�e de la structure pour le thread
  client.sock = &sock;
  client.sin = &sin;
  client.iJoueur[0] = &iJoueur[0];
  client.iJoueur[1] = &iJoueur[1];

  //Affiche le message de saisie d'IP
  positionMessage.x = positionMessage.y = 225;
  positionMessage.h = 150;
  positionMessage.w = 350;
  afficherImage(ecran, "SaisieIP.png", &positionMessage);

  //Affiche l'adresse IP de l'ordinateur
  positionMessage.x = 400;
  positionMessage.y = 227;
  positionMessage.w = 123;
  positionMessage.h = 20;
  afficherTexte(ecran, trouverIPLocal(), "papyrus.ttf", 15, couleurTexte,
                positionMessage, couleurFondTexte, 1);

  //Configuration de la couleur et l'emplacement de la saisie de texte
  couleurTexte.r = 128;
  couleurTexte.b = 0;
  positionMessage.h = 34;
  configurerTexteZoneSaisie(positionMessage, couleurFondTexte);

  SDL_EnableKeyRepeat(100, 50);

  //Si un joueur s'est connect�
  if (iJoueur[0] == 1)
  {
    configurerTexteMessage(positionMessage, couleurFondTexte);
    afficherTexte(ecran,
                  "Un joueur s'est connect�, veuillez appuyer sur une touche",
                  "papyrus.ttf", 13, couleurTexte, positionMessage,
                  couleurFondTexte, 0);
    couleurFondTexte = SDL_MapRGB(ecran->format, 255, 255, 255);
    configurerTexteZoneSaisie(positionMessage, couleurFondTexte);
  }

  do
  {
    continuer = 1;
    //Tant que continuer est vrais
    while (continuer)
    {
      //Si un joueur s'est connect�
      if (iJoueur[0] == 1)
      {
        while (continuer)
        {
          SDL_WaitEvent(&event);
          switch (event.type)
          {
            case SDL_QUIT:
              return 0;
              break;
            case SDL_KEYDOWN:
              return 1;
              break;
          }
        }
      }

      SDL_WaitEvent(&event);
      switch (event.type)
      {
        case SDL_QUIT:
          return 0;
          break;
        case SDL_KEYDOWN:
          touche = event.key.keysym.sym;
          switch (touche)
          {
            case '\r':  //Si l'utilisateur a fait retour
              continuer = 0;
              break;
            case SDLK_KP_ENTER:  //Si l'utilisateur a fait retour
              continuer = 0;  //sur le pav� num�rique
              break;
            case SDLK_ESCAPE:  //Si l'utilisateur fait �chap
              return 0;
              break;
            case '\b':  //Si l'utilisateur a fait retour arri�re
              for (cpt = 0; ip[cpt] != '\0'; cpt++)
                ;
              if (cpt != 0)
                ip[cpt - 1] = '\0';
              break;
            default:
              //Si c'est un nombre du pav� num�rique
              if (touche >= 256 && touche <= 265)
                touche -= 208;

              //si c'est le point du pav� num�rique
              else if (touche == 266)
                touche = 46;

              //Si c'est un chiffre ou un point
              if ((touche >= 48 && touche <= 57) || touche == 46)
              {
                toucheCpy[0] = touche;
                if (ip[14] == 0)
                  strcat(ip, toucheCpy);
              }
          }
          //Affiche le texte saisie
          afficherTexte(ecran, ip, "papyrus.ttf", 20, couleurTexte,
                        positionMessage, couleurFondTexte, 1);
          break;
      }
    }
    configurationClient(sin, sock, ip);

    configurerTexteMessage(positionMessage, couleurFondTexte);
    afficherTexte(ecran, "Veuillez patienter pendant la connexion",
                  "papyrus.ttf", 15, couleurTexte, positionMessage,
                  couleurFondTexte, 0);

    client.connexion = 2;
    pthread_create(&thread, NULL, connecter, &client);
    couleurFondTexte = SDL_MapRGB(ecran->format, 255, 255, 255);
    configurerTexteZoneSaisie(positionMessage, couleurFondTexte);
    continuer = 1;

    //Tant que continuer est vrais
    while (continuer)
    {
      //Si la connexion a �chou�
      if (client.connexion == SOCKET_ERROR)
      {
        configurerTexteMessage(positionMessage, couleurFondTexte);
        afficherTexte(ecran,
                      "La connexion a �chou�e, veuillez saisir de nouveau",
                      "papyrus.ttf", 15, couleurTexte, positionMessage,
                      couleurFondTexte, 0);
        configurerTexteZoneSaisie(positionMessage, couleurFondTexte);
        continuer = 0;
      }

      //Si un joueur s'est connect�
      else if (iJoueur[0] == 1)
      {
        //Tant que continuer est vrais
        while (continuer)
        {
          SDL_WaitEvent(&event);
          switch (event.type)
          {
            case SDL_QUIT:
              return 0;
              break;
            case SDL_KEYDOWN:
              return 1;
              break;
          }
        }
      }
      else if (client.connexion == 0)
        continuer = 0;

      SDL_PollEvent(&event);
      switch (event.type)
      {
        case SDL_QUIT:
          return 0;
          break;
      }
    }
  }while (!(client.connexion == 0 || client.connexion == 1));
  //Tant que la connexion a �chou�

  pthread_cancel(threadAccept);
  sockTransmition = sock;
  return 1;
}

//Envoie un int
int envoyerInt(int sock, int &donnee)
{
  int resultat;
  static int grandeur = sizeof(int);
  unsigned char *donneeEnvoi = (unsigned char*)malloc(grandeur);

  //Transforme le int pour l'envoyer, et le met dans un char
  //� cause de l'endianness et la grandeur des variables changeant
  //d'un ordinateur � l'autre
  for (int i = 0; i < grandeur; i++)
    donneeEnvoi[i] = (donnee >> ((grandeur - i) * 8)) & 0xFF;

  resultat = send(sock, (char*)donneeEnvoi, grandeur, 0);
  free(donneeEnvoi);
  return resultat;
}

//Re�oit un int
int recevoirInt(int sock, int &donnee)
{
  int resultat;
  static int grandeur = sizeof(int);
  unsigned char *donneeRecue = (unsigned char*)malloc(grandeur);

  resultat = recv(sock, (char*)donneeRecue, grandeur, 0);

  //Reconstruit le int re�u qui a �t� chang� a l'envoi
  //� cause de l'endianness et la grandeur des variables changeant
  //d'un ordinateur � l'autre
  donnee = 0;
  for (int i = 0; i < grandeur; i++)
    donnee |= (donneeRecue[i] << ((grandeur - i) * 8));

  free(donneeRecue);
  return resultat;
}

//Re�oit les coordonn�e
void* recevoirCoordonnee(void *parametre)
{
  receptionCoords *donnee = (receptionCoords*)parametre;

  //Tant que la r�ception retourne SOCKET_ERROR
  while (recevoirInt(*donnee->sockTransmition, *donnee->iXPiece) == SOCKET_ERROR)
    ;
  while (recevoirInt(*donnee->sockTransmition, *donnee->iYPiece) == SOCKET_ERROR)
    ;

  afficherImage(
      donnee->ecran, "SelectionAdverse.PNG",
      &donnee->sTabPlateau[*donnee->iYPiece][*donnee->iXPiece]->posCase);

  while (recevoirInt(*donnee->sockTransmition, *donnee->iAction) == SOCKET_ERROR)
    ;

  //Si iAction n'est pas 0
  if (*donnee->iAction)
  {
    *donnee->bAction = 1;
    while (recevoirInt(*donnee->sockTransmition, *donnee->iXDestination)
        == SOCKET_ERROR)
      ;
    while (recevoirInt(*donnee->sockTransmition, *donnee->iYDestination)
        == SOCKET_ERROR)
      ;
  }
  else
    *donnee->bAction = 0;

  return NULL;
}

//Re�oit la promotion de la pi�ce
void* recevoirPromotion(void *parametre)
{
  receptionProm *donnee = (receptionProm*)parametre;

  //Tant que la r�ception retourne SOCKET_ERROR
  while (recevoirInt(*donnee->sockTransmition, *donnee->iPromotion)
      == SOCKET_ERROR)
    ;

  return NULL;
}

//D�connecter sockets
void deconnecterSocket(SOCKET &sockServeur, SOCKET &sockClient)
{
  shutdown(sockClient, 2);
  shutdown(sockServeur, 2);
  closesocket(sockClient);
  closesocket(sockServeur);
}

//Ferme les sockets et clean WSA sous windows
void fermetureSocket(SOCKET& sockServeur, SOCKET& sockClient)
{
  deconnecterSocket(sockServeur, sockServeur);

  //Si on est sous windows
  #if defined (WIN32)
    WSACleanup();
  #endif
}

#endif
