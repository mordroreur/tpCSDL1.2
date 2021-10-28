#ifndef RENDERUTILLITIES_HEADER_
#define RENDERUTILLITIES_HEADER_


#include "SDL.h"
#include <SDL/SDL_ttf.h>
#include <time.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>

void DrawString(char *s, int x, int y, int size, char center, int R, int G, int B);


long int getTime();

void InitImage();

void freeImageMalloc();

void DrawImage(int imagenb, int x, int y, int size, char center, int nbImage, float TimebeforeNext, int flip);

#endif /* RENDERUTILLITIES_HEADER_ */
