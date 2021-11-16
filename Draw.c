#include "Draw.h"
#include "RenderUtilities.h"
#include <SDL/SDL_video.h>
#include <stdlib.h>

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

extern int HaveType; /* Valeur de la case sous le pointeur lors de la creation */

extern float camx; /* Décalage de la caméra en X */
extern float camy; /* Décalage de la caméra en Y */

extern int nbBlockY; /*Nb Blocks a afficher sur la hauteur de l'écran*/
extern int nbBlockX; /*Nb Blocks a afficher sur la longueur de l'écran*/


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////


//static inline int max(int a, int b){if(a < b){return b;}else{return a;}}
//static inline int min(int a, int b){if(a > b){return b;}else{return a;}}


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
  static int CreateStape = 0;

  int mousX;
  int mousY;
  SDL_GetMouseState(&mousX, &mousY);

  SDL_FillRect(renderer, NULL, SDL_MapRGB(renderer->format, 255, 255, 0));

  if(LastTick < SDL_GetTicks()-250){
    LastTick = SDL_GetTicks();
    if((mousX < (float)TailleEcranLong/100 *75 && mousX > (float)TailleEcranLong/100 * 25) && (mousY < (float)TailleEcranHaut/100*10 + (float)TailleEcranLong/400*50 && mousY > (float)TailleEcranHaut/100 *10)){
      if(PlayStape != 3){
	PlayStape++;
      }
    }else{
      if(PlayStape != 0){
	PlayStape--;
      }
    }
    if((mousX < (float)TailleEcranLong/100 *75 && mousX > (float)TailleEcranLong/100 * 25) && (mousY < (float)TailleEcranHaut/100*35 + (float)TailleEcranLong/400*50 && mousY > (float)TailleEcranHaut/100 *35)){
      if(CreateStape != 3){
	CreateStape++;
      }
    }else{
      if(CreateStape != 0){
	CreateStape--;
      }
    }
  }   
  DrawImage(0, 25, 10, 50, 0, 'n', PlayStape, 0, 0, 0);   
  DrawImage(0, 25, 35, 50, 0, 'n', CreateStape, 0, 0, 0);
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

int compareEntitiYlvl(const void *nb1, const void *nb2){
  return (Actulvl.enti[*(int *)nb1].y - Actulvl.enti[*(int *)nb2].y<0)?-1:1;
}


void DrawCreaLVL(){
  int mousX;
  int mousY;
  SDL_GetMouseState(&mousX, &mousY);

  int* Whereenti;
  int actuEntiDes = 0;
  int nb = 0;

  
   Whereenti = (int *)malloc(Actulvl.maxEnti*sizeof(int));


  while(nb < Actulvl.maxEnti && Actulvl.enti[nb].type != -5){
    if(Actulvl.enti[nb].type != -1){
      Whereenti[actuEntiDes++] = nb;
    }
    nb++;
  }

  Whereenti[actuEntiDes] = -1;
  qsort(Whereenti, actuEntiDes, sizeof(int), compareEntitiYlvl);

  actuEntiDes = 0;
  
  SDL_FillRect(renderer, NULL, SDL_MapRGB(renderer->format, 50, 50, 50));

  for(int j = 0; j < nbBlockY-2; j++){
    for(int i = 0; i < nbBlockX; i++){
      int image;
      if((camx-nbBlockX/2 +j >= 0 && camx-nbBlockX/2 +j < Actulvl.TerX) && (camy-nbBlockY/2 +i >= 0 && camy-nbBlockY/2 +i < Actulvl.TerY) && Actulvl.ter.cellule[(int)camx-nbBlockX/2+j][(int)camy-nbBlockY/2+i].type != -3){
	image = getimage(Actulvl.ter.cellule[(int)camx-nbBlockX/2+j][(int)camy-nbBlockY/2+i]);
	DrawImage(image, 100.0/nbBlockY*(i-((float)camx-(int)camx))*((float)TailleEcranHaut/TailleEcranLong),
		  100.0/nbBlockY * (j-((float)camy-(int)camy)), 100.1/nbBlockY * ((float)TailleEcranHaut/TailleEcranLong), 0, 'n', 0, 0, 0, 0);
      }
    }
    
    while(Whereenti[actuEntiDes] != -1 && Actulvl.enti[Whereenti[actuEntiDes]].y < j-0.85 - 11 + camx){
      nb = Whereenti[actuEntiDes];
      DrawImage(getimageEnti(Actulvl.enti[nb]), 100.0/nbBlockY*((float)Actulvl.enti[nb].x-camy+nbBlockY/2.0)*((float)TailleEcranHaut/TailleEcranLong),
	      100.0/nbBlockY * ((float)Actulvl.enti[nb].y-camx+nbBlockX/2), 100.0/nbBlockY * ((float)TailleEcranHaut/TailleEcranLong), 0, 'n', 0, 0, 0, 0);
      actuEntiDes++;
      }
  }


   if(HaveType != -1 && mousY < TailleEcranHaut - 200.0/nbBlockY*TailleEcranHaut/100.0){
    DrawImage(HaveType, 100.0/nbBlockY*((int)(mousX/((float)TailleEcranHaut/nbBlockY))-((float)camx-(int)camx))*((float)TailleEcranHaut/TailleEcranLong),
	      (float)100/nbBlockY * ((int)((mousY)/((float)TailleEcranHaut/nbBlockY)) -((float)camy-(int)camy)), 100.0/nbBlockY * ((float)TailleEcranHaut/TailleEcranLong), 0, 'n', 0, 0, 0, 0);
    }
  
  SDL_Rect re;
  re.x = 0;
  re.w = TailleEcranLong;
  re.y = (100 - (float)200/nbBlockY)*((float)(TailleEcranHaut)/100);
  re.h = TailleEcranLong-re.y;
  SDL_FillRect(renderer, &re, SDL_MapRGB(renderer->format, 150, 150, 150));

  for(int i = 0; i < 3; i++){
    DrawImage(i+4, (float)200/nbBlockY * (i+0.5) * ((float)TailleEcranHaut/TailleEcranLong), 100 - (float)100/nbBlockY, (float)100/(nbBlockY) * ((float)TailleEcranHaut/TailleEcranLong), 0, 'c', 0, 0, 0, 0);
  }

  for(int i = 0; i < 2; i++){
    DrawImage(4, (float)200/nbBlockY * (i+3.5) * ((float)TailleEcranHaut/TailleEcranLong), 100 - (float)100/nbBlockY, (float)100/(nbBlockY) * ((float)TailleEcranHaut/TailleEcranLong), 0, 'c', 0, 0, 0, 0);
    DrawImage((i*2)+7, (float)200/nbBlockY * (i+3.5) * ((float)TailleEcranHaut/TailleEcranLong), 100 - (float)100/nbBlockY, (float)100/(nbBlockY) * ((float)TailleEcranHaut/TailleEcranLong), 0, 'c', 0, 0, 0, 0);
  }


  
 
  
}





void Draw1player(){
  int* Whereenti;
  int actuEntiDes = 0;
  int nb = 0;

  
  Whereenti = (int *)malloc(Actulvl.maxEnti*sizeof(int));


  while(nb < Actulvl.maxEnti && Actulvl.enti[nb].type != -5){
    if(Actulvl.enti[nb].type != -1){
      Whereenti[actuEntiDes++] = nb;
    }
    nb++;
  }

  Whereenti[actuEntiDes] = -1;
  qsort(Whereenti, actuEntiDes, sizeof(int), compareEntitiYlvl);

  actuEntiDes = 0;
  
  SDL_FillRect(renderer, NULL, SDL_MapRGB(renderer->format, 50, 50, 50));
  

  for(int j = 0; j < nbBlockY; j++){
    for(int i = 0; i < nbBlockX; i++){
      int image;
      if((camx-nbBlockX/2 +j >= 0 && camx-nbBlockX/2 +j < Actulvl.TerX) && (camy-nbBlockY/2 +i >= 0 && camy-nbBlockY/2 +i < Actulvl.TerY) && Actulvl.ter.cellule[(int)camx-nbBlockX/2+j][(int)camy-nbBlockY/2+i].type != -3){
	image = getimage(Actulvl.ter.cellule[(int)camx-nbBlockX/2+j][(int)camy-nbBlockY/2+i]);
	DrawImage(image, 100.0/nbBlockY*(i-((float)camx-(int)camx))*((float)TailleEcranHaut/TailleEcranLong),
		  100.0/nbBlockY * (j-((float)camy-(int)camy)), 100.1/nbBlockY * ((float)TailleEcranHaut/TailleEcranLong), 0, 'n', 0, 0, 0, 0);
      }
    }
    
    while(Whereenti[actuEntiDes] != -1 && Actulvl.enti[Whereenti[actuEntiDes]].y < j-0.85 - 11 + camx){
      nb = Whereenti[actuEntiDes];
      DrawImage(getimageEnti(Actulvl.enti[nb]), 100.0/nbBlockY*((float)Actulvl.enti[nb].x-camy+nbBlockY/2.0)*((float)TailleEcranHaut/TailleEcranLong),
	      100.0/nbBlockY * ((float)Actulvl.enti[nb].y-camx+nbBlockX/2), 100.0/nbBlockY * ((float)TailleEcranHaut/TailleEcranLong), 0, 'n', 0, 0, 0, 0);
      actuEntiDes++;
      }
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
