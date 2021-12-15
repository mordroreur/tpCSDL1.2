#include "main.h"
#include "Render.h"


int main(){
  srand(time(NULL));
  
  int hightscore;
  int endedGamesWin;
  int endedLoseGame;
  char mouvsup[4];
  int nbReplay;
  int SreenSizeX;
  int SreenSizeY;
  int isFullscreen;
  int sound;
  int music;
  int time;
  
  int EtapeDuJeu = -1;
  int isSave = 0;
  ter plateau;
  char play = 'O';

  plateau.tailleX = -1;
  
  EtapeDuJeu = BouclePrincipaleDuJeu();
  

  FILE *param = fopen(PARAM_NAME, "r");
  if(param == NULL){
    hightscore = 0;
    endedGamesWin = 0;
    endedLoseGame = 0;
    mouvsup[0] = '-';
    mouvsup[1] = '-';
    mouvsup[2] = '-';
    mouvsup[3] = '-';
    nbReplay = 0;
    SreenSizeX = 750;
    SreenSizeY = 500;
    isFullscreen = 0;
    sound = 100;
    music = 100;
    time = 1000000;
    writesaveFile(hightscore, endedGamesWin, endedLoseGame, mouvsup, nbReplay, SreenSizeX, SreenSizeY, isFullscreen, sound, music, time);
  }else{
    fscanf(param, "%d\n%d\n%d\n%c\n%c\n%c\n%c\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n", &hightscore, &endedGamesWin, &endedLoseGame, &mouvsup[0], &mouvsup[1], &mouvsup[2], &mouvsup[3], &nbReplay, &SreenSizeX, &SreenSizeY, &isFullscreen, &sound, &music, &time);
    fclose(param);
  }
  
    


  FILE *save = fopen(SAVE_NAME, "r");
  if(save != NULL){
    isSave = 1;
    fclose(save);
  }
  
  if(EtapeDuJeu == 2){
    EtapeDuJeu = 1;
  }

  
  

  

  while(EtapeDuJeu){

    switch (EtapeDuJeu) {
    case 1 : /* Cas d'arriver dans le jeu */
      clearScreen;
      printf("Bonjours et bienvenue sur ce jeu de 2048 version console.\n");
      printf("Retenez bien que les mouvement de base sont :\n\tb : Bas\n\th : Haut\n\tg : Gauche\n\td : Droite\n\tq : Quitter\n\t- : Quitter (permet de lié q et pouvoir quitter quand même)\n\n");
      EtapeDuJeu = 2;
      break;
      
    case 2 :
      // Si une savegarde a ete apercu en cours
      if(isSave){
	printf("Voulez avez une partie en cours sauvegardée.\n\n");
      }

      printf("Que voulez vous faire ?\n\t");

      if(isSave){
	printf("s : récupérer la sauvergarde en cours.\n\tn : continuer la sauvegarde avec un replay.\n\t");
      }
      printf("g : utiliser l'interface graphique.\n\tc : commencer un 2048 \"standard\".\n\tt : lancer un 2048 avec une configuration bizarre.%s\n\tp : ouvrir les paramètre.\n\tq : si vous voulez deja partir.\n\n\t\t : ", (nbReplay > 0)?"\n\tr : lancer un replay.":"");
      scanf("%c", &play); while ((getchar()) != '\n');
      float temporary = -1;
      int tempX = -1;
      int tempY = -1;
      switch (play) {
	
      case 'g':
	EtapeDuJeu = BouclePrincipaleDuJeu();
	FILE *param = fopen(PARAM_NAME, "r");
	if(param == NULL){
	  hightscore = 0;
	  endedGamesWin = 0;
	  endedLoseGame = 0;
	  mouvsup[0] = '-';
	  mouvsup[1] = '-';
	  mouvsup[2] = '-';
	  mouvsup[3] = '-';
	  nbReplay = 0;
	  SreenSizeX = 750;
	  SreenSizeY = 500;
	  isFullscreen = 0;
	  sound = 100;
	  music = 100;
	  time = 1000000;
	  writesaveFile(hightscore, endedGamesWin, endedLoseGame, mouvsup, nbReplay, SreenSizeX, SreenSizeY, isFullscreen, sound, music, time);
	}else{
	  fscanf(param, "%d\n%d\n%d\n%c\n%c\n%c\n%c\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n", &hightscore, &endedGamesWin, &endedLoseGame, &mouvsup[0], &mouvsup[1], &mouvsup[2], &mouvsup[3], &nbReplay, &SreenSizeX, &SreenSizeY, &isFullscreen, &sound, &music, &time);
	  fclose(param);
	}
	FILE *save = fopen(SAVE_NAME, "r");
	if(save != NULL){
	  isSave = 1;
	  fclose(save);
	}
	if(EtapeDuJeu == 2){
	  EtapeDuJeu = 1;
	}
	break;
	
      case 's' :
	if(isSave){
	  plateau = ReadEnCoursSave(0, SAVE_NAME);
	  if(plateau.tailleX == -1){
	    printf("Il y a eu un problème d'allocation de la mémoire. Le jeu ne peu continuer dans ces conditions.\n");
	    EtapeDuJeu = -1;
	  }else {
	    afficheTer(plateau);
	    printf("Appuyez sur une touche pour reprendre la partie.");
	    while ((getchar()) != '\n');
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
	    }else{
	      EtapeDuJeu = 3;
	    }
	    break;
	  }
	}
	break;

      case 'n':
	if(isSave){
	  while(temporary < 0){
	    printf("Combien de temps en secondes sur chaque étape du replay voulez vous ? ");
	    scanf("%f", &temporary); while ((getchar()) != '\n');
	  }
	  plateau = ReadEnCoursSave(temporary, SAVE_NAME);
	  if(plateau.tailleX == -1){
	    printf("Il y a eu un problème d'allocation de la mémoire. Le jeu ne peu continuer dans ces conditions.\n");
	    EtapeDuJeu = -1;
	  }else {
	    afficheTer(plateau);
	    printf("Appuyez sur une touche pour reprendre la partie.");
	    while ((getchar()) != '\n');
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
	    }else{
	      EtapeDuJeu = 3;
	    }
	    break;
	  }
	}
	break;

      case 'c' : if(save){printf("La partie à été supprimé au profit de la prochaine partie.\n");}
	// on lance une partie avec un terrain de taille 4
	printf("Appuyez sur entrer pour lancer le jeu.");
	plateau = InitVide(4, 4);
	EtapeDuJeu = 5;
	break;

      case 't' :
	while (tempY < 0) {
	  printf("Veuillez entrez le nombre de ligne voulu : ");
	  scanf("%d", &tempY); while ((getchar()) != '\n');
	}
	while (tempX < 0) {
	  printf("Veuillez entrez le nombre de colonnes voulu : ");
	  scanf("%d", &tempX); while ((getchar()) != '\n');
	}
	if(save){printf("La partie à été supprimé au profit de la prochaine partie.\n");}
	// on lance une partie avec un terrain de taille defini apr le joueur
	printf("Appuyez sur entrer pour lancer le jeu.");
	printf("%d %d\n", tempY, tempX);
	plateau = InitVide(tempY, tempX);
	EtapeDuJeu = 5;
	break;


      case 'r':
	if(nbReplay > 0){
	  while (tempX <= -1 || tempX >= nbReplay) {
	    printf("Entrez le nombre du replay entre 0 et %d : ", nbReplay-1);
	    scanf("%d", &tempX); while ((getchar()) != '\n');
	  }
	  char name[100];
	  sprintf(name, "%s%d", SAVE_REPLAY, tempX);
	  while(temporary < 0){
	    printf("Combien de temps en secondes sur chaque étape du replay voulez vous ? ");
	    scanf("%f", &temporary); while ((getchar()) != '\n');
	  }
	  ReadEnCoursSave(temporary, name);
	  printf("Appuyez sur entrez pour continuer\n");
	  while ((getchar()) != '\n');
	  clearScreen;
	}
	break;

      case 'p': EtapeDuJeu = 6; break;

      case 'q' :case '-': EtapeDuJeu = -1;break;

      default: printf("Ce n'est pas une proposition!\n");break;
      }
      break;

    case 3 : /* Cas du jeu en affichage console */
      printf("Entrez le déplacement : ");
      // recuperation de l entree joueur
      scanf("%c", &play); while ((getchar()) != '\n');
      if(play == mouvsup[0] && mouvsup[0] != '-'){play = 'g';
      }else if(play == mouvsup[1] && mouvsup[1] != '-'){play = 'h';
      }else if(play == mouvsup[2] && mouvsup[2] != '-'){play = 'd';
      }else if(play == mouvsup[3] && mouvsup[3] != '-'){play = 'b';}
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
	    //on fais apparaitre un 2 a 75 pourcent de chance, un 4 sinon
	  SetRandomCase(&plateau, (rand()%100 < 90)?2:4);
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
      }else if (play == 'q' || play == '-') {
	// il va a l etape de fermeture du jeu
	EtapeDuJeu = -1;
      }else{
	printf("ce n'est pas un déplacement reconnu\n");
      }
      break;

    case 4:
      afficheTer(plateau);
      if(plateau.max < 2048){
	printf("Vous avez perdu! ");
	endedLoseGame++;
      }else{
	printf("Vous avez gagné! ");
	endedGamesWin++;
      }
      printf("Votre meilleur score est de %d ", plateau.score);
      if(plateau.tailleX == plateau.tailleY && plateau.tailleX == 4 && plateau.score < hightscore){
	printf("et donc inferieur au meilleur score de %d.\n", hightscore);
      }else if(plateau.tailleX == plateau.tailleY && plateau.tailleX == 4){
	printf("et donc suppérieur au meilleur score de %d.\n", hightscore);
	hightscore = plateau.score;
      }
      printf("Voulez-vous sauvegarder le replay?(o = oui, n = non) : ");
      scanf("%c", &play); while ((getchar()) != '\n');
      if(play == 'o'){
	char tmp[100];
	sprintf(tmp, "%s%d", SAVE_REPLAY, nbReplay);
	copiFile(SAVE_NAME, tmp);
	if(remove(SAVE_NAME) != 0){
	  printf("Il y a un probleme inattendu...\n");
	}
	nbReplay++;
      }else{
	if(remove(SAVE_NAME) != 0){
	  printf("Il y a un probleme inattendu...\n");
	}
      }
      writesaveFile(hightscore, endedGamesWin, endedLoseGame, mouvsup, nbReplay, SreenSizeX, SreenSizeY, isFullscreen, sound, music, time);
      printf("Voulez-vous recommencer une partie?(o = oui, n = non) : ");
      scanf("%c", &play); while ((getchar()) != '\n');
      if(play == 'o'){
	LibereTer(&plateau);
	EtapeDuJeu = 5;
      }else{
	EtapeDuJeu = -1;
      }
      break;

    case 5:
	// cas ou l initialisation du terrain c est mal passe on quitte le jeu
	if(plateau.tailleX == -1){
	  printf("Il y a eu un problème d'allocation de la mémoire. Le jeu ne peu continuer dans ces conditions.\n");
	  EtapeDuJeu = -1;
	}else {
	  while ((getchar()) != '\n');
	  afficheTer(plateau);
	  EtapeDuJeu = 3;
	}
	break;


      
    case 6:
      printf("Bienvenue dans les parametre. Vous pouvez :\n\t Ajouter une touche pour l'une des touche(g, b, d, h).\n Voulez vous faire ca ou partir (o = faire ca) ? ");
      scanf("%c", &play); while ((getchar()) != '\n');
      if(play == 'o'){
	printf("A quelle touche voulez vous mettre une correspondance ? ");
	scanf("%c", &play); while ((getchar()) != '\n');
	
	if(play == 'd'){
	  printf("Quelle correspondance voulez vous mettre ? ");
	  scanf("%c", &play); while ((getchar()) != '\n');
	  if(play == ' ' || play == '-'){
	    printf("Cette touche fais malheureusement partie d'un des rare que l'on ne peu associer...\n");
	  }else{
	    mouvsup[2] = play;
	  }
	}else if(play == 'b'){
	  printf("Quelle correspondance voulez vous mettre ? ");
	  scanf("%c", &play); while ((getchar()) != '\n');
	  if(play == ' ' || play == '-'){
	    printf("Cette touche fais malheureusement partie d'un des rare que l'on ne peu associer...\n");
	  }else{
	    mouvsup[3] = play;
	  }
	}else if(play == 'g'){
	  printf("Quelle correspondance voulez vous mettre ? ");
	  scanf("%c", &play); while ((getchar()) != '\n');
	  if(play == ' ' || play == '-'){
	    printf("Cette touche fais malheureusement partie d'un des rare que l'on ne peu associer...\n");
	  }else{
	    mouvsup[0] = play;
	  }
	}else if(play == 'h'){
	  printf("Quelle correspondance voulez vous mettre ? ");
	  scanf("%c", &play); while ((getchar()) != '\n');
	  if(play == ' ' || play == '-'){
	    printf("Cette touche fais malheureusement partie d'un des rare que l'on ne peu associer...\n");
	  }else{
	    mouvsup[1] = play;
	  }
	}else{
	  printf("Cette touche n'existe malheureusement pas dans le jeu...\n");
	}
	writesaveFile(hightscore, endedGamesWin, endedLoseGame, mouvsup, nbReplay, SreenSizeX, SreenSizeY, isFullscreen, sound, music, time);
      }else{
	clearScreen;
	EtapeDuJeu = 2;
      }
      break;

      

      
    case -1: // Cas de fermeture du jeu : liberation de la memoire et on sort de la boucle
      printf("Merci d'avoir joué, nous espérons vous revoir bientôt!!!\n");
      LibereTer(&plateau);
      while ((getchar()) != '\n');
      clearScreen;
      EtapeDuJeu = 0;
      break;


      
      
      
    default :

      
      


      printf("Pas implementé!!!\n"); EtapeDuJeu = 0;break;
    }

    

    
  }


  return 0;
}
