#include "Jeu.h"
#include "RenderUtilities.h"
#include <stdio.h>

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

extern int GetInput[2][6]; /* Valeur de si le joueur appuie */

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

void DestroyLVL(){
  free(Actulvl.enti);
  Actulvl.maxEnti = 0;
  for(int i = 0; i < Actulvl.TerX; i++){
    free(Actulvl.ter.cellule[i]);
  }
  free(Actulvl.ter.cellule);
  Actulvl.TerX = 0;
  Actulvl.TerY = 0;
}
level initVoidLVL(){
  level lev;
  int longueur = 0;
  int largeur = 0; 
  lev.enti = (entite *)malloc(sizeof(entite)*50);
  lev.maxEnti = 50;
  for(int i = 0; i < 50; i++){
    lev.enti[i].type = -5;
  }
  lev.TerX = longueur;
  lev.TerY = largeur;
  lev.ter.cellule = (cellule **)malloc(sizeof(cellule *)*longueur);
  for(int i = 0; i < longueur; i++){
    lev.ter.cellule[i] = (cellule *)malloc(sizeof(cellule)*largeur);
    for(int j = 0; j < largeur; j++){
      lev.ter.cellule[i][j].type = 0;
    }
  }
  
  return lev;
  
}


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
  lev.TerX = longueur;
  lev.TerY = largeur;
  lev.ter.cellule = (cellule **)malloc(sizeof(cellule *)*longueur);
  for(int i = 0; i < longueur; i++){
    lev.ter.cellule[i] = (cellule *)malloc(sizeof(cellule)*largeur);
    for(int j = 0; j < largeur; j++){
      lev.ter.cellule[i][j].type = -3;lev.ter.cellule[i][j].isPassable = 0;
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
      case ' ':lev.ter.cellule[longueur][largeur].type = 0;lev.ter.cellule[longueur][largeur].isPassable = 1;break;
      case '#':lev.ter.cellule[longueur][largeur].type = 5;lev.ter.cellule[longueur][largeur].isPassable = 0;break;
      case '$':lev.ter.cellule[longueur][largeur].type = 0;addEntite(&lev, 5, longueur, largeur);lev.ter.cellule[longueur][largeur].isPassable = 1;break;
      case '.':lev.ter.cellule[longueur][largeur].type = 1;lev.ter.cellule[longueur][largeur].isPassable = 1;break;
      case '@':lev.ter.cellule[longueur][largeur].type = 0;addEntite(&lev, 1, longueur, largeur);lev.ter.cellule[longueur][largeur].isPassable = 1;break;
      case '=':lev.ter.cellule[longueur][largeur].type = 1;addEntite(&lev, 6, longueur, largeur);lev.ter.cellule[longueur][largeur].isPassable = 1;break;
      case 'A':lev.ter.cellule[longueur][largeur].isPassable = 0;break;
      default:printf("FUCK %c \n", r);break;
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
    lev->enti[nb].xHitbox = 0.60;
    lev->enti[nb].yHitbox = 0.75;
    lev->enti[nb].sizeX = 1;
    lev->enti[nb].sizeY = 1;
    lev->enti[nb].xHitboxOffset = 0.1875;
    lev->enti[nb].yHitboxOffset = 0.125;
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



void Physique1play(){
  int actuGest = 0;

  //Si touche recommence niveau n'est pas appuye
  if(!GetInput[0][4]){
 
    //pour toute les entites du tableau
    while(Actulvl.enti[actuGest].type != -5){

      //si c'est le joueur
      if(Actulvl.enti[actuGest].type == 1){

	//recupreation des inputs
	if(GetInput[0][0] == 1){
	  if(Actulvl.enti[actuGest].ySpeed == 0){
	    Actulvl.enti[actuGest].ySpeed = -1;
	  }
	}else if(GetInput[0][2] == 1){
	  if(Actulvl.enti[actuGest].ySpeed == 0){
	    Actulvl.enti[actuGest].ySpeed = 1;
	  }
	}
	if(GetInput[0][3] == 1){
	  if(Actulvl.enti[actuGest].xSpeed == 0){
	    Actulvl.enti[actuGest].xSpeed = -1;
	  }
	}else if(GetInput[0][1] == 1){
	  if(Actulvl.enti[actuGest].xSpeed == 0){
	    Actulvl.enti[actuGest].xSpeed = 1;
	  }
	}
      }


      //gestion des 4deplacement.
      if(Actulvl.enti[actuGest].xSpeed < 0){
	AvanceGauche(actuGest);
      }else if (Actulvl.enti[actuGest].xSpeed > 0){
	AvanceDroite(actuGest);
      }
      if(Actulvl.enti[actuGest].ySpeed < 0){
	AvanceHaut(actuGest);
      }else if (Actulvl.enti[actuGest].ySpeed > 0){
	AvanceBas(actuGest);
      }

    

      actuGest++;
    }
  }else{
    DestroyLVL();
    Actulvl = initNextLVL();
  }  
}



void VerifWin(){
  int nb = 0;
  int win = 1;
  while(nb < Actulvl.maxEnti && Actulvl.enti[nb].type != -5){
    if(Actulvl.enti[nb].type == 5){
      win = 0;
    }
    nb++;
  }
  if(win){
    EtapeActuelleDuJeu = 41;
    DestroyLVL();
    if(unlockLVL < 6){
      unlockLVL++;
      PrintSave();
      Actulvl = initNextLVL();
      EtapeActuelleDuJeu = 42;
    }else{
      EtapeActuelleDuJeu = 2;
    }
  }
}




// La fonction fais avancer l'entite actuGest vers la gauche
void AvanceGauche(int actuGest){
  int avance = 1;
  int verif = 0;
  
  // Si il n'y a pas de mur a gauche
  if(Actulvl.ter.cellule[(int)(Actulvl.enti[actuGest].y+Actulvl.enti[actuGest].yHitboxOffset + 0.001)][(int)(Actulvl.enti[actuGest].x+Actulvl.enti[actuGest].xHitboxOffset)].isPassable && Actulvl.ter.cellule[(int)(Actulvl.enti[actuGest].y+Actulvl.enti[actuGest].yHitboxOffset + Actulvl.enti[actuGest].yHitbox - 0.001)][(int)(Actulvl.enti[actuGest].x +Actulvl.enti[actuGest].xHitboxOffset)].isPassable){
    //pour toute les autres entites
    while(Actulvl.enti[verif].type != -5){
      //si elles ont un y similaire
      if(verif != actuGest && Actulvl.enti[verif].y+Actulvl.enti[verif].yHitboxOffset < Actulvl.enti[actuGest].y+Actulvl.enti[actuGest].yHitbox+Actulvl.enti[actuGest].yHitboxOffset - 0.05 && Actulvl.enti[verif].y+Actulvl.enti[verif].yHitbox+Actulvl.enti[verif].yHitboxOffset > Actulvl.enti[actuGest].y+Actulvl.enti[actuGest].yHitboxOffset + 0.05){
	//si elles ont le meme x 
	if(Actulvl.enti[verif].x + Actulvl.enti[verif].xHitboxOffset < Actulvl.enti[actuGest].x+Actulvl.enti[actuGest].xHitboxOffset-0.001 && Actulvl.enti[verif].x+Actulvl.enti[verif].xHitboxOffset + Actulvl.enti[verif].xHitbox > Actulvl.enti[actuGest].x + Actulvl.enti[actuGest].xHitboxOffset - 0.001){
	  avance = 0;

	  // si tu peu pousser l'entite
	  if(Actulvl.ter.cellule[(int)(Actulvl.enti[verif].y+Actulvl.enti[verif].yHitboxOffset+0.001)][(int)(Actulvl.enti[verif].x-Actulvl.enti[verif].sizeX)].isPassable && Actulvl.ter.cellule[(int)(Actulvl.enti[verif].y+Actulvl.enti[verif].yHitbox+Actulvl.enti[verif].yHitboxOffset-0.001)][(int)(Actulvl.enti[verif].x-Actulvl.enti[verif].sizeX)].isPassable && Actulvl.enti[actuGest].type == 1){
	    Actulvl.enti[verif].xSpeed = Actulvl.enti[actuGest].xSpeed;
	    AvanceGauche(verif);
	    if(Actulvl.enti[verif].xSpeed == 0){
	      Actulvl.enti[actuGest].xSpeed = -Actulvl.enti[actuGest].xSpeed;
	    }
	  }else{
	    if(Actulvl.enti[actuGest].type == 1){
	      Actulvl.enti[actuGest].xSpeed = -Actulvl.enti[actuGest].xSpeed;
	    }else{
	      Actulvl.enti[actuGest].xSpeed = 0;
	    }
	  }
	      
	}
      }
      verif++;
    }
    // si aucune entite t'empeche d'avancer
    if(avance){
      if(Actulvl.enti[actuGest].type == 1){//cas joueur
	Actulvl.enti[actuGest].x += Actulvl.enti[actuGest].xSpeed/60;
	if((int)(Actulvl.enti[actuGest].x + Actulvl.enti[actuGest].xSpeed/60) != (int)(Actulvl.enti[actuGest].x) || Actulvl.enti[actuGest].x < 0){
	  Actulvl.enti[actuGest].x = (int)(Actulvl.enti[actuGest].x);
	  Actulvl.enti[actuGest].xSpeed = 0;
	}
      }else if(Actulvl.enti[actuGest].type == 5){// cas boite
	Actulvl.enti[actuGest].x += Actulvl.enti[actuGest].xSpeed/60;
	if((int)(Actulvl.enti[actuGest].x + Actulvl.enti[actuGest].xSpeed/60) != (int)(Actulvl.enti[actuGest].x) || Actulvl.enti[actuGest].x < 0){
	  Actulvl.enti[actuGest].x = (int)(Actulvl.enti[actuGest].x);
	  Actulvl.enti[actuGest].xSpeed = 0;
	  if(Actulvl.ter.cellule[(int)(Actulvl.enti[actuGest].y)][(int)(Actulvl.enti[actuGest].x)].type == 1){
	    Actulvl.enti[actuGest].type = 6;
	    VerifWin();
	  }
	}
      }else if(Actulvl.enti[actuGest].type == 6){ // cas boite lock
	Actulvl.enti[actuGest].x += Actulvl.enti[actuGest].xSpeed/60;
	Actulvl.enti[actuGest].type = 5;
      }
    }
  }else{
    Actulvl.enti[actuGest].xSpeed = -Actulvl.enti[actuGest].xSpeed;
  }
}


// La fonction fais avancer l'entite actuGest vers la droite
void AvanceDroite(int actuGest){
  int avance = 1;
  int verif = 0;
  // Si il n'y a pas de mur a droite
  if(Actulvl.enti[actuGest].x+Actulvl.enti[actuGest].xHitboxOffset+Actulvl.enti[actuGest].xHitbox < Actulvl.TerY && Actulvl.ter.cellule[(int)(Actulvl.enti[actuGest].y+Actulvl.enti[actuGest].yHitboxOffset + 0.001)][(int)(Actulvl.enti[actuGest].x + Actulvl.enti[actuGest].xHitbox + Actulvl.enti[actuGest].xHitboxOffset)].isPassable && Actulvl.ter.cellule[(int)(Actulvl.enti[actuGest].y+Actulvl.enti[actuGest].yHitbox+ Actulvl.enti[actuGest].yHitboxOffset - 0.001)][(int)(Actulvl.enti[actuGest].x + Actulvl.enti[actuGest].xHitbox + Actulvl.enti[actuGest].xHitboxOffset)].isPassable){
    //pour toute les autres entites
    while(Actulvl.enti[verif].type != -5){
      //si elles ont un y similaire
      if(verif != actuGest && Actulvl.enti[verif].y+Actulvl.enti[verif].yHitboxOffset < Actulvl.enti[actuGest].y+Actulvl.enti[actuGest].yHitboxOffset +Actulvl.enti[actuGest].yHitbox - 0.05 && Actulvl.enti[verif].y+Actulvl.enti[verif].yHitboxOffset + Actulvl.enti[verif].yHitbox > Actulvl.enti[actuGest].y+Actulvl.enti[actuGest].yHitboxOffset + 0.05){
	//si elles ont le meme x 
	if(Actulvl.enti[verif].x + Actulvl.enti[verif].xHitboxOffset < Actulvl.enti[actuGest].x + Actulvl.enti[actuGest].xHitboxOffset + Actulvl.enti[actuGest].xHitbox + 0.01 && Actulvl.enti[verif].x+Actulvl.enti[verif].xHitboxOffset + Actulvl.enti[verif].xHitbox  > Actulvl.enti[actuGest].x + Actulvl.enti[actuGest].xHitboxOffset + Actulvl.enti[actuGest].xHitbox + 0.01){
	  avance = 0;
	  // si tu peu pousser l'entite
	  if(Actulvl.ter.cellule[(int)(Actulvl.enti[verif].y+Actulvl.enti[verif].yHitboxOffset + 0.001)][(int)(Actulvl.enti[verif].x+Actulvl.enti[verif].sizeX)].isPassable && Actulvl.ter.cellule[(int)(Actulvl.enti[verif].y+Actulvl.enti[verif].yHitboxOffset + Actulvl.enti[verif].yHitbox - 0.001)][(int)(Actulvl.enti[verif].x+Actulvl.enti[actuGest].sizeX)].isPassable && Actulvl.enti[actuGest].type == 1){
	    Actulvl.enti[verif].xSpeed = Actulvl.enti[actuGest].xSpeed;
	    AvanceDroite(verif);
	    if(Actulvl.enti[verif].xSpeed == 0){
	      Actulvl.enti[actuGest].xSpeed = -Actulvl.enti[actuGest].xSpeed;
	    }
	  }else{
	    if(Actulvl.enti[actuGest].type == 1){
	      Actulvl.enti[actuGest].xSpeed = -Actulvl.enti[actuGest].xSpeed;
	    }else{
	      Actulvl.enti[actuGest].xSpeed = 0;
	    }
	  }
	      
	}
      }
      verif++;
    }
    // si aucune entite t'empeche d'avancer
    if(avance){
      if(Actulvl.enti[actuGest].type == 1){//cas joueur
	Actulvl.enti[actuGest].x += Actulvl.enti[actuGest].xSpeed/60;
	if((int)(Actulvl.enti[actuGest].x + Actulvl.enti[actuGest].xSpeed/60) != (int)(Actulvl.enti[actuGest].x)){
	  Actulvl.enti[actuGest].x = (int)(Actulvl.enti[actuGest].x + Actulvl.enti[actuGest].xSpeed);
	  Actulvl.enti[actuGest].xSpeed = 0;
	}
      }else if(Actulvl.enti[actuGest].type == 5){// cas boite
	Actulvl.enti[actuGest].x += Actulvl.enti[actuGest].xSpeed/60;
	if((int)(Actulvl.enti[actuGest].x + Actulvl.enti[actuGest].xSpeed/60) != (int)(Actulvl.enti[actuGest].x)){
	  Actulvl.enti[actuGest].x = (int)(Actulvl.enti[actuGest].x + Actulvl.enti[actuGest].xSpeed);
	  Actulvl.enti[actuGest].xSpeed = 0;
	  if(Actulvl.ter.cellule[(int)(Actulvl.enti[actuGest].y)][(int)(Actulvl.enti[actuGest].x)].type == 1){
	    Actulvl.enti[actuGest].type = 6;
	    VerifWin();
	  }
	}
      }else if(Actulvl.enti[actuGest].type == 6){ // cas boite lock
	Actulvl.enti[actuGest].x += Actulvl.enti[actuGest].xSpeed/60;
	Actulvl.enti[actuGest].type = 5;
      }
    }
  }else{
    Actulvl.enti[actuGest].xSpeed = -Actulvl.enti[actuGest].xSpeed;
  }
}

void AvanceHaut(int actuGest){
  int avance = 1;
  int verif = 0;
  // Si il n'y a pas de mur au dessus
  if(Actulvl.ter.cellule[(int)(Actulvl.enti[actuGest].y + Actulvl.enti[actuGest].yHitboxOffset)][(int)(Actulvl.enti[actuGest].x + Actulvl.enti[actuGest].xHitboxOffset + 0.001)].isPassable && Actulvl.ter.cellule[(int)(Actulvl.enti[actuGest].y + Actulvl.enti[actuGest].yHitboxOffset)][(int)(Actulvl.enti[actuGest].x + Actulvl.enti[actuGest].xHitboxOffset + Actulvl.enti[actuGest].xHitbox - 0.001)].isPassable){
    //pour toute les autres entites
    while(Actulvl.enti[verif].type != -5){
      //si elles ont un x similaire
      if(verif != actuGest && Actulvl.enti[verif].x + Actulvl.enti[verif].xHitboxOffset < Actulvl.enti[actuGest].x+Actulvl.enti[actuGest].xHitboxOffset + Actulvl.enti[actuGest].xHitbox - 0.05 && Actulvl.enti[verif].x+Actulvl.enti[verif].xHitbox + Actulvl.enti[verif].xHitboxOffset > Actulvl.enti[actuGest].x+Actulvl.enti[actuGest].xHitboxOffset + 0.05){
	//si elles ont le meme y
	if(Actulvl.enti[verif].y < Actulvl.enti[actuGest].y + Actulvl.enti[actuGest].yHitboxOffset - 0.001 && Actulvl.enti[verif].y+Actulvl.enti[verif].yHitboxOffset + Actulvl.enti[verif].yHitbox > Actulvl.enti[actuGest].y + Actulvl.enti[actuGest].yHitboxOffset - 0.001){
	  avance = 0;

	  // si tu peu pousser l'entite
	  if(Actulvl.ter.cellule[(int)(Actulvl.enti[verif].y-Actulvl.enti[verif].sizeY)][(int)(Actulvl.enti[verif].x+Actulvl.enti[verif].xHitboxOffset+0.001)].isPassable && Actulvl.ter.cellule[(int)(Actulvl.enti[verif].y-Actulvl.enti[verif].sizeY)][(int)(Actulvl.enti[verif].x+Actulvl.enti[verif].xHitboxOffset + Actulvl.enti[verif].xHitbox-0.001)].isPassable && Actulvl.enti[actuGest].type == 1){
	    Actulvl.enti[verif].ySpeed = Actulvl.enti[actuGest].ySpeed;
	    AvanceHaut(verif);
	    if(Actulvl.enti[verif].ySpeed == 0){
	      Actulvl.enti[actuGest].ySpeed = -Actulvl.enti[actuGest].ySpeed;
	    }
	  }else{
	    if(Actulvl.enti[actuGest].type == 1){
	      Actulvl.enti[actuGest].ySpeed = -Actulvl.enti[actuGest].ySpeed;
	    }else{
	      Actulvl.enti[actuGest].ySpeed = 0;
	    }
	  }
	      
	}
      }
      verif++;
      }
    // si aucune entite t'empeche d'avancer
    if(avance){
      if(Actulvl.enti[actuGest].type == 1){//cas joueur
	Actulvl.enti[actuGest].y += Actulvl.enti[actuGest].ySpeed/60;
	if((int)(Actulvl.enti[actuGest].y + Actulvl.enti[actuGest].ySpeed/60) != (int)(Actulvl.enti[actuGest].y) || Actulvl.enti[actuGest].y < 0){
	  Actulvl.enti[actuGest].y = (int)(Actulvl.enti[actuGest].y);
	  Actulvl.enti[actuGest].ySpeed = 0;
	}
      }else if(Actulvl.enti[actuGest].type == 5){// cas boite
	Actulvl.enti[actuGest].y += Actulvl.enti[actuGest].ySpeed/60;
	if((int)(Actulvl.enti[actuGest].y + Actulvl.enti[actuGest].ySpeed/60) != (int)(Actulvl.enti[actuGest].y) || Actulvl.enti[actuGest].y < 0){
	  Actulvl.enti[actuGest].y = (int)(Actulvl.enti[actuGest].y);
	  Actulvl.enti[actuGest].ySpeed = 0;
	  if(Actulvl.ter.cellule[(int)(Actulvl.enti[actuGest].y)][(int)(Actulvl.enti[actuGest].x)].type == 1){
	    Actulvl.enti[actuGest].type = 6;
	    VerifWin();
	  }
	}
      }else if(Actulvl.enti[actuGest].type == 6){ // cas boite lock
	Actulvl.enti[actuGest].y += Actulvl.enti[actuGest].ySpeed/60;
	Actulvl.enti[actuGest].type = 5;
      }
    }
  }else{
    Actulvl.enti[actuGest].ySpeed = -Actulvl.enti[actuGest].ySpeed;
  }
}


void AvanceBas(int actuGest){
  int avance = 1;
  int verif = 0;
  // Si il n'y a pas de mur a droite
  if(Actulvl.enti[actuGest].y+Actulvl.enti[actuGest].yHitboxOffset+Actulvl.enti[actuGest].yHitbox < Actulvl.TerX && Actulvl.ter.cellule[(int)(Actulvl.enti[actuGest].y+Actulvl.enti[actuGest].yHitboxOffset+Actulvl.enti[actuGest].yHitbox)][(int)(Actulvl.enti[actuGest].x + Actulvl.enti[actuGest].xHitboxOffset + 0.001)].isPassable && Actulvl.ter.cellule[(int)(Actulvl.enti[actuGest].y+Actulvl.enti[actuGest].yHitboxOffset + Actulvl.enti[actuGest].yHitbox)][(int)(Actulvl.enti[actuGest].x + Actulvl.enti[actuGest].xHitboxOffset + Actulvl.enti[actuGest].xHitbox - 0.001)].isPassable){
    //pour toute les autres entites
    while(Actulvl.enti[verif].type != -5){
      //si elles ont un x similaire
      if(verif != actuGest && Actulvl.enti[verif].x+Actulvl.enti[verif].xHitboxOffset < Actulvl.enti[actuGest].x+Actulvl.enti[actuGest].xHitboxOffset + Actulvl.enti[actuGest].xHitbox - 0.05 && Actulvl.enti[verif].x+Actulvl.enti[verif].xHitboxOffset + Actulvl.enti[verif].xHitbox > Actulvl.enti[actuGest].x+Actulvl.enti[actuGest].xHitboxOffset + 0.05){
	//si elles ont le meme y
	
	if(Actulvl.enti[verif].y + Actulvl.enti[verif].yHitboxOffset < Actulvl.enti[actuGest].y + Actulvl.enti[actuGest].yHitboxOffset + Actulvl.enti[actuGest].yHitbox + 0.001 && Actulvl.enti[verif].y+Actulvl.enti[verif].yHitboxOffset + Actulvl.enti[verif].yHitbox > Actulvl.enti[actuGest].y + Actulvl.enti[actuGest].yHitboxOffset + Actulvl.enti[actuGest].yHitbox + 0.001){
	  avance = 0;

	  // si tu peu pousser l'entite
	  if(Actulvl.ter.cellule[(int)(Actulvl.enti[verif].y+Actulvl.enti[verif].sizeY)][(int)(Actulvl.enti[verif].x+Actulvl.enti[verif].xHitboxOffset+0.001)].isPassable && Actulvl.ter.cellule[(int)(Actulvl.enti[verif].y+Actulvl.enti[verif].sizeY)][(int)(Actulvl.enti[verif].x+Actulvl.enti[verif].xHitboxOffset+Actulvl.enti[verif].xHitbox-0.001)].isPassable && Actulvl.enti[actuGest].type == 1){
	    Actulvl.enti[verif].ySpeed = Actulvl.enti[actuGest].ySpeed;
	    AvanceBas(verif);
	    if(Actulvl.enti[verif].ySpeed == 0){
	      Actulvl.enti[actuGest].ySpeed = -Actulvl.enti[actuGest].ySpeed;
	    }
	  }else{
	    if(Actulvl.enti[actuGest].type == 1){
	      Actulvl.enti[actuGest].ySpeed = -Actulvl.enti[actuGest].ySpeed;
	    }else{
	      Actulvl.enti[actuGest].ySpeed = 0;
	    }
	  }   
	}
      }
      verif++;
    }
    // si aucune entite t'empeche d'avancer
    if(avance){
      if(Actulvl.enti[actuGest].type == 1){//cas joueur
	Actulvl.enti[actuGest].y += Actulvl.enti[actuGest].ySpeed/60;
	if((int)(Actulvl.enti[actuGest].y + Actulvl.enti[actuGest].ySpeed/60) != (int)(Actulvl.enti[actuGest].y)){
	  Actulvl.enti[actuGest].y = (int)(Actulvl.enti[actuGest].y + Actulvl.enti[actuGest].ySpeed);
	  Actulvl.enti[actuGest].ySpeed = 0;
	}
      }else if(Actulvl.enti[actuGest].type == 5){// cas boite
	Actulvl.enti[actuGest].y += Actulvl.enti[actuGest].ySpeed/60;
	if((int)(Actulvl.enti[actuGest].y + Actulvl.enti[actuGest].ySpeed/60) != (int)(Actulvl.enti[actuGest].y)){
	  Actulvl.enti[actuGest].y = (int)(Actulvl.enti[actuGest].y + Actulvl.enti[actuGest].ySpeed);
	  Actulvl.enti[actuGest].ySpeed = 0;
	  if(Actulvl.ter.cellule[(int)(Actulvl.enti[actuGest].y)][(int)(Actulvl.enti[actuGest].x)].type == 1){
	    Actulvl.enti[actuGest].type = 6;
	    VerifWin();
	  }
	}
      }else if(Actulvl.enti[actuGest].type == 6){ // cas boite lock
	Actulvl.enti[actuGest].y += Actulvl.enti[actuGest].ySpeed/60;
	Actulvl.enti[actuGest].type = 5;
      }
    }
  }else{
    Actulvl.enti[actuGest].ySpeed = -Actulvl.enti[actuGest].ySpeed;
  }
}



