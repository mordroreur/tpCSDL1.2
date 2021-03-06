#ifndef _RENDER_HEADER
#define _RENDER_HEADER


#include "RenderUtilities.h"
#include "FonctionJeu.h"

#include "SDL.h"
#include "SDL_stdinc.h"
#include <SDL/SDL_mixer.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define FPS_TO_GET 60;
#define TICK_TO_GET 60;


int BouclePrincipaleDuJeu();
screen *create_Win(int x, int y, int fullscreen);
void end_sdl(char ok, char const * msg, screen *s);
void keyUp(SDL_KeyboardEvent *key, screen *win);
void *BouclePrincipaleDesTicks(void *arg);

void DrawSelectReplay(screen *win);
void DrawPlateau(screen *win);
void DrawGameOver(screen *win);
void DrawSavedOver(screen *win);
void DrawMenu(screen *win);
void recupeDep(screen *win);
Uint32 getColor(int value, screen *win);
void DrawParam(screen *win);
void DrawGetTouch(screen *win);






#endif /* _RENDER_HEADER */
