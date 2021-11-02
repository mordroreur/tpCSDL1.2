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

extern int GetInput[2][6]; /* Valeur de si le joueur appuie */

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
      case ' ':lev.ter.cellule[longueur][largeur].type = 0;lev.ter.cellule[longueur][largeur].isPassable = 1;break;
      case '#':lev.ter.cellule[longueur][largeur].type = 5;lev.ter.cellule[longueur][largeur].isPassable = 0;break;
      case '$':lev.ter.cellule[longueur][largeur].type = 0;addEntite(&lev, 5, longueur, largeur);lev.ter.cellule[longueur][largeur].isPassable = 1;break;
      case '.':lev.ter.cellule[longueur][largeur].type = 1;lev.ter.cellule[longueur][largeur].isPassable = 1;break;
      case '@':lev.ter.cellule[longueur][largeur].type = 0;addEntite(&lev, 1, longueur, largeur);lev.ter.cellule[longueur][largeur].isPassable = 1;break;
      case '=':lev.ter.cellule[longueur][largeur].type = 1;addEntite(&lev, 6, longueur, largeur);lev.ter.cellule[longueur][largeur].isPassable = 1;break;
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



void Physique1play(){
  int playernb = 0;
  while(Actulvl.enti[playernb].type != 1){playernb++;}

  if(GetInput[0][0] == 1){
    Actulvl.enti[playernb].ySpeed = -1;
  }else if(GetInput[0][2] == 1){
    Actulvl.enti[playernb].ySpeed = 1;
  }else{
    Actulvl.enti[playernb].ySpeed = 0;
  }

  if(GetInput[0][3] == 1){
    Actulvl.enti[playernb].xSpeed = -1;
  }else if(GetInput[0][1] == 1){
    Actulvl.enti[playernb].xSpeed = 1;
  }else{
    Actulvl.enti[playernb].xSpeed = 0;
  }

  int actuGest = 0;
  while(Actulvl.enti[actuGest].type != -5){



    if(Actulvl.enti[actuGest].xSpeed < 0){
      AvanceGauche(actuGest);
      



      
    }else if (Actulvl.enti[actuGest].xSpeed > 0){
      if(Actulvl.ter.cellule[(int)(Actulvl.enti[actuGest].y+Actulvl.enti[actuGest].sizeY/10)][(int)(Actulvl.enti[playernb].x + Actulvl.enti[playernb].sizeX)].isPassable && Actulvl.ter.cellule[(int)(Actulvl.enti[actuGest].y+Actulvl.enti[actuGest].sizeY/10 *9)][(int)(Actulvl.enti[playernb].x + Actulvl.enti[playernb].sizeX)].isPassable){
	Actulvl.enti[actuGest].x += Actulvl.enti[actuGest].xSpeed/60;
      }
    }



    //    printf("okay %f \n", Actulvl.enti[actuGest].ySpeed);

    if(Actulvl.enti[actuGest].ySpeed < 0){
      if(Actulvl.ter.cellule[(int)(Actulvl.enti[actuGest].y)][(int)(Actulvl.enti[playernb].x + Actulvl.enti[actuGest].sizeX/10)].isPassable && Actulvl.ter.cellule[(int)(Actulvl.enti[actuGest].y)][(int)(Actulvl.enti[playernb].x + Actulvl.enti[actuGest].sizeY/10*9)].isPassable){
	Actulvl.enti[actuGest].y += Actulvl.enti[actuGest].ySpeed/60;
      }
    }else if (Actulvl.enti[actuGest].ySpeed > 0){
      if(Actulvl.ter.cellule[(int)(Actulvl.enti[actuGest].y + Actulvl.enti[actuGest].sizeY)][(int)(Actulvl.enti[playernb].x + Actulvl.enti[actuGest].sizeX/10)].isPassable && Actulvl.ter.cellule[(int)(Actulvl.enti[actuGest].y + Actulvl.enti[actuGest].sizeY)][(int)(Actulvl.enti[playernb].x + Actulvl.enti[actuGest].sizeY/10*9)].isPassable){
	Actulvl.enti[actuGest].y += Actulvl.enti[actuGest].ySpeed/60;
      }
    }

    

    actuGest++;
  }
  //  printf("%d %d\n", (int)Actulvl.enti[playernb].x, (int)Actulvl.enti[playernb].y);
  //printf("%d\n", Actulvl.ter.cellule[(int)Actulvl.enti[playernb].y][(int)Actulvl.enti[playernb].x].type);

  
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
    EtapeActuelleDuJeu = 2;
  }
}

// La fonction fais avancer l'entite actuGest vers la gauche
void AvanceGauche(int actuGest){
  int avance = 1;
  int verif = 0;
  
  // Si il n'y a pas de mur a gauche
  if(Actulvl.ter.cellule[(int)(Actulvl.enti[actuGest].y+Actulvl.enti[actuGest].sizeY/10)][(int)Actulvl.enti[actuGest].x].isPassable && Actulvl.ter.cellule[(int)(Actulvl.enti[actuGest].y+Actulvl.enti[actuGest].sizeY/10 *9)][(int)Actulvl.enti[actuGest].x].isPassable){
    //pour toute les autres entites
    while(Actulvl.enti[verif].type != -5){
      //si elles ont un y similaire
      if(verif != actuGest && Actulvl.enti[verif].y < Actulvl.enti[actuGest].y+Actulvl.enti[actuGest].sizeY && Actulvl.enti[verif].y+Actulvl.enti[verif].sizeY > Actulvl.enti[actuGest].y){
	//si elles ont le meme x 
	if(Actulvl.enti[verif].x < Actulvl.enti[actuGest].x && Actulvl.enti[verif].x+Actulvl.enti[verif].sizeX/100 *101 > Actulvl.enti[actuGest].x){
	  avance = 0;

	  // si tu peu pousser l'entite
	  if(Actulvl.ter.cellule[(int)(Actulvl.enti[verif].y+Actulvl.enti[verif].sizeY/10)][(int)(Actulvl.enti[verif].x-0.5)].isPassable && Actulvl.ter.cellule[(int)(Actulvl.enti[verif].y+Actulvl.enti[verif].sizeY/10*9)][(int)(Actulvl.enti[verif].x-0.5)].isPassable && Actulvl.enti[actuGest].type == 1){
	    Actulvl.enti[verif].xSpeed = Actulvl.enti[actuGest].xSpeed;
	  }else{
	    Actulvl.enti[actuGest].xSpeed = 0;
	  }
	      
	}
      }
      verif++;
    }
    // si aucune entite t'empeche d'avancer
    if(avance){
      if(Actulvl.enti[actuGest].type == 1){//cas joueur
	Actulvl.enti[actuGest].x += Actulvl.enti[actuGest].xSpeed/60;
      }else if(Actulvl.enti[actuGest].type == 5){// cas boite
	Actulvl.enti[actuGest].x += Actulvl.enti[actuGest].xSpeed/60;
	if((int)(Actulvl.enti[actuGest].x + Actulvl.enti[actuGest].xSpeed/60) != (int)(Actulvl.enti[actuGest].x)){
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
  }
}
