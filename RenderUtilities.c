#include "RenderUtilities.h"
#include "SDL_image.h"
#include "SDL_rotozoom.h"
#include "SDL_timer.h"
#include "SDL_video.h"
#include <stdio.h>

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


static SDL_Surface **fileImage;
static int *PixelXnb;
static int *PixelYnb;
static int *XImagenb;
static int *YImagenb;
static int *TotalImagenb;





long int getTime(){
  struct timespec tms;
  if (clock_gettime(CLOCK_REALTIME,&tms)) {
        return -1;
    }
  return ((tms.tv_sec*1000000) + (tms.tv_nsec/1000));
}


void DrawString(char *s, int x, int y, int size, char center, int R, int G, int B){
  int n = 0;
  while(s[n] != '\0'){
    n++;
  }
  
  
  SDL_Color Color = {R, G, B};
  SDL_Surface* surfaceMessage = TTF_RenderText_Solid(RobotoFont, s, Color); 
  SDL_Rect Message_rect;
  switch (center) {
  case 'n':
    Message_rect.x = (int)((float)(TailleEcranLong)/100 * x); 
    Message_rect.y = (int)((float)(TailleEcranHaut)/100 * y);
    Message_rect.w = (int)((float)(TailleEcranLong)/400 * size * n);
    Message_rect.h = (int)((float)(TailleEcranHaut)/100 * size);
    break;
  case 'e':
    Message_rect.x = (int)((float)(TailleEcranLong)/100 * x - (float)(TailleEcranLong)/400 * size * (n+1)); 
    Message_rect.y = (int)((float)(TailleEcranHaut)/100 * y);
    Message_rect.w = (int)((float)(TailleEcranLong)/400 * size * n);
    Message_rect.h = (int)((float)(TailleEcranHaut)/100 * size);
    break;
  case 's':
    Message_rect.x = (int)((float)(TailleEcranLong)/100 * x - (float)(TailleEcranLong)/400 * size * n); 
    Message_rect.y = (int)((float)(TailleEcranHaut)/100 * y - (float)(TailleEcranHaut)/100 * size);
    Message_rect.w = (int)((float)(TailleEcranLong)/400 * size * n);
    Message_rect.h = (int)((float)(TailleEcranHaut)/100 * size);
    break;
  case 'w':
    Message_rect.x = (int)((float)(TailleEcranLong)/100 * x);
    Message_rect.y = (int)((float)(TailleEcranHaut)/100 * y - (float)(TailleEcranHaut)/100 * size);
    Message_rect.w = (int)((float)(TailleEcranLong)/400 * size * n);
    Message_rect.h = (int)((float)(TailleEcranHaut)/100 * size);
    break;
  case 'c':
    Message_rect.x = (int)((float)(TailleEcranLong)/100 * x - ((float)(TailleEcranLong)/400 * size * n)/2); 
    Message_rect.y = (int)((float)(TailleEcranHaut)/100 * y - ((float)(TailleEcranHaut)/100 * size)/2);
    Message_rect.w = (int)((float)(TailleEcranLong)/400 * size * n);
    Message_rect.h = (int)((float)(TailleEcranHaut)/100 * size);
    break;
  default:
    Message_rect.x = (int)((float)(TailleEcranLong)/100 * x); 
    Message_rect.y = (int)((float)(TailleEcranHaut)/100 * y);
    Message_rect.w = (int)((float)(TailleEcranLong)/400 * size * n);
    Message_rect.h = (int)((float)(TailleEcranHaut)/100 * size);
    break;
  }

  SDL_BlitSurface(surfaceMessage, NULL, renderer, &Message_rect);
  SDL_FreeSurface(surfaceMessage);



  
}

void freeImageMalloc(){
  free(fileImage);
}

void InitImage(){
  int nbImage = 1;
  fileImage = (SDL_Surface **)malloc(sizeof(SDL_Surface *) * nbImage);
  PixelXnb = (int *)malloc(sizeof(int) * nbImage);
  PixelYnb = (int *)malloc(sizeof(int) * nbImage);
  XImagenb = (int *)malloc(sizeof(int) * nbImage);
  YImagenb = (int *)malloc(sizeof(int) * nbImage);
  TotalImagenb = (int *)malloc(sizeof(int) * nbImage);

  
  fileImage[0] = IMG_Load("Res/nyanCat.jpg");PixelXnb[0] = 301; PixelYnb[0] = 167;XImagenb[0] = 1; YImagenb[0] = 1; TotalImagenb[0] = 1;

  
}


void DrawImage(int imagenb, int x, int y, int sizeX, int sizeY, char center, int etatPremier, float TimebeforeNext, int flip, int angle){


  SDL_Rect Image_rect;
  SDL_Rect keepImage;
  int imageVoulu = 0;

  
  switch (center) {
  case 'n':
    Image_rect.x = (int)((float)(TailleEcranLong)/100 * x); 
    Image_rect.y = (int)((float)(TailleEcranHaut)/100 * y);
    break;
  case 'e':
    Image_rect.x = (int)((float)(TailleEcranLong)/100 * x - (float)(TailleEcranLong)/400 * sizeX); 
    Image_rect.y = (int)((float)(TailleEcranHaut)/100 * y);
    break;
  case 's':
    Image_rect.x = (int)((float)(TailleEcranLong)/100 * x - (float)(TailleEcranLong)/400 * sizeX); 
    Image_rect.y = (int)((float)(TailleEcranHaut)/100 * y - (float)(TailleEcranHaut)/100 * sizeY);
    break;
  case 'w':
    Image_rect.x = (int)((float)(TailleEcranLong)/100 * x);
    Image_rect.y = (int)((float)(TailleEcranHaut)/100 * y - (float)(TailleEcranHaut)/100 * sizeY);
    break;
  case 'c':
    Image_rect.x = (int)((float)(TailleEcranLong)/100 * x - ((float)(TailleEcranLong)/400 * sizeX)/2); 
    Image_rect.y = (int)((float)(TailleEcranHaut)/100 * y - ((float)(TailleEcranHaut)/100 * sizeY)/2);
    break;
  default:
    Image_rect.x = (int)((float)(TailleEcranLong)/100 * x); 
    Image_rect.y = (int)((float)(TailleEcranHaut)/100 * y);
    break;
  }

  if(TimebeforeNext == 0){
    imageVoulu = etatPremier;
  }else{
    imageVoulu = (etatPremier + ((int)((SDL_GetTicks()/((float)1000*TimebeforeNext)))%TotalImagenb[imagenb]))%TotalImagenb[imagenb];
  }

  float resx;
  float resy;
  
  if(sizeY == 0 && sizeX != 0){
    resx = ((float)TailleEcranLong * (float)sizeX/100)/PixelXnb[imagenb];
    resy = resx;
  }else if(sizeX == 0 && sizeY != 0){
    resy = ((float)TailleEcranHaut * (float)sizeY/100)/PixelYnb[imagenb];
    resx = resy;
  }else if(sizeX != 0 && sizeY != 0){
    resx = ((float)TailleEcranLong * (float)sizeX/100)/PixelXnb[imagenb];
    resy = ((float)TailleEcranHaut * (float)sizeY/100)/PixelYnb[imagenb];
  }else{
    resx = 1;
    resy = 1;
  }

  
  if(TotalImagenb[imagenb] != 1){
    int col = imageVoulu / XImagenb[imagenb];
    int line = imageVoulu % XImagenb[imagenb];
    keepImage.x = line*PixelXnb[imagenb]*resx;
    keepImage.y = col*PixelYnb[imagenb]*resy;
    keepImage.w = PixelXnb[imagenb]*resx;
    keepImage.h = PixelYnb[imagenb]*resy;
  }else {
    keepImage.x = 0;
    keepImage.y = 0;
    keepImage.w = (float)PixelXnb[imagenb]*resx;
    keepImage.h = (float)PixelYnb[imagenb]*resy;
  }

  if(flip == 1){
    resx = -resx;
  }else if(flip == 2){
    resy = -resy;
  }else if(flip == 3){
    resy = -resy;
    resx = -resx;
  }

  SDL_BlitSurface(rotozoomSurfaceXY(fileImage[imagenb], 0.0, resx, resy, 0), &keepImage, renderer, &Image_rect);

  
  

  
  
  
}

////////SOKOBAN
