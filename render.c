#include "render.h"
#include "Draw.h"
#include "RenderUtilities.h"
#include <SDL/SDL_error.h>
#include <stdio.h>


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

extern int InputVal[2][4]; /* Valeur en int des inputs pour les action joueurs */

extern int GetInput[2][6]; /* Valeur de si le joueur appuie */

extern int HaveType; /* Valeur de la case sous le pointeur lors de la creation */

extern float camx; /* Décalage de la caméra en X */
extern float camy; /* Décalage de la caméra en Y */

extern int nbBlockY; /*Nb Blocks a afficher sur la hauteur de l'écran*/
extern int nbBlockX; /*Nb Blocks a afficher sur la longueur de l'écran*/

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////



int otherX;
int otherY;

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
	case 43: Draw1player();DrawExit();break;
	case 52: DrawCreaLVL();break;
	case 53: DrawCreaLVL();DrawExit();break;
	case 55: Draw1player();break;
	default: EtapeActuelleDuJeu = 0;break;
	}
	
	if(DEBUG){
	  char affichageFrameDebug[5];
	  sprintf(affichageFrameDebug, "%d", LastFpsCount);
	  DrawString(affichageFrameDebug, 0, 0, 6, 'n', 0, 0, 0);
	  sprintf(affichageFrameDebug, "%d", LastTickCount);
	  DrawString(affichageFrameDebug, 100, 0, 6, 'e', 0, 0, 0);
	}else if(EtapeActuelleDuJeu == 42){
	  char affichageLevelEncours[30];
	  sprintf(affichageLevelEncours, "Niveau %d", unlockLVL);
	  DrawString(affichageLevelEncours, 0, 0, 6, 'n', 255, 255, 255);
	}
      
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

  if(RobotoFont != NULL){
    TTF_CloseFont(RobotoFont);
  }
  freeImageMalloc();
  
  if (!ok){
    strncpy(msg_formated, msg, 250);
    //    printf("%s : %s\n", msg_formated, SDL_GetError());
    fprintf(stderr, "%s : %s\n", msg_formated, SDL_GetError());
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
      end_sdl(0, "ERROR SDL VIDEOMODE INIT RESIZABLE");
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
  //printf("%d\n", key->keysym.sym);
  switch(key->keysym.sym){
  case SDLK_ESCAPE:if(EtapeActuelleDuJeu%2 == 0 && HaveType == -1){EtapeActuelleDuJeu++;}else if(EtapeActuelleDuJeu == 3 || EtapeActuelleDuJeu == 43 || EtapeActuelleDuJeu == 53 ){EtapeActuelleDuJeu--;}else if((EtapeActuelleDuJeu == 52 && HaveType != -1)){HaveType = -1;}else{EtapeActuelleDuJeu = 0;}break;
  case 8:if(EtapeActuelleDuJeu == 3 || EtapeActuelleDuJeu == 43 || EtapeActuelleDuJeu == 53 ){EtapeActuelleDuJeu = 0;}break;
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
  default:
    for(int i = 0;i < 2; i++){
      for(int j = 0; j < 6; j++){
	if(key->keysym.sym == InputVal[i][j]){
	  GetInput[i][j] = 0;
	  if(i == 0 && j == 4 && EtapeActuelleDuJeu == 52){
	    EtapeActuelleDuJeu = 55;
	    char tmp[70] = "CeciEstunFichierTempo";
	    SaveActuLVL(tmp);
	  }else if(i == 0 && j == 4 && EtapeActuelleDuJeu == 55){
	    EtapeActuelleDuJeu = 52;
	  }
	}
      }
    }
    break;
  }
}



void keyDown(SDL_KeyboardEvent *key){
  for(int i = 0;i < 2; i++){
      for(int j = 0; j < 6; j++){
	if(key->keysym.sym == InputVal[i][j]){
	  GetInput[i][j] = 1;
	}
      }
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
	  PrintSave();
	  if(otherY > TailleEcranHaut){
	    SDL_FreeSurface(renderer);
	    if((renderer = SDL_SetVideoMode(TailleEcranLong,TailleEcranHaut,32,
					    SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF )) == NULL){
	      end_sdl(0, "ERROR SDL VIDEOMODE");
	    }
	  }
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
	      keyDown(&event.key);break;
	      break; 
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
		  if((mousX < (float)TailleEcranLong/100 *75 && mousX > (float)TailleEcranLong/100 * 25) && (mousY < (float)TailleEcranHaut/100*10 + (float)TailleEcranLong/400*50 && mousY > (float)TailleEcranHaut/100 *10)){
		    EtapeActuelleDuJeu = 41;
		    Actulvl = initNextLVL();
		    camy = 5;
		    camx = 12;
		    EtapeActuelleDuJeu = 42;
		  }else if((mousX < (float)TailleEcranLong/100 *75 && mousX > (float)TailleEcranLong/100 * 25) && (mousY < (float)TailleEcranHaut/100*35 + (float)TailleEcranLong/400*50 && mousY > (float)TailleEcranHaut/100 *35)){
		    EtapeActuelleDuJeu = 41;
		    Actulvl = initVoidLVL();
		    HaveType = -1;
		    camx = 0;
		    camy = 0;
		    EtapeActuelleDuJeu = 52;
		  }
		}else if(EtapeActuelleDuJeu == 52){


		  if(mousY < TailleEcranHaut - 2 * TailleEcranHaut/nbBlockY){
		    if(HaveType != -1){
		      //printf("woaw %d %d  %f  %f\n", (int)(mousX/((float)TailleEcranHaut/nbBlockY))  + (int)(camy - 6), (int)(mousY/((float)TailleEcranHaut/nbBlockY)) + (int)(camx-12), camx+2, 2-camy);
		      if(Actulvl.TerX == 0 && Actulvl.TerY == 0){
			cellule **cel = (cellule **)malloc(sizeof(cellule *)*1);
			for(int i = 0; i < 1; i++){
			  cel[i] = (cellule *)malloc(sizeof(cellule)*1);
			  for(int j = 0; j < 1; j++){
			    cel[i][j].type = -3; cel[i][j].isPassable = 0;
			  }
			}

			for(int i = 0; i < Actulvl.TerX; i++){
			  free(Actulvl.ter.cellule[i]);
			}

			free(Actulvl.ter.cellule);

			Actulvl.ter.cellule = cel;
		      
			Actulvl.TerX = 1;
			Actulvl.TerY = 1;
			camy = 6-(int)(mousX/((float)TailleEcranHaut/nbBlockY));
			camx = 12-(int)(mousY/((float)TailleEcranHaut/nbBlockY));

			AddCase(HaveType, 0, 0);
			
		      }else if(((int)(mousX/((float)TailleEcranHaut/nbBlockY))  + (int)(camy - 6) < 0 || (int)(mousX/((float)TailleEcranHaut/nbBlockY)) + (int)(camy - 6) >= Actulvl.TerY) ||
			       ((int)(mousY/((float)TailleEcranHaut/nbBlockY)) + (int)(camx - 12) < 0 || (int)(mousY/((float)TailleEcranHaut/nbBlockY)) + (int)(camx - 12) >= Actulvl.TerX)){
			EtapeActuelleDuJeu = 41;
			int h = Actulvl.TerY;
			int w = Actulvl.TerX;

			int hb = 0;
			int wb = 0;
		      
			if(((int)(mousX/((float)TailleEcranHaut/nbBlockY))  + (int)(camy - 6) < 0 || (int)(mousX/((float)TailleEcranHaut/nbBlockY)) + (int)(camy - 6) >= Actulvl.TerY)){
			  h = abs((int)(mousX/((float)TailleEcranHaut/nbBlockY))  + (int)(camy - 6)) + 1;
			  if((int)(mousX/((float)TailleEcranHaut/nbBlockY))  + (int)(camy - 6) < 0){
			    hb -= (int)(mousX/((float)TailleEcranHaut/nbBlockY))  + (int)(camy - 6);
			    int nom = 0;
			    while(Actulvl.enti[nom].type != -5){
			      Actulvl.enti[nom].x -= (int)(mousX/((float)TailleEcranHaut/nbBlockY))  + (int)(camy - 6);
			      nom++;
			    }
			    camy -= (int)(mousX/((float)TailleEcranHaut/nbBlockY))  + (int)(camy - 6);
			    h += Actulvl.TerY-1;
			  }
			}

			if(((int)(mousY/((float)TailleEcranHaut/nbBlockY)) + (int)(camx - 12) < 0 || (int)(mousY/((float)TailleEcranHaut/nbBlockY)) + (int)(camx - 12) >= Actulvl.TerX)){
			  w = abs((int)(mousY/((float)TailleEcranHaut/nbBlockY)) + (int)(camx - 12)) + 1;
			  if((int)(mousY/((float)TailleEcranHaut/nbBlockY)) + (int)(camx - 12) < 0){
			    wb -= (int)(mousY/((float)TailleEcranHaut/nbBlockY)) + (int)(camx - 12);
			    int nom = 0;
			    while(Actulvl.enti[nom].type != -5){
			      Actulvl.enti[nom].y -= (int)(mousY/((float)TailleEcranHaut/nbBlockY)) + (int)(camx - 12);
			      nom++;
			    }
			    camx -= (int)(mousY/((float)TailleEcranHaut/nbBlockY)) + (int)(camx - 12);
			    w += Actulvl.TerX-1;
			  }
			}
		      
		      
		      
		      
			cellule **cel = (cellule **)malloc(sizeof(cellule *)*w);
			for(int i = 0; i < w; i++){
			  cel[i] = (cellule *)malloc(sizeof(cellule)*h);
			  for(int j = 0; j < h; j++){
			    cel[i][j].type = -3;cel[i][j].isPassable = 0;
			  }
			}

			//printf("%d %d\n", wb, hb);
		      
			for(int i = wb; i < wb+Actulvl.TerX; i++){
			  for(int j = hb; j < hb + Actulvl.TerY; j++){
			    cel[i][j].type = Actulvl.ter.cellule[i-wb][j-hb].type;
			    cel[i][j].isPassable = Actulvl.ter.cellule[i-wb][j-hb].isPassable;
			  }
			}
		      
		      
		      
			for(int i = 0; i < Actulvl.TerX; i++){
			  free(Actulvl.ter.cellule[i]);
			}
			free(Actulvl.ter.cellule);

			Actulvl.ter.cellule = cel;
		      
			Actulvl.TerX = w;
			Actulvl.TerY = h;


		      
			AddCase(HaveType, (int)(mousY/((float)TailleEcranHaut/nbBlockY)) + (int)(camx - 12), abs((int)(mousX/((float)TailleEcranHaut/nbBlockY))  + (int)(camy - 6)));
			
			EtapeActuelleDuJeu = 52;
		      }else{

			AddCase(HaveType, (int)(mousY/((float)TailleEcranHaut/nbBlockY)) + (int)(camx - 12), abs((int)(mousX/((float)TailleEcranHaut/nbBlockY))  + (int)(camy - 6)));
			
		      }
		    
		    }
		  }else{
		    if(mousX < 2 * TailleEcranHaut/nbBlockY){
		      HaveType = 4;
		    }else if(mousX < 4 * TailleEcranHaut/nbBlockY){
		      HaveType = 5;
		    }else if(mousX < 6 * TailleEcranHaut/nbBlockY){
		      HaveType = 6;
		    }else if(mousX < 8 * TailleEcranHaut/nbBlockY){
		      HaveType = 7;
		    }else if(mousX < 10 * TailleEcranHaut/nbBlockY){
		      HaveType = 9;
		    }
		  }






		  
		}else if(EtapeActuelleDuJeu == 3 || EtapeActuelleDuJeu == 43 || EtapeActuelleDuJeu == 53){
		    if((mousX < (float)TailleEcranLong/100 *75 -((float)TailleEcranLong/2)/64 *6  && mousX > (float)TailleEcranLong/100 * 25 + ((float)TailleEcranLong/2)/64 *38) && (mousY < (float)TailleEcranHaut/100*(50-100/6) + ((float)TailleEcranLong/4)/32 * 28 && mousY > (float)TailleEcranHaut/100 *(50-100/6) + ((float)TailleEcranLong/4)/32 * 19)){
		      EtapeActuelleDuJeu = 0;
		    }else if((mousX < (float)TailleEcranLong/100 *75 -((float)TailleEcranLong/2)/64 *38  && mousX > (float)TailleEcranLong/100 * 25 + ((float)TailleEcranLong/2)/64 *6) && (mousY < (float)TailleEcranHaut/100*(50-100/6) + ((float)TailleEcranLong/4)/32 * 28 && mousY > (float)TailleEcranHaut/100 *(50-100/6) + ((float)TailleEcranLong/4)/32 * 19)){
		      EtapeActuelleDuJeu--;
		    }
		}
	      } else if (event.button.button == SDL_BUTTON_RIGHT) {
		if(EtapeActuelleDuJeu == 52){
		  HaveType = -1;
		}
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

      if(EtapeActuelleDuJeu == 42 || EtapeActuelleDuJeu == 55){
	Physique1play();
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



void AddCase(int type, int x, int y){

  if(HaveType >= 7){
			
    if(Actulvl.ter.cellule[x][y].isPassable == 0){
      Actulvl.ter.cellule[x][y].type = 0;
      Actulvl.ter.cellule[x][y].isPassable = 1;
    }
    //    printf("%d\n", type);
    if(type == 9){
      addEntite(&Actulvl, 1, x, y);
    }else if(type == 7) {
      if(Actulvl.ter.cellule[x][y].type == 1){
	addEntite(&Actulvl, 6, x, y);
      }else{
	addEntite(&Actulvl, 5, x, y);
      }
    }
    
    
			
			  
  }else{
    switch(type){
    case 4 : Actulvl.ter.cellule[x][y].type = 0; Actulvl.ter.cellule[x][y].isPassable = 1;break;
    case 5 : Actulvl.ter.cellule[x][y].type = 1; Actulvl.ter.cellule[x][y].isPassable = 1;break;
    case 6 : Actulvl.ter.cellule[x][y].type = 5; Actulvl.ter.cellule[x][y].isPassable = 0;break;
    }
    
    
  }
  
  
  
}
