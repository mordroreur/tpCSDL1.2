#ifndef _FONCTIONJEU_HEADER
#define _FONCTIONJEU_HEADER


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Terrain{
  int **tab;
  int max;
  int tailleX;
  int tailleY;
  int vide;
  int score;
}ter;



ter InitVide(int n, int y);
void afficheTer(ter T);
int SetRandomCase(ter *T, int n);
void CaseMouve(ter *T, int depNum);
int CanDep(ter T, int depNum);
void LibereTer(ter *T);
ter ReadEnCoursSave(float time, char *name);
void copiFile(char *old, char *nouv);

void writesaveFile(int hightscore, int endedGamesWin, int endedLoseGame, char mouvsup[4], int nbReplay);


#define MAX(a, b) (a<b)?b:a

#define SAVE_NAME "Res/Partie_encours"
#define PARAM_NAME "Res/Parametres"
#define SAVE_REPLAY "Res/Replay"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) /* cas ou le compilateur est windows */

#include <Windows.h>
#define clearScreen system("cls")

#elif __linux__ /* Linux */

#include <unistd.h>
#define clearScreen system("clear")

#else /* Ni linux ni windows */
#   error "Unknown compiler"
#endif





#endif /* _FONCTIONJEU_HEADER */
