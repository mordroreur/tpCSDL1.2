#ifndef DRAW_HEADER_
#define DRAW_HEADER_

#include "SDL.h"
#include "Jeu.h"
#include "RenderUtilities.h"
#include <SDL/SDL_ttf.h>
#include <time.h>
#include <math.h>

void LoadingScreen();

void DrawMenu();

void DrawExit();

void DrawCreaLVL();

void Draw1player();

int getimage(cellule l);

int getimageEnti(entite e);

	       
#endif /* DRAW_HEADER_ */
