#ifndef JEU_HEADER_
#define JEU_HEADER_

#include <stdio.h>
#include <stdlib.h>

typedef struct Entite{

  float x;
  float y;

  float xSpeed;
  float ySpeed;

  float sizeX; //Animation
  float sizeY;

  float xHitbox; // taille de la hitbox
  float yHitbox;

  float xHitboxOffset; // offset entre x et le coin de la hitbox
  float yHitboxOffset;

  int type;
  int direction;
  
}entite;

typedef struct Cellule{
  int type;
  int isPassable;
}cellule;

typedef struct Terrain{
  cellule **cellule;
}terrain;


typedef struct Level{
  entite *enti;
  int maxEnti;
  terrain ter;
  int TerX;
  int TerY;
}level;

void addEntite(level *lev, int type, float longu, float larg);
level initNextLVL();
void DestroyLVL();
void Physique1play();
void VerifWin();
void AvanceGauche(int actuGest);
void AvanceDroite(int actuGest);
void AvanceHaut(int actuGest);
void AvanceBas(int actuGest);

#endif /* JEU_HEADER_ */
