#include "Jeu.h"

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////      Recuperation des variables globale     ///////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////



extern int EtapeActuelleDuJeu; /* 0 = fin; 1 = Loading Screen... */

extern int DEBUG; /* 1 : affiche le debug */

extern int unlockLVL; /* Dernier niveau debloque */

extern int fullscreen; /* Si le niveau est en plein ecran au demarage */

extern level Actulvl; /* Niveau actuelle sur lequel on joue */

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////


level initNextLVL(){
  level lev;
  FILE *niv;
  char s[100];
  char r;
  int longueur = 0;
  int largeur = 0;
  int tempoLarg = 0;
  
  sprintf(s, "Res/Level/Niveau%d", unlockLVL);
  niv = fopen(s, "r");
  if(niv == NULL){
    printf("probleme avec : %s\n", s);
    exit(0);
  }
  while((r = getc(niv)) != EOF){
    if(r == '\n'){
      longueur++;
      if(tempoLarg > largeur){
	largeur = tempoLarg;
      }
      tempoLarg = 0;
    }else{
      tempoLarg++;
    }
  }
  fclose(niv);
  if(tempoLarg != 0){
    longueur++;
    if(tempoLarg > largeur){
      largeur = tempoLarg;
    }
  }
  lev.enti = (entite *)malloc(sizeof(entite)*50);
  lev.maxEnti = 0;
  lev.TerX = longueur;
  lev.TerY = largeur;
  lev.ter.cellule = (cellule **)malloc(sizeof(cellule *)*longueur);
  for(int i = 0; i < longueur; i++){
    lev.ter.cellule[i] = (cellule *)malloc(sizeof(cellule)*largeur);
    for(int j = 0; j < largeur; j++){
      lev.ter.cellule[i][j].type = -3;
    }
  }

  lev.maxEnti = 50;
  lev.enti = (entite *)malloc(50*sizeof(entite));
  for(int i = 0; i < 50; i++){
    lev.enti[i].type = -5;
  }

  
  largeur = 0;
  longueur = 0;
  niv = fopen(s, "r");
  while ((r = getc(niv)) != EOF){
    if(r != '\n'){
      switch (r) {
      case ' ':lev.ter.cellule[longueur][largeur].type = 0;break;
      case '#':lev.ter.cellule[longueur][largeur].type = 5;break;
      case '$':lev.ter.cellule[longueur][largeur].type = 0;addEntite(&lev, 5, longueur, largeur);break;
      case '.':lev.ter.cellule[longueur][largeur].type = 1;break;
      case '@':lev.ter.cellule[longueur][largeur].type = 0;addEntite(&lev, 1, longueur, largeur);break;
      case '=':lev.ter.cellule[longueur][largeur].type = 1;addEntite(&lev, 6, longueur, largeur);break;
      default:printf("FUCK");break;
      }
      largeur++;
    }else{
      largeur = 0;
      longueur++;
    } 
  }
  fclose(niv);
  return lev;
}


void addEntite(level *lev, int type, float longu, float larg){
  int nb = 0;
  while(nb < lev->maxEnti && lev->enti[nb].type != -5){
    nb++;
  }
  if(nb == lev->maxEnti){
    lev->maxEnti+=50;
    lev->enti =  realloc(lev->enti, lev->maxEnti*sizeof(entite));
  }


  if(type == 1){
    lev->enti[nb].x = larg;
    lev->enti[nb].y = longu;
    lev->enti[nb].ySpeed = 0;
    lev->enti[nb].xSpeed = 0;
    lev->enti[nb].xHitbox = 1;
    lev->enti[nb].yHitbox = 1;
    lev->enti[nb].sizeX = 1;
    lev->enti[nb].sizeY = 1;
    lev->enti[nb].xHitboxOffset = 0;
    lev->enti[nb].yHitboxOffset = 0;
    lev->enti[nb].type = 1;
    lev->enti[nb].direction = 0;
  } if(type == 5){
    lev->enti[nb].x = larg;
    lev->enti[nb].y = longu;
    lev->enti[nb].ySpeed = 0;
    lev->enti[nb].xSpeed = 0;
    lev->enti[nb].xHitbox = 1;
    lev->enti[nb].yHitbox = 1;
    lev->enti[nb].sizeX = 1;
    lev->enti[nb].sizeY = 1;
    lev->enti[nb].xHitboxOffset = 0;
    lev->enti[nb].yHitboxOffset = 0;
    lev->enti[nb].type = 5;
    lev->enti[nb].direction = 0;
  }else if(type == 6){
    lev->enti[nb].x = larg;
    lev->enti[nb].y = longu;
    lev->enti[nb].ySpeed = 0;
    lev->enti[nb].xSpeed = 0;
    lev->enti[nb].xHitbox = 1;
    lev->enti[nb].yHitbox = 1;
    lev->enti[nb].sizeX = 1;
    lev->enti[nb].sizeY = 1;
    lev->enti[nb].xHitboxOffset = 0;
    lev->enti[nb].yHitboxOffset = 0;
    lev->enti[nb].type = 6;
    lev->enti[nb].direction = 0;
  }
  
}
