#include "Render.h"
#include "FonctionJeu.h"
#include "RenderUtilities.h"
#include "SDL_video.h"

static int tickCount = 0;
static int resizingTime = 0;


int BouclePrincipaleDuJeu(){

  long int LastFrame;
  long int TimeCount;
  long int NowTime;
  long int timeForNewFrame = 1000000 / FPS_TO_GET;

  //Debogage
  int fpsCount = 0;
  int LastFpsCount = 0;
  int LastTickCount = 0;


  int DEBUG = 1;

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


   

  
  
  /*************Initailisation de la fenetre***********/
  screen *win = create_Win(SreenSizeX, SreenSizeY, isFullscreen);

  win->hightscore = hightscore;
  win->endedGamesWin = endedGamesWin;
  win->endedLoseGame = endedLoseGame;
  win->mouvsup[0] = mouvsup[0];
  win->mouvsup[1] = mouvsup[1];
  win->mouvsup[2] = mouvsup[2];
  win->mouvsup[3] = mouvsup[3];
  win->nbReplay = nbReplay;
  win->sound = sound;
  win->music = music;
  win->returnValue = 0;
  win->time = time;

  FILE *save = fopen(SAVE_NAME, "r");
  if(save != NULL){
    win->isSave = 1;
    fclose(save);
  }else{
    win->isSave = 0;
  }
  
  /************Initialisation des variables de temps**************/
  LastFrame = getTime();
  TimeCount = getTime();
  NowTime = getTime();


  /************Début de la boucle des ticks***********************/
  pthread_t threadBoucleDesTicks;
  int RetourDuThreadDesTicks = pthread_create(&threadBoucleDesTicks, NULL, BouclePrincipaleDesTicks,  win);
  if(RetourDuThreadDesTicks){
    return 1;
  }


  /********************* Charegement des images en arrière plan********/
  pthread_t threadLoadImages;
  int RetourDuThreadDesImages = pthread_create(&threadLoadImages, NULL, InitImage,  NULL);
  if(RetourDuThreadDesImages){
    return 1;
  }



  
  
  /************Début de la boucle frames**************************/
  while (win->EtapeActuelleDujeu) {
    NowTime = getTime();

    /* Gestion de l'affichage écran */
    if (NowTime - LastFrame > timeForNewFrame) {
      if(resizingTime == 0){

	
	switch (win->EtapeActuelleDujeu) {
	case 1: DrawMenu(win);break;
	case 2: DrawPlateau(win); break;
	case 4: DrawPlateau(win); DrawGameOver(win); break;
	case 6: DrawPlateau(win); DrawSavedOver(win); break;
	case 8: DrawPlateau(win); break;
	case 10: DrawPlateau(win); break;
	case 12: DrawPlateau(win); break;
	case 16: DrawSelectReplay(win); break;
	default: break;//EtapeActuelleDujeu = 0;break;
	}


	
	
	if(DEBUG){
	  char affichageFrameDebug[5];
	  sprintf(affichageFrameDebug, "%d", LastFpsCount);
	  DrawString(affichageFrameDebug, 0, 0, 6, 'n', 0, 0, 0, *win);
	  sprintf(affichageFrameDebug, "%d", LastTickCount);
	  DrawString(affichageFrameDebug, 100, 0, 6, 'e', 0, 0, 0, *win);
	}
      
	SDL_Flip(win->renderer);

      
      }
      LastFrame += timeForNewFrame;
      fpsCount++;
    }else{

      /* Endors le cpu pour garder de la ressource */
      NowTime = getTime();
      
      long SleepForCPU = 0;
      SleepForCPU = (long)(timeForNewFrame - (NowTime - LastFrame)) / 300;
      SDL_Delay(SleepForCPU);
    }
    

    /* Gestion du debugage */
    if (NowTime > TimeCount)
      {
	TimeCount += 1000000;
	LastFpsCount = fpsCount;
	LastTickCount = tickCount;
	//printf("%d images cette seconde et %d ticks\n", fpsCount, tickCount);
	fpsCount = 0;
	tickCount = 0;
      }
  }

  
  /* on referme proprement */
  end_sdl(1, "Normal ending", win);

  return win->returnValue;
}




void end_sdl(char ok, char const * msg, screen *s) {
  char msg_formated[255];

  
  freeImageMalloc();
  
  if (!ok){
    strncpy(msg_formated, msg, 250);
    //    printf("%s : %s\n", msg_formated, SDL_GetError());
    fprintf(stderr, "%s : %s\n", msg_formated, SDL_GetError());
  }

  
  if(s->renderer != NULL){
    SDL_FreeSurface(s->renderer);
  }

  SDL_Quit();

  if (!ok) {exit(EXIT_FAILURE);}
}





screen *create_Win(int x, int y, int fullscreen) {
  screen *s = (screen *)malloc(sizeof(screen));
  s->fullscreen = fullscreen;
  /* Pour mettre un icon au jeu : */
  //SDL_WM_SetIcon(IMG_Load("caisse.jpg"), NULL); // Si utilisé pas oublié #include sdl image
  s->EtapeActuelleDujeu = 1;
  
  /* Initialisation de la SDL  + gestion de l'échec possible */
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
    end_sdl(0, "ERROR SDL INIT", s);

  /* Recuperation de la 'meilleur' taille de fenetre-> */
  const SDL_VideoInfo* info = SDL_GetVideoInfo();
  s->otherX = info->current_w;
  s->otherY = info->current_h;
  if(s->otherX > s->otherY*2){
    s->otherX = s->otherX/2;
  }
  

  
  //  printf("Taille de l'écran\n\tw : %d\n\th : %d\n", TailleEcranLong,
  //	 TailleEcranHaut);

  /* Creation de la fenetre de jeu. */
  if(fullscreen == 0){
    if((s->renderer = SDL_SetVideoMode(x,y,32,
				    SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF ))==NULL){
      end_sdl(0, "ERROR SDL VIDEOMODE INIT RESIZABLE", s);
    }

  }else if(fullscreen == 1){
    // Usefull for getmouseState working
    if((s->renderer = SDL_SetVideoMode(x,y,32,
				    SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF ))==NULL){
      end_sdl(0, "ERROR SDL VIDEOMODE INIT RESIZABLE", s);
    }
    //SDL_Delay(500);
    SDL_FreeSurface(s->renderer);
    
    if((s->renderer = SDL_SetVideoMode(s->otherX,s->otherY,32,
				    SDL_HWSURFACE | SDL_FULLSCREEN | SDL_DOUBLEBUF ))==NULL){
      end_sdl(0, "ERROR SDL VIDEOMODE",s);
    }
    //printf("%d %d\n", s->otherX, s->otherY);
    s->otherX = x;
    s->otherY = y;
  }
  /* Mise d'un titre a la fenetre. */
  SDL_WM_SetCaption("THIS IS A F****** WINDOWS",NULL) ;





  

  

  
  /* Recuperation de la taille de la fenetre. */
  info = SDL_GetVideoInfo();
  s->TailleX = info->current_w;
  s->TailleY = info->current_h;

  
  
  return s;

}


void keyUp(SDL_KeyboardEvent *key, screen *win){
  //printf("%d\n", key->keysym.sym);
  switch(key->keysym.sym){
  case SDLK_ESCAPE:win->EtapeActuelleDujeu = 0;break;
  case SDLK_F11 :
    resizingTime = SDL_GetTicks();
    int x = win->otherX;
    int y = win->otherY;
    win->otherX = win->TailleX;
    win->otherY = win->TailleY;
    win->fullscreen = !win->fullscreen;
    if(win->renderer->flags == 16){
      SDL_FreeSurface(win->renderer);
      win->renderer = SDL_SetVideoMode(x,y,32, SDL_HWSURFACE | SDL_FULLSCREEN | SDL_DOUBLEBUF);
      writesaveFile(win->hightscore, win->endedGamesWin, win->endedLoseGame, win->mouvsup, win->nbReplay, win->TailleX, win->TailleY, win->fullscreen, win->sound, win->music, win->time);
      
    }else{
      SDL_FreeSurface(win->renderer);
      win->renderer = SDL_SetVideoMode(x,y,32, SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF);
      writesaveFile(win->hightscore, win->endedGamesWin, win->endedLoseGame, win->mouvsup, win->nbReplay, x, y, win->fullscreen, win->sound, win->music, win->time);
    }
    win->TailleX = x;
    win->TailleY = y;

  default:
    
    if((key->keysym.sym == win->mouvsup[0] && win->mouvsup[0] != '-') || key->keysym.sym == SDLK_LEFT){key->keysym.sym = 'g';
    }else if((key->keysym.sym == win->mouvsup[1] && win->mouvsup[1] != '-') || key->keysym.sym == SDLK_UP){key->keysym.sym = 'h';
    }else if((key->keysym.sym == win->mouvsup[2] && win->mouvsup[2] != '-') || key->keysym.sym == SDLK_RIGHT){key->keysym.sym = 'd';
    }else if((key->keysym.sym == win->mouvsup[3] && win->mouvsup[3] != '-') || key->keysym.sym == SDLK_DOWN){key->keysym.sym = 'b';}
      // si c est une entree de deplacement
    if(win->EtapeActuelleDujeu == 2 && (key->keysym.sym == 'd' || key->keysym.sym == 'h' || key->keysym.sym == 'g' || key->keysym.sym == 'b')){
	int depNum = (key->keysym.sym == 'b')?1:(key->keysym.sym == 'g')?2:(key->keysym.sym == 'h')?3:4;
	// si le joueur peu se deplacer dans la direction qu il a choisi
	if(CanDep(win->plateau, depNum)){
	  // on ajoute le deplacement a la sauvegarde
	  FILE *save = fopen(SAVE_NAME, "a");
	  fprintf(save, "%c ", key->keysym.sym);
	  fflush(save);
	  fclose(save);
	  win->depEnCours = depNum;

	  // On deplace et on recupere les info de deplacement
	  recupeDep(win);
	  win->timeBegin = getTime();
	  
	  
	    //on fais apparaitre un 2 a 75 pourcent de chance, un 4 sinon
	  SetRandomCase(&win->plateau, (rand()%100 < 90)?2:4);
	  // on cosidere que le joueur ne peu pas bouger
	  int GameOver = 1;
	  //pour les quatre deplacement
	  for(int i = 1;i < 5; i++){
	    if(CanDep(win->plateau, i)){
	      // si le joueur peu bouger on change cette variable
	      GameOver = 0;
	      break;
	    }
	  }
	  // si GameOver est toujours a 1 alors le joueur a perdu
	  if(GameOver){
	    // il va donc a l ecran 4
	    win->EtapeActuelleDujeu = 4;
	    if(win->plateau.max < 2048){
	      win->endedLoseGame++;
	    }else{
	      win->endedGamesWin++;
	    }
	  }
	}
      }
    
    break;
  }
}




void *BouclePrincipaleDesTicks(void *arg){

  screen *win = arg;

  FILE *save;
  
  long int LastTick;
  long int timeForNewTick = 1000000 / TICK_TO_GET;
  long int NowTime;

  int mousX;
  int mousY;

  SDL_Event event;
  
  
  NowTime = getTime();
  LastTick = getTime();

  
  while(win->EtapeActuelleDujeu){

    NowTime = getTime();
    
    /* Gestion des verif gameplay */
    if (NowTime - LastTick > timeForNewTick) {
      

      if(resizingTime != 0){
	if(resizingTime+500 < SDL_GetTicks()){
	  if(win->otherY > win->TailleY){
	    writesaveFile(win->hightscore, win->endedGamesWin, win->endedLoseGame, win->mouvsup, win->nbReplay, win->TailleX, win->TailleY, win->fullscreen, win->sound, win->music, win->time);
	    SDL_FreeSurface(win->renderer);
	    if((win->renderer = SDL_SetVideoMode(win->TailleX,win->TailleY,32,
					    SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF )) == NULL){
	      end_sdl(0, "ERROR SDL VIDEOMODE", win);
	    }
	  }else{
	    writesaveFile(win->hightscore, win->endedGamesWin, win->endedLoseGame, win->mouvsup, win->nbReplay, win->otherX, win->otherY, win->fullscreen, win->sound, win->music, win->time);
	    SDL_FreeSurface(win->renderer);
	  }
	  resizingTime = 0;
	}
      }
      //printf("%d %d\n", renderer->w, renderer->h);

      if((win->EtapeActuelleDujeu == 8 || win->EtapeActuelleDujeu == 10) && win->timeBegin < getTime()-win->time){
	int nb = 0;
	char r;
	char str[100];
	char append[10];
	char play;
	int Value;
	int nbVide;
	strcpy(str, "");
	strcpy(append, "");
	while(nb < 3){
	  if((r = getc(save)) != EOF){
	    strcpy(append, "");
	    sprintf(append, "%c", r);
	    strcat(str, append);
	    if(r == ' '){
	      nb++;
	    }
	  }else{
	    nb = 15;
	  }
	}
	if(nb != 15){
	  
	  sscanf(str, "%c %d %d ", &play, &Value, &nbVide);
	  win->depEnCours = (play == 'b')?1:(play == 'g')?2:(play == 'h')?3:(play == 'n')?0:4;
	  if(win->depEnCours != 0){
	    // on effectue le deplacement deja effectue par le joueur
	    recupeDep(win);
	    win->timeBegin = getTime();
	  }

	  int mis = 0;
	  int videVue = -1;
	  //pour toute les cases
	  for(int i = 0; i < win->plateau.tailleY; i++){
	    for(int j = 0; j < win->plateau.tailleY; j++){
	      // si la case est vide
	      if(win->plateau.tab[i][j] == -1){
		// Une case de plus ou on peu mettre
		videVue++;
		// si le numero de case coincide avec le numero de cette case on met a cette case
		if(videVue == nbVide){
		  win->plateau.tab[i][j] = Value;
		  mis = 1;
		  break;
		}
	      }
	    }
	    // si on a deja mis une case, on sort de la boucle
	    if(mis){
	      break;
	    }
	  }
	  // si une case a etait mis alors rien d anormale
	  if(mis){
	    win->plateau.vide--;
	    // sinon il doit y avoir une erreur
	  }else{
	    printf("Erreur lors de la mise d'un block de valeur %d en %d alors qu'il y a %d vide.\n", Value, nbVide, win->plateau.vide);
	    // on remet la taille a -1 pour recuperer le cas d erreur
	    win->plateau.tailleX = -1;
	    win->plateau.tailleY = -1;
	    win->EtapeActuelleDujeu = 2;
	  }
	
	}else if(nb == 15){
	  int GameOver = 1;
	  //pour les quatre deplacement
	  for(int i = 1;i < 5; i++){
	    if(CanDep(win->plateau, i)){
	      // si le joueur peu bouger on change cette variable
	      GameOver = 0;
	      break;
	    }
	  }
	  // si GameOver est toujours a 1 alors le joueur a perdu
	  if(GameOver){
	    // il va donc a l ecran 4
	    if(win->EtapeActuelleDujeu == 8){
	      win->EtapeActuelleDujeu = 4;
	    }else{
	      win->EtapeActuelleDujeu = 6;
	    }
	    
	  }else{
	    win->EtapeActuelleDujeu = 2;
	  }
	}

      }


      
      
      /* Gestion des imputs clavier */
      while (SDL_PollEvent(&event))
	{
	  switch (event.type)
	    {
	    case SDL_KEYDOWN:
	      //	      keyDown(&event.key, EtapeActuelleDuJeu);break;
	      break; 
	    case SDL_KEYUP:
	      keyUp(&event.key, win);
	      break;
	      /*      case SDL_MOUSEWHEEL:
	      // if (event.wheel.y > 0) {
	      // } else if (event.wheel.y < 0) {
	      // }
	      break;*/
	      case SDL_MOUSEBUTTONDOWN:
	      SDL_GetMouseState(&mousX, &mousY);
	      if (event.button.button == SDL_BUTTON_LEFT) {
		if(win->EtapeActuelleDujeu == 1){
		  if(mousX > win->TailleX/4 && mousX < win->TailleX/4 * 2 && mousY > win->TailleY/4 && mousY < win->TailleY/4 * 2 ){
		    int tX = 4;
		    int tY = 4;
		    // initialisation du plateau
		    win->plateau = InitVide(tX, tY);
		    if(win->plateau.tailleX == -1){
		      win->EtapeActuelleDujeu = 0;
		    }
		    win->depEnCours = 0;
		    win->timeBegin = getTime() - win->time;

		    win->oldTer = (int ** )malloc(sizeof(int *) * tY);
		    if(win->oldTer == NULL){
		      win->EtapeActuelleDujeu = 0;
		    }else{
		      for(int i = 0; i < tY; i++){
			win->oldTer[i] = (int *)malloc(sizeof(int) * tX);
			if(win->oldTer[i] == NULL){
			  for(int j = 0; j < i; i++){
			    free(win->oldTer[j]);
			  }
			  free(win->oldTer);
			  win->EtapeActuelleDujeu = 0;
			}
		      }
		    }
		    // On met que toute les cases sont vides
		    if(win->oldTer != NULL){
		      for(int i = 0; i < tY; i++){
			for(int j = 0; j < tX; j++){
			  win->oldTer[i][j] = -1;
			}
		      }
		    }
		    win->DepCase = (int ** )malloc(sizeof(int *) * tY);
		    if(win->DepCase == NULL){
		      win->EtapeActuelleDujeu = 0;
		    }else{
		      for(int i = 0; i < tY; i++){
			win->DepCase[i] = (int *)malloc(sizeof(int) * tX);
			if(win->DepCase[i] == NULL){
			  for(int j = 0; j < i; i++){
			    free(win->DepCase[j]);
			  }
			  free(win->DepCase);
			  win->EtapeActuelleDujeu = 0;
			}
		      }
		    }
		    if(win->DepCase != NULL){
		      // On met que toute les cases sont vides
		      for(int i = 0; i < tY; i++){
			for(int j = 0; j < tX; j++){
			  win->DepCase[i][j] = -1;
			} 
		      }
		      
		    }
		    
		    
		    win->EtapeActuelleDujeu = 2;

		  }else if(mousX > win->TailleX/10 && mousX < win->TailleX/10 * 2 && mousY > win->TailleY/10 && mousY < win->TailleY/10 * 2 && win->isSave){
		    char r;
		    char str[100];
		    char append[100];
		    int tX = 4;
		    int tY = 4;
		    save = fopen(SAVE_NAME, "r");
		    if(save == NULL){
		      printf("C'est assez cocasse... Un fichier ne devrais pas disparaitre...\n");
		      exit(0);
		    }
		    
		    win->EtapeActuelleDujeu = 8;
		    // Lectuer de la ligne d'entete 
		    while((r = getc(save)) != '\n'){
		      strcpy(append, "");
		      sprintf(append, "%c", r);
		      strcat(str, append);
		    }  
		    //analyse de la premiere ligne pour recuperer la taille
		    if(str[0] == 'N'){
		      sscanf(str, "N %d %d", &tX, &tY);
		    }
		    
		    // initialisation du plateau
		    //creation du tableau
		    win->plateau.tab = (int ** )malloc(sizeof(int *) * tY);
		    if(win->plateau.tab == NULL){
		      win->EtapeActuelleDujeu = 0;
		    }else{
		      for(int i = 0; i < tY; i++){
			win->plateau.tab[i] = (int *)malloc(sizeof(int) * tX);
			if(win->plateau.tab[i] == NULL){
			  for(int j = 0; j < i; i++){
			    free(win->plateau.tab[j]);
			  }
			  free(win->plateau.tab);
			  win->EtapeActuelleDujeu = 0;
			}
		      }
		    }
		    // On met que toute les cases sont vides
		    for(int i = 0; i < tY; i++){
		      for(int j = 0; j < tX; j++){
			win->plateau.tab[i][j] = -1;
		      } 
		    }
		    // initialisation des autres variables
		    win->plateau.max = 2;
		    win->plateau.score = 0;
		    win->plateau.vide = tX*tY;
		    win->plateau.tailleX = tX;
		    win->plateau.tailleY = tY;

		    if(win->plateau.tailleX == -1){
		      win->EtapeActuelleDujeu = 0;
		    }
		    win->depEnCours = 0;
		    win->timeBegin = getTime() - win->time;

		    win->oldTer = (int ** )malloc(sizeof(int *) * tY);
		    if(win->oldTer == NULL){
		      win->EtapeActuelleDujeu = 0;
		    }else{
		      for(int i = 0; i < tY; i++){
			win->oldTer[i] = (int *)malloc(sizeof(int) * tX);
			if(win->oldTer[i] == NULL){
			  for(int j = 0; j < i; i++){
			    free(win->oldTer[j]);
			  }
			  free(win->oldTer);
			  win->EtapeActuelleDujeu = 0;
			}
		      }
		    }
		    // On met que toute les cases sont vides
		    if(win->oldTer != NULL){
		      for(int i = 0; i < tY; i++){
			for(int j = 0; j < tX; j++){
			  win->oldTer[i][j] = -1;
			}
		      }
		    }
		    win->DepCase = (int ** )malloc(sizeof(int *) * tY);
		    if(win->DepCase == NULL){
		      win->EtapeActuelleDujeu = 0;
		    }else{
		      for(int i = 0; i < tY; i++){
			win->DepCase[i] = (int *)malloc(sizeof(int) * tX);
			if(win->DepCase[i] == NULL){
			  for(int j = 0; j < i; i++){
			    free(win->DepCase[j]);
			  }
			  free(win->DepCase);
			  win->EtapeActuelleDujeu = 0;
			}
		      }
		    }
		    if(win->DepCase != NULL){
		      // On met que toute les cases sont vides
		      for(int i = 0; i < tY; i++){
			for(int j = 0; j < tX; j++){
			  win->DepCase[i][j] = -1;
			} 
		      }
		      
		    }
		    
		    
		    
		   

		  }else if(mousX > 0 && mousX < win->TailleX/10 && mousY > win->TailleY - win->TailleY/10 && mousY < win->TailleY ){
		    win->returnValue = 2;
		    win->EtapeActuelleDujeu = 0;
		  }else if(mousX >  win->TailleX/4 && mousX < win->TailleX/4 * 2 && mousY > win->TailleY/5 * 3 && mousY < win->TailleY/4 + win->TailleY/5 * 3 ){
		    win->EtapeActuelleDujeu = 16;
		  }
		}else if(win->EtapeActuelleDujeu == 4){


		  if(mousX > win->TailleX/100 && mousX < win->TailleX/10 && mousY > win->TailleY/4 && mousY < win->TailleY/4 + win->TailleY/2){
		    char tmp[100];
		    sprintf(tmp, "%s%d", SAVE_REPLAY, win->nbReplay);
		    copiFile(SAVE_NAME, tmp);
		    win->nbReplay++;
		    if(remove(SAVE_NAME) != 0){
		      printf("Il y a un probleme inattendu...\n");
		    }
		    win->EtapeActuelleDujeu = 1;
		    if(remove(SAVE_NAME) != 0){
		      printf("Il y a un probleme inattendu...\n");
		    }
		    win->EtapeActuelleDujeu = 6;
		    
		    writesaveFile(win->hightscore, win->endedGamesWin, win->endedLoseGame, win->mouvsup, win->nbReplay, (win->fullscreen)?win->otherX:win->TailleX,  (win->fullscreen)?win->otherY:win->TailleY, win->fullscreen, win->sound, win->music, win->time);
		  }else if(mousX > win->TailleX - win->TailleX/100 - win->TailleX/10 && mousX < win->TailleX - win->TailleX/100 && mousY > win->TailleY/4 && mousY < win->TailleY/4 + win->TailleY/2 ){
		    win->EtapeActuelleDujeu = 1;
		    if(remove(SAVE_NAME) != 0){
		      printf("Il y a un probleme inattendu...\n");
		    }
		    if(win->plateau.tailleX != -1){
		      if(win->oldTer != NULL){
			for(int i = 0; i < win->plateau.tailleY; i++){
			  if(win->oldTer[i] != NULL){
			    free(win->oldTer[i]);
			  }
			}
			free(win->oldTer);
		      }

		      if(win->DepCase != NULL){
			for(int i = 0; i < win->plateau.tailleY; i++){
			  if(win->DepCase[i] != NULL){
			    free(win->DepCase[i]);
			  }
			}
			free(win->DepCase);
		      }
		    }
		    LibereTer(&win->plateau);
		    
		   
		    writesaveFile(win->hightscore, win->endedGamesWin, win->endedLoseGame, win->mouvsup, win->nbReplay, (win->fullscreen)?win->otherX:win->TailleX,  (win->fullscreen)?win->otherY:win->TailleY, win->fullscreen, win->sound, win->music, win->time); 
		    
		  }

		  
		}else if(win->EtapeActuelleDujeu == 6){

		  if(mousX > win->TailleX - win->TailleX/100 - win->TailleX/10 && mousX < win->TailleX - win->TailleX/100 && mousY > win->TailleY/4 && mousY < win->TailleY/4 + win->TailleY/2 ){
		    win->EtapeActuelleDujeu = 1;
		    if(remove(SAVE_NAME) != 0){
		      printf("Il y a un probleme inattendu...\n");
		    }
		    if(win->plateau.tailleX != -1){
		      if(win->oldTer != NULL){
			for(int i = 0; i < win->plateau.tailleY; i++){
			  if(win->oldTer[i] != NULL){
			    free(win->oldTer[i]);
			  }
			}
			free(win->oldTer);
		      }

		      if(win->DepCase != NULL){
			for(int i = 0; i < win->plateau.tailleY; i++){
			  if(win->DepCase[i] != NULL){
			    free(win->DepCase[i]);
			  }
			}
			free(win->DepCase);
		      }
		    }
		    LibereTer(&win->plateau);
		    
		   
		    writesaveFile(win->hightscore, win->endedGamesWin, win->endedLoseGame, win->mouvsup, win->nbReplay, (win->fullscreen)?win->otherX:win->TailleX,  (win->fullscreen)?win->otherY:win->TailleY, win->fullscreen, win->sound, win->music, win->time); 
		    
		  }
		  
		}else if(win->EtapeActuelleDujeu == 16){

		  for(int i = 0; i < win->nbReplay; i++){

		    if(mousX > win->TailleX/4 && mousX < win->TailleX/4 + win->TailleX/2 && mousY > win->TailleY/(win->nbReplay + 2) * (i+1) && mousY < win->TailleY/(win->nbReplay + 2) * (i+1) + win->TailleY/(win->nbReplay + 3)){

		      char r;
		      char str[100];
		      char append[100];
		      int tX = 4;
		      int tY = 4;
		      char nom[100];
		      sprintf(nom, "%s%d", SAVE_REPLAY, i);
		      save = fopen(nom, "r");
		      if(save == NULL){
			printf("C'est assez cocasse... Un fichier ne devrais pas disparaitre...\n");
			exit(0);
		      }
		    
		      win->EtapeActuelleDujeu = 10;
		      // Lectuer de la ligne d'entete 
		      while((r = getc(save)) != '\n'){
			strcpy(append, "");
			sprintf(append, "%c", r);
			strcat(str, append);
		      }  
		      //analyse de la premiere ligne pour recuperer la taille
		      if(str[0] == 'N'){
			sscanf(str, "N %d %d", &tX, &tY);
		      }
		    
		      // initialisation du plateau
		      //creation du tableau
		      win->plateau.tab = (int ** )malloc(sizeof(int *) * tY);
		      if(win->plateau.tab == NULL){
			win->EtapeActuelleDujeu = 0;
		      }else{
			for(int i = 0; i < tY; i++){
			  win->plateau.tab[i] = (int *)malloc(sizeof(int) * tX);
			  if(win->plateau.tab[i] == NULL){
			    for(int j = 0; j < i; i++){
			      free(win->plateau.tab[j]);
			    }
			    free(win->plateau.tab);
			    win->EtapeActuelleDujeu = 0;
			  }
			}
		      }
		      // On met que toute les cases sont vides
		      for(int i = 0; i < tY; i++){
			for(int j = 0; j < tX; j++){
			  win->plateau.tab[i][j] = -1;
			} 
		      }
		      // initialisation des autres variables
		      win->plateau.max = 2;
		      win->plateau.score = 0;
		      win->plateau.vide = tX*tY;
		      win->plateau.tailleX = tX;
		      win->plateau.tailleY = tY;

		      if(win->plateau.tailleX == -1){
			win->EtapeActuelleDujeu = 0;
		      }
		      win->depEnCours = 0;
		      win->timeBegin = getTime() - win->time;

		      win->oldTer = (int ** )malloc(sizeof(int *) * tY);
		      if(win->oldTer == NULL){
			win->EtapeActuelleDujeu = 0;
		      }else{
			for(int i = 0; i < tY; i++){
			  win->oldTer[i] = (int *)malloc(sizeof(int) * tX);
			  if(win->oldTer[i] == NULL){
			    for(int j = 0; j < i; i++){
			      free(win->oldTer[j]);
			    }
			    free(win->oldTer);
			    win->EtapeActuelleDujeu = 0;
			  }
			}
		      }
		      // On met que toute les cases sont vides
		      if(win->oldTer != NULL){
			for(int i = 0; i < tY; i++){
			  for(int j = 0; j < tX; j++){
			    win->oldTer[i][j] = -1;
			  }
			}
		      }
		      win->DepCase = (int ** )malloc(sizeof(int *) * tY);
		      if(win->DepCase == NULL){
			win->EtapeActuelleDujeu = 0;
		      }else{
			for(int i = 0; i < tY; i++){
			  win->DepCase[i] = (int *)malloc(sizeof(int) * tX);
			  if(win->DepCase[i] == NULL){
			    for(int j = 0; j < i; i++){
			      free(win->DepCase[j]);
			    }
			    free(win->DepCase);
			    win->EtapeActuelleDujeu = 0;
			  }
			}
		      }
		      if(win->DepCase != NULL){
			// On met que toute les cases sont vides
			for(int i = 0; i < tY; i++){
			  for(int j = 0; j < tX; j++){
			    win->DepCase[i][j] = -1;
			  } 
			}
		      
		      }
		    
		      
		    }
		    
		  }

		  
		}
	      } else if (event.button.button == SDL_BUTTON_RIGHT) {
	
	      }
	      break;
	    case SDL_QUIT:
	      
		win->EtapeActuelleDujeu = 0;
	      
	      break;
	    case SDL_VIDEORESIZE:
	      resizingTime = SDL_GetTicks();
	      SDL_Delay(100);
	      win->TailleY = event.resize.h;
	      win->TailleX = event.resize.w;
	      break;
	    default:
	      break;
	    }
	}

      
      LastTick += timeForNewTick;
      tickCount++;
    }else{

      /* Endors le cpu pour garder de la ressource */
      NowTime = getTime();
      
      long SleepForCPU = 0;
      SleepForCPU = (long)(timeForNewTick - (NowTime - LastTick)) / 300;
      SDL_Delay(SleepForCPU);
    } 
  }
  return NULL;
}


extern TTF_Font *RobotoFont;

void DrawPlateau(screen *win){

  SDL_FillRect(win->renderer, NULL, SDL_MapRGB(win->renderer->format, 255, 255, 255));

  int maxi = MIN(win->TailleX, win->TailleY);
  
  SDL_Rect gameRect;
  gameRect.x = win->TailleX/2 - 3*maxi/8;
  gameRect.y = win->TailleY/2 - 3*maxi/8;
  gameRect.w = 3*maxi/4;
  gameRect.h = 3*maxi/4;
  SDL_FillRect(win->renderer, &gameRect, SDL_MapRGB(win->renderer->format, 150, 150, 150));

  gameRect.w = 0.01*maxi;
  gameRect.h = 6.0*maxi/8;
  
  for(int i = 0; i < win->plateau.tailleX+1; i++){
    gameRect.x = win->TailleX/2.0 - 3.0*maxi/8 + (3.0*maxi/((win->plateau.tailleX) * 4))*i;
    SDL_FillRect(win->renderer, &gameRect, SDL_MapRGB(win->renderer->format, 0, 0, 0));
  }
  
  gameRect.x = win->TailleX/2 - 3*maxi/8;
  gameRect.h = 0.01*maxi;
  gameRect.w = 6.0*maxi/8 + 0.01*maxi;
  for(int i = 0; i < win->plateau.tailleY+1; i++){
    gameRect.y = win->TailleY/2.0 - 3.0*maxi/8 + (3.0*maxi/((win->plateau.tailleY) * 4))*i;
    SDL_FillRect(win->renderer, &gameRect, SDL_MapRGB(win->renderer->format, 0, 0, 0));
  }

  SDL_Color Color = {255, 255, 255};


  if(win->timeBegin < getTime()-win->time){
    for(int i = 0; i < win->plateau.tailleY; i++){
      for(int j = 0; j < win->plateau.tailleX; j++){
	if(win->plateau.tab[i][j] != -1){
	  gameRect.w = 3.0*maxi/(4 * win->plateau.tailleX) - 0.03*maxi;
	  gameRect.h = 3.0*maxi/(4 * win->plateau.tailleY) - 0.03*maxi;

	  gameRect.x = win->TailleX/2.0 - 3.0*maxi/8 + (3.0*maxi/((win->plateau.tailleX) * 4))*j + 0.02*maxi;
	  gameRect.y = win->TailleY/2.0 - 3.0*maxi/8 + (3.0*maxi/((win->plateau.tailleY) * 4))*i + 0.02*maxi;
	  SDL_FillRect(win->renderer, &gameRect, getColor(win->plateau.tab[i][j], win));
	  
	  char nb[50];
	  sprintf(nb, "%d", win->plateau.tab[i][j]);

	
	  SDL_Surface* surfaceMessage = TTF_RenderText_Solid(RobotoFont, nb, Color);
	
	  float res;
	  //printf("res : %f %d %d \n", surfaceMessage->h*res, gameRect.w, gameRect.h);
	  if((float)(gameRect.w)/surfaceMessage->w < (float)(gameRect.h)/surfaceMessage->h){
	    res = (float)(gameRect.w)/surfaceMessage->w;
	    gameRect.y += gameRect.h/2.0 - surfaceMessage->h*res/2; 
	  }else{
	    res = (float)(gameRect.h)/surfaceMessage->h;
	    gameRect.x += gameRect.w/2.0 - surfaceMessage->w*res/2; 
	  }
	  
	  SDL_Surface *tmp = rotozoomSurface(surfaceMessage, 0.0, res, 0);
	
	  SDL_BlitSurface(tmp, NULL, win->renderer, &gameRect);
	
	  SDL_FreeSurface(tmp);
	  SDL_FreeSurface(surfaceMessage);
	
	
	}
      }
    }
  }else {
    int Ynext = (win->depEnCours == 1)?1:(win->depEnCours == 3)?-1:0;
    int Xnext = (win->depEnCours == 4)?1:(win->depEnCours == 2)?-1:0;
    
    for(int i = 0; i < win->plateau.tailleY; i++){
      for(int j = 0; j < win->plateau.tailleX; j++){
	if(win->oldTer[i][j] != -1){
	  gameRect.w = 3.0*maxi/(4 * win->plateau.tailleX) - 0.03*maxi;
	  gameRect.h = 3.0*maxi/(4 * win->plateau.tailleY) - 0.03*maxi;

	  gameRect.x = win->TailleX/2.0 - 3.0*maxi/8 + (3.0*maxi/((win->plateau.tailleX) * 4))*j + 0.02*maxi + (gameRect.w+0.03*maxi)*Xnext*win->DepCase[i][j] * (float)(getTime()-win->timeBegin)/win->time ;
	  gameRect.y = win->TailleY/2.0 - 3.0*maxi/8 + (3.0*maxi/((win->plateau.tailleY) * 4))*i + 0.02*maxi + (gameRect.h+0.03*maxi)*Ynext*win->DepCase[i][j] * (float)(getTime()-win->timeBegin)/win->time;
	  
	  SDL_FillRect(win->renderer, &gameRect, getColor(win->oldTer[i][j], win));
	  
	  char nb[50];
	  sprintf(nb, "%d", win->oldTer[i][j]);

	
	  SDL_Surface* surfaceMessage = TTF_RenderText_Solid(RobotoFont, nb, Color);
	
	  float res;
	  //printf("res : %f %d %d \n", surfaceMessage->h*res, gameRect.w, gameRect.h);
	  if((float)(gameRect.w)/surfaceMessage->w < (float)(gameRect.h)/surfaceMessage->h){
	    res = (float)(gameRect.w)/surfaceMessage->w;
	    gameRect.y += gameRect.h/2.0 - surfaceMessage->h*res/2; 
	  }else{
	    res = (float)(gameRect.h)/surfaceMessage->h;
	    gameRect.x += gameRect.w/2.0 - surfaceMessage->w*res/2; 
	  }
	  
	  SDL_Surface *tmp = rotozoomSurface(surfaceMessage, 0.0, res, 0);
	
	  SDL_BlitSurface(tmp, NULL, win->renderer, &gameRect);
	
	  SDL_FreeSurface(tmp);
	  SDL_FreeSurface(surfaceMessage);
	
	
	}
      }
    }
  }
}


void DrawMenu(screen *win){
  SDL_FillRect(win->renderer, NULL, SDL_MapRGB(win->renderer->format, 255, 255, 255));
  int mousX = 0;
  int mousY = 0;
  
  SDL_Rect gameRect;

  SDL_GetMouseState(&mousX, &mousY);

  
  gameRect.x = win->TailleX/4;
  gameRect.y = win->TailleY/4;
  gameRect.w = win->TailleX/4;
  gameRect.h = win->TailleY/4;

  if(mousX > win->TailleX/4 && mousX < win->TailleX/4 * 2 && mousY > win->TailleY/4 && mousY < win->TailleY/4 * 2 ){
    SDL_FillRect(win->renderer, &gameRect, SDL_MapRGB(win->renderer->format, 100, 100, 100));
  }else{
    SDL_FillRect(win->renderer, &gameRect, SDL_MapRGB(win->renderer->format, 0, 0, 0));
  }

  gameRect.x = win->TailleX/10;
  gameRect.y = win->TailleY/10;
  gameRect.w = win->TailleX/10;
  gameRect.h = win->TailleY/10;
  if(win->isSave){
    if(mousX > win->TailleX/10 && mousX < win->TailleX/10 * 2 && mousY > win->TailleY/10 && mousY < win->TailleY/10 * 2 ){
      SDL_FillRect(win->renderer, &gameRect, SDL_MapRGB(win->renderer->format, 100, 100, 100));
    }else{
      SDL_FillRect(win->renderer, &gameRect, SDL_MapRGB(win->renderer->format, 0, 0, 0));
    }
  }

  gameRect.x = 0;
  gameRect.y = win->TailleY-win->TailleY/10;
  gameRect.w = win->TailleX/10;
  gameRect.h = win->TailleY/10;

  if(mousX > 0 && mousX < win->TailleX/10 && mousY > win->TailleY - win->TailleY/10 && mousY < win->TailleY ){
    SDL_FillRect(win->renderer, &gameRect, SDL_MapRGB(win->renderer->format, 0, 0, 255));
  }else{
    SDL_FillRect(win->renderer, &gameRect, SDL_MapRGB(win->renderer->format, 210, 210, 255));
  }


  gameRect.x = win->TailleX/4;
  gameRect.y = win->TailleY/5 * 3;
  gameRect.w = win->TailleX/4;
  gameRect.h = win->TailleY/4;

  if(mousX >  win->TailleX/4 && mousX < win->TailleX/4 * 2 && mousY > win->TailleY/5 * 3 && mousY < win->TailleY/4 + win->TailleY/5 * 3 ){
    SDL_FillRect(win->renderer, &gameRect, SDL_MapRGB(win->renderer->format, 100, 100, 100));
  }else{
    SDL_FillRect(win->renderer, &gameRect, SDL_MapRGB(win->renderer->format, 0, 0, 0));
  }
  
}


void DrawGameOver(screen *win){



  int mousX;
  int mousY;
  SDL_Rect gameRect;
  int maxi = MIN(win->TailleX, win->TailleY);

  
  SDL_GetMouseState(&mousX, &mousY);

  gameRect.x = win->TailleX/2 - 3*maxi/8;
  gameRect.y = win->TailleY/2 - 3*maxi/8;
  gameRect.w = 3.0*maxi/4 + 0.01*maxi;
  gameRect.h = 3.0*maxi/4 + 0.01*maxi;
  SDL_Surface *surf = SDL_CreateRGBSurface(0, gameRect.w, gameRect.h, 32, 150, 150, 150, 150);
  SDL_FillRect(surf, NULL, 0x0000000F);
  SDL_BlitSurface(surf, NULL, win->renderer, &gameRect);

  SDL_FreeSurface(surf);


  if(win->plateau.max < 2048){
    DrawString("Game Over", 50, 50, 15, 'c', 255, 255, 255, *win);
  }else{
    DrawString("Well Done", 50, 50, 15, 'c', 255, 255, 255, *win);
  }

  
  
  gameRect.x = win->TailleX/100;
  gameRect.y = win->TailleY/4;
  gameRect.w = win->TailleX/10 - win->TailleX/100;
  gameRect.h = win->TailleY/2;

  if(mousX > win->TailleX/100 && mousX < win->TailleX/10 && mousY > win->TailleY/4 && mousY < win->TailleY/4 + win->TailleY/2 ){
    SDL_FillRect(win->renderer, &gameRect, SDL_MapRGB(win->renderer->format, 100, 100, 100));
  }else{
    SDL_FillRect(win->renderer, &gameRect, SDL_MapRGB(win->renderer->format, 0, 0, 0));
  }

  DrawString("Save", 6, 42, 6, 'c', 255, 255, 255, *win);
  DrawString("replay", 6, 49, 6, 'c', 255, 255, 255, *win);
  DrawString("?", 6, 55, 6, 'c', 255, 255, 255, *win);


  gameRect.x = win->TailleX - win->TailleX/100 - win->TailleX/10;
  gameRect.y = win->TailleY/4;
  gameRect.w = win->TailleX/10 - win->TailleX/100;
  gameRect.h = win->TailleY/2;

  if(mousX > win->TailleX - win->TailleX/100 - win->TailleX/10 && mousX < win->TailleX - win->TailleX/100 && mousY > win->TailleY/4 && mousY < win->TailleY/4 + win->TailleY/2 ){
    SDL_FillRect(win->renderer, &gameRect, SDL_MapRGB(win->renderer->format, 100, 100, 100));
  }else{
    SDL_FillRect(win->renderer, &gameRect, SDL_MapRGB(win->renderer->format, 0, 0, 0));
  }


  DrawString("menu", 94, 49, 6, 'c', 255, 255, 255, *win);
}


void DrawSavedOver(screen *win){



  int mousX;
  int mousY;
  SDL_Rect gameRect;
  int maxi = MIN(win->TailleX, win->TailleY);

  
  SDL_GetMouseState(&mousX, &mousY);

  gameRect.x = win->TailleX/2 - 3*maxi/8;
  gameRect.y = win->TailleY/2 - 3*maxi/8;
  gameRect.w = 3.0*maxi/4 + 0.01*maxi;
  gameRect.h = 3.0*maxi/4 + 0.01*maxi;
  SDL_Surface *surf = SDL_CreateRGBSurface(0, gameRect.w, gameRect.h, 32, 150, 150, 150, 150);
  SDL_FillRect(surf, NULL, 0x0000000F);
  SDL_BlitSurface(surf, NULL, win->renderer, &gameRect);

  SDL_FreeSurface(surf);


  if(win->plateau.max < 2048){
    DrawString("Game Over", 50, 50, 15, 'c', 255, 255, 255, *win);
  }else{
    DrawString("Well Done", 50, 50, 15, 'c', 255, 255, 255, *win);
  }
  

  gameRect.x = win->TailleX - win->TailleX/100 - win->TailleX/10;
  gameRect.y = win->TailleY/4;
  gameRect.w = win->TailleX/10 - win->TailleX/100;
  gameRect.h = win->TailleY/2;

  if(mousX > win->TailleX - win->TailleX/100 - win->TailleX/10 && mousX < win->TailleX - win->TailleX/100 && mousY > win->TailleY/4 && mousY < win->TailleY/4 + win->TailleY/2 ){
    SDL_FillRect(win->renderer, &gameRect, SDL_MapRGB(win->renderer->format, 100, 100, 100));
  }else{
    SDL_FillRect(win->renderer, &gameRect, SDL_MapRGB(win->renderer->format, 0, 0, 0));
  }


  DrawString("menu", 94, 49, 6, 'c', 255, 255, 255, *win);


  
}




void recupeDep(screen *win){
  for(int i = 0; i < win->plateau.tailleY; i++){
    for(int j = 0; j < win->plateau.tailleX; j++){
      win->oldTer[i][j] = win->plateau.tab[i][j];
      win->DepCase[i][j] = 0;
    }
  }
  int Ynext = (win->depEnCours == 1)?1:(win->depEnCours == 3)?-1:0;
  int Xnext = (win->depEnCours == 4)?1:(win->depEnCours == 2)?-1:0;
  // varible necessaire a pas que 0 2 2 4 se transforme en 0 0 0 8 mais bien en 0 0 4 4
  int *isfusX = (int *)malloc(win->plateau.tailleX * win->plateau.tailleY/2 * sizeof(int));
  int *isfusY = (int *)malloc(win->plateau.tailleX * win->plateau.tailleY/2 * sizeof(int));
  for(int i = 0; i < win->plateau.tailleX * win->plateau.tailleY/2; i++){
    // les tableaux sont initialises vide
    isfusX[i] = -1;
    isfusY[i] = -1;
  }
  // pour chacune des cases (le sens de la boucle depend de Xnext et Ynext)
  for(int i = (Ynext == 1)?win->plateau.tailleY-2:(Ynext == -1)?1:0; (Ynext == 1)?i > -1:i < win->plateau.tailleY; (Ynext == 1)?i--:i++){
    for(int j = (Xnext == 1)?win->plateau.tailleX-2:(Xnext == -1)?1:0; (Xnext == 1)?j > -1:j < win->plateau.tailleX; (Xnext == 1)?j--:j++){
      // si la case n est pas vide
      if(win->plateau.tab[i][j] != -1){
	// nombre de deplacement entre la case et son point d arrive
        int whereGo = 0;
        // pour toute les cases dans la direction appuye par le joueur
        for(int k = 1; k < MAX(win->plateau.tailleX, win->plateau.tailleY); k++){
          if(i+(Ynext*k) < win->plateau.tailleY && i+(Ynext*k) > -1 && j+(Xnext*k) <win->plateau.tailleX && j+(Xnext*k) > -1){
            // si la case est vide
            if(win->plateau.tab[i+(Ynext*k)][j+(Xnext*k)] == -1){
              // la case devient la nouvelle plus loin sur laquelle elle peu aller
	      whereGo = k;
            // si la case est de meme valeurs
            }else if(win->plateau.tab[i+(Ynext*k)][j+(Xnext*k)] == win->plateau.tab[i][j]){
              // peut elle se combiner
              int PeuDuo = 1;
              // pour toute les cases deja combine lors de se deplacement
              for(int m = 0; m < win->plateau.tailleX * win->plateau.tailleY/2; m++){
                if(isfusX[m] == -1){
                  break;
                // si c la case appartient
                }else if(isfusX[m] == i+(Ynext*k) && isfusY[m] == j+(Xnext*k)){
                  //alors elle ne peu pas se combiner
                  PeuDuo = 0;
                  break;
                }
              }
              //si elle peu se combiner
              if(PeuDuo){
                whereGo = k;
                // on sort cr la case ne peu pas aller plus loins
                break;
              }else{
                //si la case ne peu pas se combiner on sort la case ne peu pas
                //aller plus loins que un cran en moins
                break;
              }
            }else{
              //si le nouvelle case n est ni vide ni de ca valleur
              break;
            }
          }else {
            // si la case est bloque par un mur
            break;
          }
        }
        //si le deplacement implique que la case a bouge
        if(whereGo != 0){
	  win->DepCase[i][j] = whereGo;
          //si la nouvelle case n est pas vide
          if(win->plateau.tab[i+(Ynext*whereGo)][j+(Xnext*whereGo)] != -1){
            //on met les deux cases dans la nouvelle
            win->plateau.tab[i+(Ynext*whereGo)][j+(Xnext*whereGo)] += win->plateau.tab[i][j];
            //on vide l ancienne case
            win->plateau.tab[i][j] = -1;
            // on met a jour le score
            win->plateau.score += win->plateau.tab[i+(Ynext*whereGo)][j+(Xnext*whereGo)];
            // on met a jour le maximum
            win->plateau.max = MAX(win->plateau.max, win->plateau.tab[i+(Ynext*whereGo)][j+(Xnext*whereGo)]);
            // on met que une case vient d etre libere
            win->plateau.vide++;
            // on met que la case vient d etre fusionne sur ce tour
            int new = 0;
            while(isfusX[new] != -1){new++;}
            isfusX[new] = i+(Ynext*whereGo);
            isfusY[new] = j+(Xnext*whereGo);
          }else{
            // sinon on met la nouvelle case a la valeur de notre case et la
            // precedente a vide
            win->plateau.tab[i+(Ynext*whereGo)][j+(Xnext*whereGo)] = win->plateau.tab[i][j];
            win->plateau.tab[i][j] = -1;
          }
        }
      }
    }
  }  
  // on libere la memoire
  free(isfusX);
  free(isfusY);  
}





void DrawSelectReplay(screen *win){
  SDL_FillRect(win->renderer, NULL, SDL_MapRGB(win->renderer->format, 255, 255, 255));
  int mousX = 0;
  int mousY = 0;
  
  SDL_Rect gameRect;

  SDL_GetMouseState(&mousX, &mousY);

  for(int i = 0; i < win->nbReplay; i++){
    gameRect.x = win->TailleX/4;
    gameRect.y = win->TailleY/(win->nbReplay + 2) * (i+1);
    gameRect.w = win->TailleX/2;
    gameRect.h = win->TailleY/(win->nbReplay + 3);

    if(mousX > gameRect.x && mousX < gameRect.x+gameRect.w && mousY > gameRect.y && mousY < gameRect.y + gameRect.h ){
      SDL_FillRect(win->renderer, &gameRect, SDL_MapRGB(win->renderer->format, 100, 100, 100));
    }else{
      SDL_FillRect(win->renderer, &gameRect, SDL_MapRGB(win->renderer->format, 0, 0, 0));
    }
  }
  
}


Uint32 getColor(int value, screen *win){

  switch(value){
  case 0:return SDL_MapRGB(win->renderer->format, 0, 0, 0);
  case 2:return SDL_MapRGB(win->renderer->format, 255, 0, 0);
  case 4:return SDL_MapRGB(win->renderer->format, 255, 25, 0);
  case 8:return SDL_MapRGB(win->renderer->format, 255, 50, 0);
  case 16:return SDL_MapRGB(win->renderer->format, 255, 75, 0);
  case 32:return SDL_MapRGB(win->renderer->format, 255, 101, 0);
  case 64:return SDL_MapRGB(win->renderer->format, 255, 126, 0);
  case 128:return SDL_MapRGB(win->renderer->format, 255, 152, 0);
  case 256:return SDL_MapRGB(win->renderer->format, 255, 177, 0);
  case 512:return SDL_MapRGB(win->renderer->format, 255, 203, 0);
  case 1024:return SDL_MapRGB(win->renderer->format, 255, 228, 0);
  case 2048:return SDL_MapRGB(win->renderer->format, 255, 255, 0);
  case 4096:return SDL_MapRGB(win->renderer->format, 0, 0, 255);
  case 8192:return SDL_MapRGB(win->renderer->format, 51, 51, 255);
  case 16384:return SDL_MapRGB(win->renderer->format, 102, 102, 255);
  case 32768:return SDL_MapRGB(win->renderer->format, 153, 153, 255);
  case 65536:return SDL_MapRGB(win->renderer->format, 204, 204, 255);
  default :return SDL_MapRGB(win->renderer->format, 255/(log2(value-131072)/log2(2)), 255/(log2(value-131072)/log2(2)), 255/(log2(value-131072)/log2(2)));
  }
  
}
