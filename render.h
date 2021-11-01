#ifndef RENDER_HEADER_
#define RENDER_HEADER_

#include "SDL.h"
#include "RenderUtilities.h"
#include "Jeu.h"
#include "Draw.h"
#include <SDL/SDL_ttf.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define FPS_TO_GET 60  /* Nombre d'actualisation de l'écran recherché */
#define TICK_TO_GET 60 /* Nombre de verification du gameplay*/


int BouclePrincipaleDuJeu();


void create_Win();


void end_sdl(char ok, char const *msg);


void keyUp(SDL_KeyboardEvent *key);


void *BouclePrincipaleDesTicks(void *CeciEstUneVaribleNull);



#endif /* RENDER_HEADER_ */
