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


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////




//TTF_Font *RobotoFont;





static int tickCount = 0;



int BouclePrincipaleDuJeu(){

  long int LastFrame;
  long int TimeCount;
  long int NowTime;
  long int timeForNewFrame = 1000000 / FPS_TO_GET;

  //Debogage
  int fpsCount = 0;
  int LastFpsCount = 0;
  int LastTickCount = 0;

  
  
  


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
    
  
  /************Début de la boucle frames**************************/
  while (EtapeActuelleDuJeu) {
    NowTime = getTime();

    /* Gestion de l'affichage écran */
    if (NowTime - LastFrame > timeForNewFrame) {

      SDL_Surface *rectangle = NULL;
      rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, 1,1, 32,0,0,0,0);

      SDL_FillRect(renderer, NULL, SDL_MapRGB(renderer->format, 0,0,0));
      SDL_FillRect(rectangle, NULL, SDL_MapRGB(rectangle->format,25,26*4,4*4));




      /*
      if(DEBUG){
	char affichageFrameDebug[5];
	sprintf(affichageFrameDebug, "%d", LastFpsCount);
	DrawString(affichageFrameDebug, 0, 0, 6, 'n', 0, 0, 0);
	sprintf(affichageFrameDebug, "%d", LastTickCount);
	DrawString(affichageFrameDebug, 100, 0, 6, 'e', 0, 0, 0);}*/
      
      SDL_Flip(renderer);

      LastFrame += timeForNewFrame;
      fpsCount++;
    }else{

      /* Endors le cpu pour garder de la ressource */
      NowTime = getTime();
      
      long SleepForCPU = 0;
      SleepForCPU = (long)(timeForNewFrame - (NowTime - LastFrame)) / 300;
      SDL_Delay(SleepForCPU);
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
	/*      case SDL_WINDOWEVENT:
	if(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED){
	  TailleEcranHaut = event.window.data2;
	  TailleEcranLong = event.window.data1;
	}
	break;*/
      default:
        break;
      }
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
  /* Initialisation de la SDL  + gestion de l'échec possible */
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
    end_sdl(0, "ERROR SDL INIT");

  /* Recuperation de la 'meilleur' taille de fenetre. */
  const SDL_VideoInfo* info = SDL_GetVideoInfo();
  TailleEcranLong = info->current_w;
  TailleEcranHaut = info->current_h;
  printf("Taille de l'écran\n\tw : %d\n\th : %d\n", TailleEcranLong,
	 TailleEcranHaut);

  /* Creation de la fenetre de jeu. */
  if((renderer = SDL_SetVideoMode(TailleEcranLong,TailleEcranHaut,8,
				  SDL_HWSURFACE | SDL_RESIZABLE /*| SDL_DOUBLEBUF */))==NULL){
    end_sdl(0, "ERROR SDL VIDEOMODE");
  }
  
  /* Mise d'un titre a la fenetre. */
  SDL_WM_SetCaption("THIS IS A F****** WINDOWS",NULL) ;




  

  // écrire (lettre chiffre) dans le render, grace à la bibliothèque TTF
  /*  if (TTF_Init() == -1)
  {
    printf("TTF_Init: %s\n", TTF_GetError());
    exit(2);
    }*/

  /* Taille de écran fournit par SDL */
  //SDL_GetWindowSize(window, &TailleEcranLong, &TailleEcranHaut);

  
  //RobotoFont = TTF_OpenFont("Res/Roboto-Black.ttf", 50);
  
}


void keyUp(SDL_KeyboardEvent *key){
  //printf("%c\n", key->keysym.sym);
  switch(key->keysym.sym){
  case SDLK_ESCAPE:EtapeActuelleDuJeu = 0;break;
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



