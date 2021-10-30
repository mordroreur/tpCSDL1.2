#include "Draw.h"

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////      Recuperation des variables globale     ///////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////



extern int EtapeActuelleDuJeu; /* 0 = fin; 1 = Loading Screen... */

extern int TailleEcranLong; /* Taille de l'ecran en nombre de pixel de gauche a droite */
extern int TailleEcranHaut; /* Taille de l'ecran en nombre de pixel de haut en bas */

extern SDL_Surface * renderer; /* Ecran sur lequel on dessine */

extern SDL_Event event; /* Les events que l'on récupèrera via SDL. */

extern int DEBUG; /* 1 : affiche le debug */

extern TTF_Font *RobotoFont; /* Font utilise pour ecrire dans le programme */

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

inline int abs(int nb){if(nb < 0){return -nb;}else{return nb;}}


void LoadingScreen(){
  SDL_FillRect(renderer, NULL, SDL_MapRGB(renderer->format, 255, 255, 255)); 
  DrawString("Loading", 50, 50, 6, 'c', 0, 0, 0);
  int nb3 = SDL_GetTicks()/10;
  int nb = (nb3) % 100 - 50;
  int nb2 = (nb3+35)% 98 - 49;
  nb3 = (nb3+65)% 96 - 48;
  SDL_Rect rect;
  rect.y = TailleEcranHaut/2 + TailleEcranLong/1000 * abs(nb) - TailleEcranLong/100;
  rect.x = TailleEcranLong/2 + TailleEcranLong/100*11;
  rect.h = TailleEcranHaut/100;
  rect.w = TailleEcranLong/100;
  SDL_FillRect(renderer, &rect, SDL_MapRGB(renderer->format, 0, 0, 0));
  rect.y = TailleEcranHaut/2 + TailleEcranLong/1000 * abs(nb2) - TailleEcranLong/100;
  rect.x = TailleEcranLong/2 + TailleEcranLong/100*13;
  SDL_FillRect(renderer, &rect, SDL_MapRGB(renderer->format, 0, 0, 0));
  rect.y = TailleEcranHaut/2 + TailleEcranLong/1000 * abs(nb3) - TailleEcranLong/100;
  rect.x = TailleEcranLong/2 + TailleEcranLong/100*15;
  SDL_FillRect(renderer, &rect, SDL_MapRGB(renderer->format, 0, 0, 0));
}

void DrawMenu(){
  


  
}
