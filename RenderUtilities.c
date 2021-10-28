#include "RenderUtilities.h"
#include "SDL_image.h"
#include "SDL_rotozoom.h"
#include "SDL_timer.h"
#include "SDL_video.h"

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
    Message_rect.x = (int)((float)(TailleEcranLong)/100 * x - (float)(TailleEcranLong)/400 * size * n); 
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

  
  fileImage[0] = IMG_Load("Res/nyanCat.jpg");PixelXnb[0] = 16; PixelYnb[0] = 16;XImagenb[0] = 4; YImagenb[0] = 1; TotalImagenb[0] = 1;

  
}


void DrawImage(int imagenb, int x, int y, int size, char center, int etatPremier, float TimebeforeNext, int flip){


  SDL_Rect Image_rect;
  SDL_Rect keepImage;
  int imageVoulu = 0;

  
  switch (center) {
  case 'n':
    Image_rect.x = (int)((float)(TailleEcranLong)/100 * x); 
    Image_rect.y = (int)((float)(TailleEcranHaut)/100 * y);
    break;
  case 'e':
    Image_rect.x = (int)((float)(TailleEcranLong)/100 * x - (float)(TailleEcranLong)/400 * size); 
    Image_rect.y = (int)((float)(TailleEcranHaut)/100 * y);
    break;
  case 's':
    Image_rect.x = (int)((float)(TailleEcranLong)/100 * x - (float)(TailleEcranLong)/400 * size); 
    Image_rect.y = (int)((float)(TailleEcranHaut)/100 * y - (float)(TailleEcranHaut)/100 * size);
    break;
  case 'w':
    Image_rect.x = (int)((float)(TailleEcranLong)/100 * x);
    Image_rect.y = (int)((float)(TailleEcranHaut)/100 * y - (float)(TailleEcranHaut)/100 * size);
    break;
  case 'c':
    Image_rect.x = (int)((float)(TailleEcranLong)/100 * x - ((float)(TailleEcranLong)/400 * size)/2); 
    Image_rect.y = (int)((float)(TailleEcranHaut)/100 * y - ((float)(TailleEcranHaut)/100 * size)/2);
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

  size = (TailleEcranHaut/100 * size)/PixelYnb[imagenb];
  printf("%d\n", size);
  
  if(TotalImagenb[imagenb] != 1){
    int col = imageVoulu / XImagenb[imagenb];
    int line = imageVoulu % XImagenb[imagenb];
    keepImage.x = line*PixelXnb[imagenb]*size;
    keepImage.y = col*PixelYnb[imagenb]*size;
    keepImage.w = PixelXnb[imagenb]*size;
    keepImage.h = PixelYnb[imagenb]*size;
    SDL_BlitSurface(rotozoomSurface(fileImage[imagenb], 1.0, size, 0), &keepImage, renderer, &Image_rect);
  }else {
    SDL_BlitSurface(rotozoomSurface(fileImage[imagenb], 0.0, size, 0), NULL, renderer, &Image_rect);
  }


  
  

  
  
  
}

