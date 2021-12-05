
#ifndef _RENDERUTILITIES_HEADER
#define _RENDERUTILITIES_HEADER

#include "FonctionJeu.h"

#include "SDL.h"
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_rotozoom.h>
#include <time.h>


typedef struct Screen{
  SDL_Surface *renderer;
  int TailleY;
  int TailleX;
  int otherX;
  int otherY;
  int fullscreen;
  int EtapeActuelleDujeu;
  int hightscore;
  int endedGamesWin;
  int endedLoseGame;
  char mouvsup[4];
  int nbReplay;
  int sound;
  int music;
  ter plateau;
  int returnValue;
  int depEnCours;
  int **oldTer;
  int **DepCase;
  int time;
  long int timeBegin;
  int isSave;
} screen;


long int getTime();
void DrawString(char *s, int x, int y, int size, char center, int R, int G,
                int B, screen W);
void freeImageMalloc();
void *InitImage(void *CeciEstUneVariableVide);
void DrawImage(int imagenb, float x, float y, float sizeX, float sizeY, char center, int etatPremier, float TimebeforeNext, int flip, int angle, screen W);


#endif /* _RENDERUTILITIES_HEADER */
