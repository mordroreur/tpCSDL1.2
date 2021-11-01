#include "render.h"


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////      Recuperation des variables globale     ///////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////



extern int EtapeActuelleDuJeu; /* 0 = fin; 1 = Loading Screen... */

extern int TailleEcranLong; /* Taille de l'ecran en nombre de pixel de gauche a droite */
extern int TailleEcranHaut; /* Taille de l'ecran en nombre de pixel de haut en bas */

extern SDL_Surface * renderer; /* Ecran sur lequel on dessine */

extern SDL_Event event; /* Les events que l'on récupèrera via SDL. */

extern int DEBUG; /* 1 : affiche le debug */

extern TTF_Font *RobotoFont; /* Font utilise pour ecrire dans le programme */

extern int unlockLVL; /* Dernier niveau debloque */

extern int fullscreen; /* Si le niveau est en plein ecran au demarage */

extern level Actulvl; /* Niveau actuelle sur lequel on joue */

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////



static int otherX;
static int otherY;

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

  
  
  /*************Initailisation de la fenetre***********/
  create_Win();

  
  /************Initialisation des variables de temps**************/
  LastFrame = getTime();
  TimeCount = getTime();
  NowTime = getTime();


  /************Début de la boucle des ticks***********************/
  pthread_t threadBoucleDesTicks;
  int RetourDuThreadDesTicks = pthread_create(&threadBoucleDesTicks, NULL, BouclePrincipaleDesTicks,  NULL);
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
  while (EtapeActuelleDuJeu) {
    NowTime = getTime();

    /* Gestion de l'affichage écran */
    if (NowTime - LastFrame > timeForNewFrame) {
      if(resizingTime == 0){

	
	switch (EtapeActuelleDuJeu) {
	case 1: LoadingScreen();break;
	case 2: DrawMenu();break;
	case 3: DrawMenu();DrawExit();break;
	case 41: LoadingScreen();break;
	case 42: Draw1player();break;
	default: EtapeActuelleDuJeu = 0;break;
	}
	
	if(DEBUG){
	  char affichageFrameDebug[5];
	  sprintf(affichageFrameDebug, "%d", LastFpsCount);
	  DrawString(affichageFrameDebug, 0, 0, 6, 'n', 0, 0, 0);
	  sprintf(affichageFrameDebug, "%d", LastTickCount);
	  DrawString(affichageFrameDebug, 100, 0, 6, 'e', 0, 0, 0);}
      
	SDL_Flip(renderer);

      
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
  end_sdl(1, "Normal ending");

  return EXIT_SUCCESS;
}




void end_sdl(char ok, char const * msg) {
  char msg_formated[255];
  int l;

  if(RobotoFont != NULL){
    TTF_CloseFont(RobotoFont);
  }
  freeImageMalloc();
  
  if (!ok){
    strncpy(msg_formated, msg, 250);
    l = strlen(msg_formated);
    strcpy(msg_formated+l, " : %s\n");
    fprintf(stderr,"%s", msg_formated);
  }

  
  if(renderer != NULL){
    SDL_FreeSurface(renderer);
  }

  SDL_Quit();

  if (!ok) {exit(EXIT_FAILURE);}
}





void create_Win() {

  /* Pour mettre un icon au jeu : */
  //SDL_WM_SetIcon(IMG_Load("caisse.jpg"), NULL); // Si utilisé pas oublié #include sdl image

  
  /* Initialisation de la SDL  + gestion de l'échec possible */
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
    end_sdl(0, "ERROR SDL INIT");

  /* Recuperation de la 'meilleur' taille de fenetre. */
  const SDL_VideoInfo* info = SDL_GetVideoInfo();
  otherX = info->current_w;
  otherY = info->current_h;
  if(otherX > otherY*2){
    otherX = otherX/2;
  }
  

  
  //  printf("Taille de l'écran\n\tw : %d\n\th : %d\n", TailleEcranLong,
  //	 TailleEcranHaut);

  /* Creation de la fenetre de jeu. */
  if(fullscreen == 0){
    if((renderer = SDL_SetVideoMode(TailleEcranLong,TailleEcranHaut,32,
				    SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF ))==NULL){
      end_sdl(0, "ERROR SDL VIDEOMODE");
    }
  }else if(fullscreen == 1){
    if((renderer = SDL_SetVideoMode(otherX,otherY,32,
				    SDL_HWSURFACE | SDL_FULLSCREEN | SDL_DOUBLEBUF ))==NULL){
      end_sdl(0, "ERROR SDL VIDEOMODE");
    }
    otherX = TailleEcranLong;
    otherY = TailleEcranHaut;
    TailleEcranLong = renderer->w;
    TailleEcranHaut = renderer->h;
  }
  /* Mise d'un titre a la fenetre. */
  SDL_WM_SetCaption("THIS IS A F****** WINDOWS",NULL) ;




  

  // écrire (lettre chiffre) dans le render, grace à la bibliothèque TTF
  if (TTF_Init() == -1)
    {
      printf("TTF_Init: %s\n", TTF_GetError());
      exit(2);
    }

  
  /* Recuperation de la taille de la fenetre. */
  info = SDL_GetVideoInfo();
  TailleEcranLong = info->current_w;
  TailleEcranHaut = info->current_h;

  
  
  RobotoFont = TTF_OpenFont("Res/Font/Roboto-Black.ttf", 50);


  

}


void keyUp(SDL_KeyboardEvent *key){
  //printf("%c\n", key->keysym.sym);
  switch(key->keysym.sym){
  case SDLK_ESCAPE:if(EtapeActuelleDuJeu%2 == 0){EtapeActuelleDuJeu++;}else if(EtapeActuelleDuJeu == 3){EtapeActuelleDuJeu--;}else{EtapeActuelleDuJeu = 0;}break;
  case SDLK_F11: resizingTime = SDL_GetTicks();
    SDL_Delay(100);
    int x = otherX;
    int y = otherY;
    otherX = TailleEcranLong;
    otherY = TailleEcranHaut;
    if(renderer->flags == 16){
      SDL_FreeSurface(renderer);
      renderer = SDL_SetVideoMode(x,y,32, SDL_HWSURFACE | SDL_FULLSCREEN | SDL_DOUBLEBUF );
    }else{
      SDL_FreeSurface(renderer);
      renderer = SDL_SetVideoMode(x,y,32, SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF );
    }
    TailleEcranLong = x;
    TailleEcranHaut = y;
    break;
  default:break;
  }
}






void *BouclePrincipaleDesTicks(void *CeciEstUneVaribleNull){
  
  long int LastTick;
  long int timeForNewTick = 1000000 / TICK_TO_GET;
  long int NowTime;

  int mousX;
  int mousY;

  
  NowTime = getTime();
  LastTick = getTime();

  
  while(EtapeActuelleDuJeu){

    NowTime = getTime();
    
    /* Gestion des verif gameplay */
    if (NowTime - LastTick > timeForNewTick) {
      

      if(resizingTime != 0){
	if(resizingTime+500 < SDL_GetTicks()){
	  FILE *Save = fopen("Res/Sauvegarde", "r+");
	  getc(Save); getc(Save);
	  if(otherY > TailleEcranHaut){
	    fprintf(Save, "n\n%d\n%d\n", TailleEcranLong, TailleEcranHaut);
	    fflush(Save);
	    SDL_FreeSurface(renderer);
	    if((renderer = SDL_SetVideoMode(TailleEcranLong,TailleEcranHaut,32,
					    SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF )) == NULL){
	      end_sdl(0, "ERROR SDL VIDEOMODE");
	    }
	  }else{
	    fprintf(Save, "f\n%d\n%d\n", otherX, otherY);
	    fflush(Save);
	  }
	  fclose(Save);
	  resizingTime = 0;
	}
      }
      //printf("%d %d\n", renderer->w, renderer->h);

      
      /* Gestion des imputs clavier */
      while (SDL_PollEvent(&event))
	{
	  switch (event.type)
	    {
	    case SDL_KEYDOWN:
	      break; // KeyDown(&event.key);break;
	    case SDL_KEYUP:
	      keyUp(&event.key);
	      break;
	      /*      case SDL_MOUSEWHEEL:
	      // if (event.wheel.y > 0) {
	      // } else if (event.wheel.y < 0) {
	      // }
	      break;*/
	    case SDL_MOUSEBUTTONDOWN:
	      SDL_GetMouseState(&mousX, &mousY);
	      if (event.button.button == SDL_BUTTON_LEFT) {
		if(EtapeActuelleDuJeu == 2){
		  if((mousX < (float)TailleEcranLong/100 *75 && mousX > (float)TailleEcranLong/100 * 25) && (mousY < (float)TailleEcranHaut/100*25 + (float)TailleEcranLong/400*50 && mousY > (float)TailleEcranHaut/100 *25)){
		    EtapeActuelleDuJeu = 41;
		    Actulvl = initNextLVL();
		    EtapeActuelleDuJeu = 42;
		    /*for(int i = 0; i < Actulvl.TerX; i++){
		      for(int j = 0; j < Actulvl.TerY; j++){
			printf("%d", Actulvl.ter.cellule[i][j].type);
		      }
		      printf("\n");
		      }*/
		  }
		}else if(EtapeActuelleDuJeu == 3){
		    if((mousX < (float)TailleEcranLong/100 *75 -((float)TailleEcranLong/2)/64 *6  && mousX > (float)TailleEcranLong/100 * 25 + ((float)TailleEcranLong/2)/64 *38) && (mousY < (float)TailleEcranHaut/100*(50-100/6) + ((float)TailleEcranLong/4)/32 * 28 && mousY > (float)TailleEcranHaut/100 *(50-100/6) + ((float)TailleEcranLong/4)/32 * 19)){
		      EtapeActuelleDuJeu = 0;
		    }else if((mousX < (float)TailleEcranLong/100 *75 -((float)TailleEcranLong/2)/64 *38  && mousX > (float)TailleEcranLong/100 * 25 + ((float)TailleEcranLong/2)/64 *6) && (mousY < (float)TailleEcranHaut/100*(50-100/6) + ((float)TailleEcranLong/4)/32 * 28 && mousY > (float)TailleEcranHaut/100 *(50-100/6) + ((float)TailleEcranLong/4)/32 * 19)){
		      EtapeActuelleDuJeu--;
		    }
		}
	      } else if (event.button.button == SDL_BUTTON_RIGHT) {
	      }
	      break;
	    case SDL_QUIT:
	      if(EtapeActuelleDuJeu == 2){
		EtapeActuelleDuJeu++;
	      }else {
		EtapeActuelleDuJeu = 0;
	      }
	      break;
	    case SDL_VIDEORESIZE:
	      resizingTime = SDL_GetTicks();
	      SDL_Delay(100);
	      TailleEcranHaut = event.resize.h;
	      TailleEcranLong = event.resize.w;
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



