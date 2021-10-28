#include "main.h"

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////       Declaration des variables globale     ///////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////


int EtapeActuelleDuJeu; /* 0 = fin; 1 = Loading Screen... */

int TailleEcranLong = 1200; /* Taille de l'ecran en nombre de pixel de gauche a droite */
int TailleEcranHaut= 750; /* Taille de l'ecran en nombre de pixel de haut en bas */

SDL_Surface * renderer = NULL; /* Ecran sur lequel on dessine */

SDL_Event event; /* Les events que l'on récupèrera via SDL. */

int DEBUG = 1; /* 1 : affiche le debug*/

TTF_Font *RobotoFont = NULL; /* Font utilise pour ecrire dans le programme */

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

int main()
{
  //  srand(time(NULL)); // in case we need random

  EtapeActuelleDuJeu = 1;

  BouclePrincipaleDuJeu();

  
  
}
