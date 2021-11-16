#ifndef RENDERUTILLITIES_HEADER_
#define RENDERUTILLITIES_HEADER_


#include "SDL.h"
#include <SDL/SDL_ttf.h>
#include <time.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>

void DrawString(char *s, int x, int y, int size, char center, int R, int G, int B);


long int getTime();

void *InitImage(void *CeciEstUneVariableVide);

void freeImageMalloc();

void DrawImage(int imagenb, float x, float y, float sizeX, float sizeY, char center, int etatPremier, float TimebeforeNext, int flip, int angle);

void PrintSave();
#endif /* RENDERUTILLITIES_HEADER_ */
