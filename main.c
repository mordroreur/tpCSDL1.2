#include "main.h"
#include "FonctionJeu.h"
#include <stdio.h>

int main(){
  srand(time(NULL));
  
  int EtapeDuJeu = -1;
  int isSave = 0;
  ter plateau;
  char play = 'O';

  FILE *save = fopen(SAVE_NAME, "r");
  if(save != NULL){
    isSave = 1;
    fclose(save);
  }
  
  EtapeDuJeu = 2;

  
  

  while(EtapeDuJeu){

    switch (EtapeDuJeu) {
    case 2 : /* Cas d'arriver dans le jeu */
      clearScreen;
      printf("Bonjours et bienvenue sur ce jeu de 2048 version console.\n");
      // Si une savegarde a ete apercu en cours
      if(isSave){
	printf("Voulez vous chargé la partie sauvegardé ?\nTapez o pour oui ou n pour non : ");
	scanf("%c", &play); while ((getchar()) != '\n');
	if(play == 'o'){
	  
	}else{
	  printf("La partie à été supprimé au profit de la prochaine partie.\n");
	  //
	  // Lance la selection de jeu ici
	  //
	  // on lance une partie avec un terrain de taille 4
	  printf("Appuyez sur entrer pour lancer le jeu.");
	  plateau = InitVide(4);
	  // cas ou l initialisation du terrain c est mal passe on quitte le jeu
	  if(plateau.taille == -1){
	    printf("Il y a eu un problème d'allocation de la mémoire. Le jeu ne peu continuer dans ces conditions.\n");
	    EtapeDuJeu = -1;
	  }else {
	    getchar();
	    afficheTer(plateau);
	    EtapeDuJeu = 3;
	  }
	  //
	  //
	  //
	}
      // si pas de partie en cours a charge
      }else{
	//
	// Lance la selection de jeu ici
	//
	// on lance une partie avec un terrain de taille 4
	printf("Appuyez sur entrer pour lancer le jeu.");
	plateau = InitVide(4);
	// cas ou l initialisation du terrain c est mal passe on quitte le jeu
	if(plateau.taille == -1){
	  printf("Il y a eu un problème d'allocation de la mémoire. Le jeu ne peu continuer dans ces conditions.\n");
	  EtapeDuJeu = -1;
	}else {
	  getchar();
	  afficheTer(plateau);
	  EtapeDuJeu = 3;
	}
	//
	//
	//
      }
      break;

    case 3 : /* Cas du jeu en affichage console */
      printf("Entrez le déplacement(d = droite, h = haut, g = gauche, b = bas) \n\tou q pour quitter : ");
      // recuperation de l entree joueur
      scanf("%c", &play); while ((getchar()) != '\n');
      // si c est une entree de deplacement
      if(play == 'd' || play == 'h' || play == 'g' || play == 'b'){
	int depNum = (play == 'b')?1:(play == 'g')?2:(play == 'h')?3:4;
	// si le joueur peu se deplacer dans la direction qu il a choisi
	if(CanDep(plateau, depNum)){
	  // on ajoute le deplacement a la sauvegarde
	  FILE *save = fopen(SAVE_NAME, "a");
	  fprintf(save, "%c ", play);
	  fflush(save);
	  fclose(save);
	  // on deplace
	  CaseMouve(&plateau, depNum);
	  // on affiche le nouveau jeu
	  afficheTer(plateau);
	  // on cosidere que le joueur ne peu pas bouger
	  int GameOver = 1;
	  //pour les quatre deplacement
	  for(int i = 1;i < 5; i++){
	    if(CanDep(plateau, i)){
	      // si le joueur peu bouger on change cette variable
	      GameOver = 0;
	      break;
	    }
	  }
	  // si GameOver est toujours a 1 alors le joueur a perdu
	  if(GameOver){
	    // il va donc a l ecran 4
	    EtapeDuJeu = 4;
	  }
	}else{
	  printf("Ce déplacement n'est pas valide!!!");
	}
      // si le joueur selectionne de sortir du jeu
      }else if (play == 'q') {
	// il va a l etape de fermeture du jeu
	EtapeDuJeu = -1;
      }else{
	printf("ce n'est pas un déplacement reconnu\n");
      }
      break;

    case -1: // Cas de fermeture du jeu : liberation de la memoire et on sort de la boucle
      printf("Merci d'avoir joué, nous espérons vous revoir bientôt!!!\n");
      LibereTer(&plateau);
      getchar();
      clearScreen;
      EtapeDuJeu = 0;
      break;

      
    default : printf("Pas implementé!!!\n"); EtapeDuJeu = 0;break;
    }

    

    
  }


  return 0;
}
