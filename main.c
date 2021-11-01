#include "main.h"

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////       Declaration des variables globale     ///////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////


int EtapeActuelleDuJeu; /* 0 = fin; 1 = Loading Screen... */

int TailleEcranLong = 960; /* Taille de l'ecran en nombre de pixel de gauche a droite */
int TailleEcranHaut= 540; /* Taille de l'ecran en nombre de pixel de haut en bas */

SDL_Surface * renderer = NULL; /* Ecran sur lequel on dessine */

SDL_Event event; /* Les events que l'on récupèrera via SDL. */

int DEBUG = 1; /* 1 : affiche le debug*/

TTF_Font *RobotoFont = NULL; /* Font utilise pour ecrire dans le programme */

int unlockLVL = 1; /* Dernier niveau debloque */

int fullscreen = 0; /* Si le niveau est en plein ecran au demarage */

level Actulvl; /* Niveau actuelle sur lequel on joue */

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

int main()
{
  //  srand(time(NULL)); // in case we need random
  FILE *Save;
  

  Save = fopen("Res/Sauvegarde", "r");
  if(Save == NULL){
    FILE *Write;
    Write = fopen("Res/Sauvegarde", "w+");
    fprintf(Write, "1\nn\n960\n540\n");
    fflush(Write);
    fclose(Write);
  }else{
    char s[100];
    char r;
    int valeur = 0;
    int nb = 0;
    while ((r = getc(Save)) != EOF){
      if(r == '\n'){
	s[valeur] = '\0';
	valeur = 0;
	switch(nb){
	case 0:sscanf(s, "%d", &unlockLVL);break;
	case 1:if(s[0] == 'f'){fullscreen = 1;}break;
	case 2:sscanf(s, "%d", &TailleEcranLong);break;
	case 3:sscanf(s, "%d", &TailleEcranHaut);break;
	}
	nb++;
      }else{
	s[valeur++] = r;
      }
    }
    fclose(Save);
  }
  
  EtapeActuelleDuJeu = 1;

  BouclePrincipaleDuJeu();

  
  
}
