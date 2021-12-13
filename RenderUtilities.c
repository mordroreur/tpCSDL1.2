#include "RenderUtilities.h"
#include "SDL_ttf.h"


static SDL_Surface **fileImage;
static int *PixelXnb;
static int *PixelYnb;
static int *XImagenb;
static int *YImagenb;
static int *ImYoffset;
static int *ImXoffset;
static int *TotalImagenb;

TTF_Font *RobotoFont;


long int getTime(){
  struct timespec tms;
  if (clock_gettime(CLOCK_REALTIME,&tms)) {
        return -1;
    }
  return ((tms.tv_sec*1000000) + (tms.tv_nsec/1000));
}


void DrawString(char *s, int x, int y, int size, char center, int R, int G, int B, screen W){
  //TTF_SizeText();
  SDL_Color Color = {R, G, B};
  SDL_Surface* surfaceMessage = TTF_RenderText_Solid(RobotoFont, s, Color);
  SDL_Rect Message_rect;
  float res = ((float)W.TailleY * (float)size/100)/surfaceMessage->h;
  switch (center) {
  case 'n':
    Message_rect.x = (int)((float)(W.TailleX)/100 * x); 
    Message_rect.y = (int)((float)(W.TailleY)/100 * y);
    break;
  case 'e':
    Message_rect.x = (int)((float)(W.TailleX)/100 * x - surfaceMessage->w*res); 
    Message_rect.y = (int)((float)(W.TailleY)/100 * y);
    break;
  case 's':
    Message_rect.x = (int)((float)(W.TailleX)/100 * x - surfaceMessage->w*res); 
    Message_rect.y = (int)((float)(W.TailleY)/100 * y - surfaceMessage->h*res);
    break;
  case 'w':
    Message_rect.x = (int)((float)(W.TailleX)/100 * x);
    Message_rect.y = (int)((float)(W.TailleY)/100 * y - surfaceMessage->h*res);
    break;
  case 'c':
    Message_rect.x = (int)((float)(W.TailleX)/100 * x - surfaceMessage->w*(res/2)); 
    Message_rect.y = (int)((float)(W.TailleY)/100 * y - surfaceMessage->h*(res/2));
    break;
  default:
    Message_rect.x = (int)((float)(W.TailleX)/100 * x); 
    Message_rect.y = (int)((float)(W.TailleY)/100 * y);
    break;
  }

  SDL_Surface *tmp = rotozoomSurface(surfaceMessage, 0.0, res, 0);
  SDL_BlitSurface(tmp, NULL, W.renderer, &Message_rect);
  SDL_FreeSurface(tmp);
  SDL_FreeSurface(surfaceMessage);



  
}

void freeImageMalloc(){
  free(fileImage);
  if(RobotoFont != NULL){
    TTF_CloseFont(RobotoFont);
  }
}

void *InitImage(void *CeciEstUneVariableVide){
  int nbImage = 10;
  fileImage = (SDL_Surface **)malloc(sizeof(SDL_Surface *) * nbImage);
  PixelXnb = (int *)malloc(sizeof(int) * nbImage);
  PixelYnb = (int *)malloc(sizeof(int) * nbImage);
  XImagenb = (int *)malloc(sizeof(int) * nbImage);
  YImagenb = (int *)malloc(sizeof(int) * nbImage);
  ImYoffset = (int *)malloc(sizeof(int) * nbImage);
  ImXoffset = (int *)malloc(sizeof(int) * nbImage);
  TotalImagenb = (int *)malloc(sizeof(int) * nbImage);

  // écrire (lettre chiffre) dans le render, grace à la bibliothèque TTF
  if (TTF_Init() == -1)
    {
      printf("TTF_Init: %s\n", TTF_GetError());
      exit(2);
    }
  RobotoFont = TTF_OpenFont("Res/Font/Roboto-Black.ttf", 50);


  fileImage[0] = IMG_Load("Res/Image/param.png");PixelXnb[0] = 50; PixelYnb[0] = 50;XImagenb[0] = 1; YImagenb[0] = 1; ImYoffset[0] = 0; ImXoffset[0] = 0; TotalImagenb[0] = 1; 

  

  return NULL;
}



void DrawImage(int imagenb, float x, float y, float sizeX, float sizeY, char center, int etatPremier, float TimebeforeNext, int flip, int angle, screen W){


  SDL_Rect Image_rect;
  SDL_Rect keepImage;
  int imageVoulu = 0;

  float resx;
  float resy;
  
  if(sizeY == 0 && sizeX != 0){
    resx = ((float)W.TailleX * sizeX/99.9)/PixelXnb[imagenb];
    resy = resx;
  }else if(sizeX == 0 && sizeY != 0){
    resy = ((float)W.TailleY * sizeY/100.0)/PixelYnb[imagenb];
    resx = resy;
  }else if(sizeX != 0 && sizeY != 0){
    resx = ((float)W.TailleX * (float)sizeX/100)/PixelXnb[imagenb];
    resy = ((float)W.TailleY * (float)sizeY/100)/PixelYnb[imagenb];
  }else{
    resx = 1;
    resy = 1;
  }

  
  switch (center) {
  case 'n':
    Image_rect.x = (int)((float)(W.TailleX)/100 * x); 
    Image_rect.y = (int)((float)(W.TailleY)/100 * y - ImYoffset[imagenb]*resy);
    break;
  case 'e':
    Image_rect.x = (int)((float)(W.TailleX)/100 * x - (float)(PixelXnb[imagenb]) * resx); 
    Image_rect.y = (int)((float)(W.TailleY)/100 * y);
    break;
  case 's':
    Image_rect.x = (int)((float)(W.TailleX)/100 * x - (float)(PixelXnb[imagenb]) * resx); 
    Image_rect.y = (int)((float)(W.TailleY)/100 * y - (float)(PixelYnb[imagenb]) * resy);
    break;
  case 'w':
    Image_rect.x = (int)((float)(W.TailleX)/100 * x);
    Image_rect.y = (int)((float)(W.TailleY)/100 * y - (float)(PixelYnb[imagenb]) * resy);
    break;
  case 'c':
    Image_rect.x = (int)((float)(W.TailleX)/100 * x - (float)(PixelXnb[imagenb]) * resx/2); 
    Image_rect.y = (int)((float)(W.TailleY)/100 * y - (float)(PixelYnb[imagenb]) * resy/2);
    break;
  default:
    Image_rect.x = (int)((float)(W.TailleX)/100 * x); 
    Image_rect.y = (int)((float)(W.TailleY)/100 * y);
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
  SDL_BlitSurface(tmp, &keepImage, W.renderer, &Image_rect);
  SDL_FreeSurface(tmp);
}


