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

int InputVal[2][6]; /* Valeur en int des inputs pour les action joueurs */

int GetInput[2][6]; /* Valeur de si le joueur appuie */

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
    fprintf(Write, "1\nn\n960\n540\n122\n100\n115\n113\n101\n102\n273\n275\n274\n276\n104\n105");
    fflush(Write);
    fclose(Write);
    InputVal[0][0] = 122;
    InputVal[0][1] = 100;
    InputVal[0][2] = 115;
    InputVal[0][3] = 113;
    InputVal[0][4] = 101;
    InputVal[0][5] = 102;
    InputVal[1][0] = 273;
    InputVal[1][1] = 275;
    InputVal[1][2] = 274;
    InputVal[1][3] = 276;
    InputVal[1][4] = 104;
    InputVal[1][5] = 105;
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
	case 4:sscanf(s, "%d", &InputVal[0][0]);break;
	case 5:sscanf(s, "%d", &InputVal[0][1]);break;
	case 6:sscanf(s, "%d", &InputVal[0][2]);break;
	case 7:sscanf(s, "%d", &InputVal[0][3]);break;
	case 8:sscanf(s, "%d", &InputVal[0][4]);break;
	case 9:sscanf(s, "%d", &InputVal[0][5]);break;
	case 10:sscanf(s, "%d", &InputVal[1][0]);break;
	case 11:sscanf(s, "%d", &InputVal[1][1]);break;
	case 12:sscanf(s, "%d", &InputVal[1][2]);break;
	case 13:sscanf(s, "%d", &InputVal[1][3]);break;
	case 14:sscanf(s, "%d", &InputVal[1][4]);break;
	case 15:sscanf(s, "%d", &InputVal[1][5]);break;
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
