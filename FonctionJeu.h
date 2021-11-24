#ifndef _FONCTIONJEU_HEADER
#define _FONCTIONJEU_HEADER


#include <stdio.h>


typedef struct Terrain{
  int **tab;
  int max;
  int taille;
  int vide;
  int score;
}ter;



ter InitVide(int n);





#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) /* cas ou le compilateur est windows */

#define clearScreen system("cls")

#elif __linux__ /* Linux */

#define clearScreen system("clear")

#else /* Ni linux ni windows */
#   error "Unknown compiler"
#endif





#endif /* _FONCTIONJEU_HEADER */
