#include "render.h"
#include "RenderUtilities.h"
#include <SDL/SDL_timer.h>
#include <SDL/SDL_video.h>



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

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////




//TTF_Font *RobotoFont;



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
  InitImage();

  
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


  
  /************Début de la boucle frames**************************/
  while (EtapeActuelleDuJeu) {
    NowTime = getTime();

    /* Gestion de l'affichage écran */
    if (NowTime - LastFrame > timeForNewFrame) {
      if(resizingTime == 0){

	
	SDL_FillRect(renderer, NULL, SDL_MapRGB(renderer->format, 255,255,255));
      
	for(int i = 0; i < 10; i++){
	  DrawImage(0, 0, 10*i, 0, 10, 'n', 0, 0, 0, 0);      
	}

	char affichageFrameDebug[5];
	sprintf(affichageFrameDebug, "%d", TailleEcranLong);
	DrawString(affichageFrameDebug, 50, 50, 6, 'c', 0, 0, 0);
	
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
  //  printf("Taille de l'écran\n\tw : %d\n\th : %d\n", TailleEcranLong,
  //	 TailleEcranHaut);

  /* Creation de la fenetre de jeu. */
  if((renderer = SDL_SetVideoMode(TailleEcranLong,TailleEcranHaut,32,
				  SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF ))==NULL){
    end_sdl(0, "ERROR SDL VIDEOMODE");
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

  
  
  RobotoFont = TTF_OpenFont("Res/Roboto-Black.ttf", 50);


  

}


void keyUp(SDL_KeyboardEvent *key){
  //printf("%c\n", key->keysym.sym);
  switch(key->keysym.sym){
  case SDLK_ESCAPE:EtapeActuelleDuJeu = 0;break;
  case SDLK_F11: resizingTime = SDL_GetTicks();
    SDL_Delay(10);
    int x = otherX;
    int y = otherY;
    otherX = TailleEcranLong;
    otherY = TailleEcranHaut;
    if(renderer->flags == 16){
      renderer = SDL_SetVideoMode(x,y,32, SDL_HWSURFACE | SDL_FULLSCREEN | SDL_DOUBLEBUF );
    }else{
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
  NowTime = getTime();
  LastTick = getTime();

  
  while(EtapeActuelleDuJeu){

    NowTime = getTime();
    
    /* Gestion des verif gameplay */
    if (NowTime - LastTick > timeForNewTick) {
      

      if(resizingTime != 0){
	if(resizingTime+500 < SDL_GetTicks()){
	  resizingTime = 0;
	}
      }

      
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
	      // if (event.button.button == SDL_BUTTON_LEFT) {
	      // } else if (event.button.button == SDL_BUTTON_RIGHT) {
	      // }
	      break;
	    case SDL_QUIT:
	      EtapeActuelleDuJeu = 0;
	      break;
	    case SDL_VIDEORESIZE:
	      resizingTime = SDL_GetTicks();
	      SDL_Delay(100);
	      printf("on resize\n");
	      TailleEcranHaut = event.resize.h;
	      TailleEcranLong = event.resize.w;
	      renderer = SDL_SetVideoMode(TailleEcranLong,TailleEcranHaut,32,
					  SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF );
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



