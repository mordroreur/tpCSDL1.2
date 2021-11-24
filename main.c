#include "main.h"
#include <stdio.h>

int main(){
  srand(time(NULL));
  
  int EtapeDuJeu = -1;
  int isSave = 0;
  ter plateau;



  EtapeDuJeu = 2;

  

  while(EtapeDuJeu){

    switch (EtapeDuJeu) {
    case 2 : /* Cas d'arriver dans le jeu */
      clearScreen;
      printf("Bonjours et bienvenue sur ce jeu de 2048 version console.\n");
      if(isSave){
	printf("Vous n'êtes pas sensé etres la...\n");
	EtapeDuJeu = 0;
      }else{
	printf("Appuyez sur entrer pour lancer le jeu.");
	plateau = InitVide(4);
	getchar();
	clearScreen;
	EtapeDuJeu = 0;
      }
      break;

    case 3 : /* Cas du jeu en affichage console */
      

      
    default : break;
    }

    

    
  }


  return 0;
}
