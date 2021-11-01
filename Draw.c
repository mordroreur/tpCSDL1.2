#include "Draw.h"
#include "RenderUtilities.h"

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

extern level Actulvl; /* Niveau actuelle sur lequel on joue */

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////


static inline int max(int a, int b){if(a < b){return b;}else{return a;}}
static inline int min(int a, int b){if(a > b){return b;}else{return a;}}


void LoadingScreen(){
  SDL_FillRect(renderer, NULL, SDL_MapRGB(renderer->format, 255, 255, 255)); 
  DrawString("Loading", 50, 50, 10, 'c', 0, 0, 0);
  int nb3 = SDL_GetTicks()/10;
  int nb = (nb3) % 100 - 50;
  int nb2 = (nb3+35)% 98 - 49;
  nb3 = (nb3+65)% 96 - 48;
  SDL_Rect rect;
  rect.y = TailleEcranHaut/2 + (float)TailleEcranLong/2000 * abs(nb) - TailleEcranLong/100;
  rect.x = TailleEcranLong/2 + TailleEcranLong/100*11;
  rect.h = TailleEcranHaut/100;
  rect.w = TailleEcranLong/100;
  SDL_FillRect(renderer, &rect, SDL_MapRGB(renderer->format, 0, 0, 0));
  rect.y = TailleEcranHaut/2 + (float)TailleEcranLong/2000 * abs(nb2) - TailleEcranLong/100;
  rect.x = TailleEcranLong/2 + TailleEcranLong/100*13;
  SDL_FillRect(renderer, &rect, SDL_MapRGB(renderer->format, 0, 0, 0));
  rect.y = TailleEcranHaut/2 + (float)TailleEcranLong/2000 * abs(nb3) - TailleEcranLong/100;
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
    if((mousX < (float)TailleEcranLong/100 *75 && mousX > (float)TailleEcranLong/100 * 25) && (mousY < (float)TailleEcranHaut/100*25 + (float)TailleEcranLong/400*50 && mousY > (float)TailleEcranHaut/100 *25)){
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

  if((mousX < (float)TailleEcranLong/100 *75 -((float)TailleEcranLong/2)/64 *6  && mousX > (float)TailleEcranLong/100 * 25 + ((float)TailleEcranLong/2)/64 *38) && (mousY < (float)TailleEcranHaut/100*(50-100/6) + ((float)TailleEcranLong/4)/32 * 28 && mousY > (float)TailleEcranHaut/100 *(50-100/6) + ((float)TailleEcranLong/4)/32 * 19)){
    DrawImage(3, 25, 50-100/6, 50, 0, 'n', 0, 0, 0, 0);
  }else if((mousX < (float)TailleEcranLong/100 *75 -((float)TailleEcranLong/2)/64 *38  && mousX > (float)TailleEcranLong/100 * 25 + ((float)TailleEcranLong/2)/64 *6) && (mousY < (float)TailleEcranHaut/100*(50-100/6) + ((float)TailleEcranLong/4)/32 * 28 && mousY > (float)TailleEcranHaut/100 *(50-100/6) + ((float)TailleEcranLong/4)/32 * 19)){
    DrawImage(2, 25, 50-100/6, 50, 0, 'n', 0, 0, 0, 0);
  }else {
    DrawImage(1, 25, 50-100/6, 50, 0, 'n', 0, 0, 0, 0);
  }


  
  SDL_FreeSurface(tmp);
  

  
}

void DrawCreaLVL(){
  int mousX;
  int mousY;
  SDL_GetMouseState(&mousX, &mousY);

  
  SDL_FillRect(renderer, NULL, SDL_MapRGB(renderer->format, 0, 0, 0));
  
}



static float camx = 8;
static float camy = 4;

void Draw1player(){
  int nbBlockY = 8;
  int nbBlockX = 16;
  for(int i = 0; i < nbBlockX; i++){
    for(int j = 0; j < nbBlockY; j++){
      int image;
      if((camx-nbBlockX/2 +j >= 0 && camx-nbBlockX/2 +j < Actulvl.TerX) && (camy-nbBlockY/2 +i >= 0 && camy-nbBlockY/2 +i < Actulvl.TerY)){
	image = getimage(Actulvl.ter.cellule[(int)camx-nbBlockX/2+j][(int)camy-nbBlockY/2+i]);
      }else {
	image = 4;
      }
      DrawImage(image, 100/8*i*((float)TailleEcranHaut/TailleEcranLong), (float)100/8 * j, 0, (float)100/8, 'n', 0, 0, 0, 0);
    }
  }

  int nb = 0;
  while(nb < Actulvl.maxEnti && Actulvl.enti[nb].type != -5){
    DrawImage(getimageEnti(Actulvl.enti[nb]), 100/8*(Actulvl.enti[nb].x-camx+nbBlockX/2)*((float)TailleEcranHaut/TailleEcranLong), (float)100/8 * (Actulvl.enti[nb].y-camy+nbBlockY/2), 0, (float)100/8, 'n', 0, 0, 0, 0);
    nb++;
  }




  
}

int getimageEnti(entite e){
  switch(e.type){
  case 1:return 9;
  case 5:return 7;
  case 6:return 8;
  default:return 4;
  }
}

int getimage(cellule l){
  switch(l.type){
  case 0:return 4;
  case 1:return 5;
  case 5:return 6;
  default:return 4;
  }
}
