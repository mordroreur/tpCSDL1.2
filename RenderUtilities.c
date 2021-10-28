#include "RenderUtilities.h"

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


long int getTime(){
  struct timespec tms;
  if (clock_gettime(CLOCK_REALTIME,&tms)) {
        return -1;
    }
  return ((tms.tv_sec*1000000) + (tms.tv_nsec/1000));
}

/*
void DrawString(char *s, int x, int y, int size, char center, int R, int G, int B){
  int n = 0;
  while(s[n] != '\0'){
    n++;
  }

  SDL_Color Color = {R, G, B};
  SDL_Surface* surfaceMessage = TTF_RenderText_Solid(RobotoFont, s, Color); 
  SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
  SDL_Rect Message_rect;
  switch (center) {
  case 'n':
    Message_rect.x = (int)((float)(TailleEcranLong)/100 * x); 
    Message_rect.y = (int)((float)(TailleEcranHaut)/100 * y);
    Message_rect.w = (int)((float)(TailleEcranLong)/400 * size * n);
    Message_rect.h = (int)((float)(TailleEcranHaut)/100 * size);
    break;
  case 'e':
    Message_rect.x = (int)((float)(TailleEcranLong)/100 * x - (float)(TailleEcranLong)/400 * size * n); 
    Message_rect.y = (int)((float)(TailleEcranHaut)/100 * y);
    Message_rect.w = (int)((float)(TailleEcranLong)/400 * size * n);
    Message_rect.h = (int)((float)(TailleEcranHaut)/100 * size);
    break;
  default:
    Message_rect.x = (int)((float)(TailleEcranLong)/100 * x); 
    Message_rect.y = (int)((float)(TailleEcranHaut)/100 * y);
    Message_rect.w = (int)((float)(TailleEcranLong)/400 * size * n);
    Message_rect.h = (int)((float)(TailleEcranHaut)/100 * size);
    break;
  }

  SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
  SDL_FreeSurface(surfaceMessage);
  SDL_DestroyTexture(Message);


  
}


void DrawImage(int imagenb, int x, int y, int size, char center){




  
}
*/
