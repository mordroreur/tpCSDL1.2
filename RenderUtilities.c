#include "RenderUtilities.h"
#include "SDL_image.h"
#include "SDL_rotozoom.h"
#include "SDL_timer.h"
#include "SDL_video.h"
#include <SDL/SDL_video.h>
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
  SDL_Color Color = {R, G, B};
  SDL_Surface* surfaceMessage = TTF_RenderText_Solid(RobotoFont, s, Color);
  SDL_Rect Message_rect;
  float res = ((float)TailleEcranHaut * (float)size/100)/surfaceMessage->h;
  switch (center) {
  case 'n':
    Message_rect.x = (int)((float)(TailleEcranLong)/100 * x); 
    Message_rect.y = (int)((float)(TailleEcranHaut)/100 * y);
    break;
  case 'e':
    Message_rect.x = (int)((float)(TailleEcranLong)/100 * x - surfaceMessage->w*res); 
    Message_rect.y = (int)((float)(TailleEcranHaut)/100 * y);
    break;
  case 's':
    Message_rect.x = (int)((float)(TailleEcranLong)/100 * x - surfaceMessage->w*res); 
    Message_rect.y = (int)((float)(TailleEcranHaut)/100 * y - surfaceMessage->h*res);
    break;
  case 'w':
    Message_rect.x = (int)((float)(TailleEcranLong)/100 * x);
    Message_rect.y = (int)((float)(TailleEcranHaut)/100 * y - surfaceMessage->h*res);
    break;
  case 'c':
    Message_rect.x = (int)((float)(TailleEcranLong)/100 * x - surfaceMessage->w*(res/2)); 
    Message_rect.y = (int)((float)(TailleEcranHaut)/100 * y - surfaceMessage->h*(res/2));
    break;
  default:
    Message_rect.x = (int)((float)(TailleEcranLong)/100 * x); 
    Message_rect.y = (int)((float)(TailleEcranHaut)/100 * y);
    break;
  }

  SDL_Surface *tmp = rotozoomSurface(surfaceMessage, 0.0, res, 0);
  SDL_BlitSurface(tmp, NULL, renderer, &Message_rect);
  SDL_FreeSurface(tmp);
  SDL_FreeSurface(surfaceMessage);



  
}

void freeImageMalloc(){
  free(fileImage);
}

void *InitImage(void *CeciEstUneVariableVide){
  int nbImage = 10;
  fileImage = (SDL_Surface **)malloc(sizeof(SDL_Surface *) * nbImage);
  PixelXnb = (int *)malloc(sizeof(int) * nbImage);
  PixelYnb = (int *)malloc(sizeof(int) * nbImage);
  XImagenb = (int *)malloc(sizeof(int) * nbImage);
  YImagenb = (int *)malloc(sizeof(int) * nbImage);
  TotalImagenb = (int *)malloc(sizeof(int) * nbImage);

  
  fileImage[0] = IMG_Load("Res/Image/Menu/Play.png");PixelXnb[0] = 64; PixelYnb[0] = 16;XImagenb[0] = 1; YImagenb[0] = 4; TotalImagenb[0] = 4;
  fileImage[1] = IMG_Load("Res/Image/Menu/quitter.png");PixelXnb[1] = 64; PixelYnb[1] = 32;XImagenb[1] = 1; YImagenb[1] = 1; TotalImagenb[1] = 1;
  fileImage[2] = IMG_Load("Res/Image/Menu/quitterNo.png");PixelXnb[2] = 64; PixelYnb[2] = 32;XImagenb[2] = 1; YImagenb[2] = 1; TotalImagenb[2] = 1;
  fileImage[3] = IMG_Load("Res/Image/Menu/quitterYes.png");PixelXnb[3] = 64; PixelYnb[3] = 32;XImagenb[3] = 1; YImagenb[3] = 1; TotalImagenb[3] = 1;

  fileImage[4] = IMG_Load("Res/Image/CaseJeu/Sol.png");PixelXnb[4] = 16; PixelYnb[4] = 16;XImagenb[4] = 1; YImagenb[4] = 1; TotalImagenb[4] = 1;
  fileImage[5] = IMG_Load("Res/Image/CaseJeu/SolLock.png");PixelXnb[5] = 16; PixelYnb[5] = 16;XImagenb[5] = 1; YImagenb[5] = 1; TotalImagenb[5] = 1;
  fileImage[6] = IMG_Load("Res/Image/CaseJeu/mur.png");PixelXnb[6] = 16; PixelYnb[6] = 16;XImagenb[6] = 1; YImagenb[6] = 1; TotalImagenb[6] = 1;
  
  fileImage[7] = IMG_Load("Res/Image/CaseJeu/BoiteVide.png");PixelXnb[7] = 16; PixelYnb[7] = 16;XImagenb[7] = 1; YImagenb[7] = 1; TotalImagenb[7] = 1;
  fileImage[8] = IMG_Load("Res/Image/CaseJeu/BoiteLock.png");PixelXnb[8] = 16; PixelYnb[8] = 16;XImagenb[8] = 1; YImagenb[8] = 1; TotalImagenb[8] = 1;

  fileImage[9] = IMG_Load("Res/Image/VertAvant.png");PixelXnb[9] = 16; PixelYnb[9] = 16;XImagenb[9] = 4; YImagenb[9] = 1; TotalImagenb[9] = 4;
  


  EtapeActuelleDuJeu = 2;
  return NULL;
}


void DrawImage(int imagenb, float x, float y, float sizeX, float sizeY, char center, int etatPremier, float TimebeforeNext, int flip, int angle){


  SDL_Rect Image_rect;
  SDL_Rect keepImage;
  int imageVoulu = 0;

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

  
  switch (center) {
  case 'n':
    Image_rect.x = (int)((float)(TailleEcranLong)/100 * x); 
    Image_rect.y = (int)((float)(TailleEcranHaut)/100 * y);
    break;
  case 'e':
    Image_rect.x = (int)((float)(TailleEcranLong)/100 * x - (float)(PixelXnb[imagenb]) * resx); 
    Image_rect.y = (int)((float)(TailleEcranHaut)/100 * y);
    break;
  case 's':
    Image_rect.x = (int)((float)(TailleEcranLong)/100 * x - (float)(PixelXnb[imagenb]) * resx); 
    Image_rect.y = (int)((float)(TailleEcranHaut)/100 * y - (float)(PixelYnb[imagenb]) * resy);
    break;
  case 'w':
    Image_rect.x = (int)((float)(TailleEcranLong)/100 * x);
    Image_rect.y = (int)((float)(TailleEcranHaut)/100 * y - (float)(PixelYnb[imagenb]) * resy);
    break;
  case 'c':
    Image_rect.x = (int)((float)(TailleEcranLong)/100 * x - (float)(PixelXnb[imagenb]) * resx/2); 
    Image_rect.y = (int)((float)(TailleEcranHaut)/100 * y - (float)(PixelYnb[imagenb]) * resy/2);
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
  SDL_Surface *tmp = rotozoomSurfaceXY(fileImage[imagenb], 0.0, resx, resy, 0);
  SDL_BlitSurface(tmp, &keepImage, renderer, &Image_rect);
  SDL_FreeSurface(tmp);

  
  

  
  
  
}

////////SOKOBAN
