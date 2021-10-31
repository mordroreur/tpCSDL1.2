#include "Draw.h"
#include "RenderUtilities.h"
#include <SDL/SDL_mouse.h>
#include <SDL/SDL_timer.h>
#include <SDL/SDL_video.h>

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
  DrawString("Loading", 50, 50, 10, 'c', 0, 0, 0);
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
  static int LastTick = 0;
  static int PlayStape = 0;

  int mousX;
  int mousY;
  SDL_GetMouseState(&mousX, &mousY);

  SDL_FillRect(renderer, NULL, SDL_MapRGB(renderer->format, 255, 255, 0));

  if(LastTick < SDL_GetTicks()-250){
    LastTick = SDL_GetTicks();
    if((mousX < TailleEcranLong/100 *75 && mousX > TailleEcranLong/100 * 25) && (mousY < (float)TailleEcranHaut/100*25 + TailleEcranLong/400*50 && mousY > (float)TailleEcranHaut/100 *25)){
      if(PlayStape != 3){
	PlayStape++;
      }
    }else{
      if(PlayStape != 0){
	PlayStape--;
      }
    }
  } 
  
  
  DrawImage(0, 25, 25, 50, 0, 'n', PlayStape, 0, 0, 0);
}

void DrawExit(){

  SDL_Surface *tmp = SDL_CreateRGBSurface(0, TailleEcranLong, TailleEcranHaut, 32, 0, 0, 0, 0);
  int mousX;
  int mousY;
  SDL_GetMouseState(&mousX, &mousY);

  SDL_SetAlpha(tmp, SDL_SRCALPHA, 200);
  SDL_Rect r;
  r.x = 0;
  r.y = 0;

  SDL_BlitSurface(tmp, NULL, renderer, &r);

  if((mousX < TailleEcranLong/100 *70.5 && mousX > TailleEcranLong/100 * 54.9) && (mousY < (float)TailleEcranHaut/100*54.1 + TailleEcranLong/600*44.8 && mousY > (float)TailleEcranHaut/100 *54.1)){
    DrawImage(3, 50, 50, 50, 0, 'c', 0, 0, 0, 0);
  }else if((mousX < TailleEcranLong/100 *45.5 && mousX > TailleEcranLong/100 * 29.6) && (mousY < (float)TailleEcranHaut/100*54.1 + TailleEcranLong/400*44.8 && mousY > (float)TailleEcranHaut/100 *54.1)){
    DrawImage(2, 50, 50, 50, 0, 'c', 0, 0, 0, 0);
  }else {
    DrawImage(1, 50, 50, 50, 0, 'c', 0, 0, 0, 0);
  }


  
  SDL_FreeSurface(tmp);
  

  
}

void DrawCreaLVL(){
  int mousX;
  int mousY;
  SDL_GetMouseState(&mousX, &mousY);

  
  SDL_FillRect(renderer, NULL, SDL_MapRGB(renderer->format, 0, 0, 0));
  
}
