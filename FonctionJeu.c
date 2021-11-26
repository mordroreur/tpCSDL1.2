#include "FonctionJeu.h"
#include <stdio.h>


ter InitVide(int n){
  // creation de la variable que l on initialise
  ter T;
  // permet que si on return un terrain de taille -1 il y a eu une erreur
  T.taille = -1;
  // On initialise le terrain(on return si erreur)
  T.tab = (int ** )malloc(sizeof(int *) * n);
  if(T.tab == NULL){
    return T;
  }else{
    for(int i = 0; i < n; i++){
      T.tab[i] = (int *)malloc(sizeof(int) * n);
      if(T.tab[i] == NULL){
	for(int j = 0; j < i; i++){
	  free(T.tab[j]);
	}
	free(T.tab);
	return T;
      }
    }
  }
  // On met que toute les cases sont vides
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      T.tab[i][j] = -1;
    } 
  }
  // On met les valeurs initiales a toute les caracteristique du terrain
  T.taille = n;
  T.max = 2;
  T.score = 0;
  T.vide = n*n;
  //On creer un fichier de sauvegarde
  FILE *save = fopen(SAVE_NAME, "w");
  fprintf(save, "N %d %d\n", T.taille, T.taille);
  fflush(save);
  fclose(save);
  // On met les deux premier zeros dans la grille
  SetRandomCase(&T, 2);
  SetRandomCase(&T, 2);
  // On retourne le terrain ainsi genere
  return T;
}


void afficheTer(ter T){
  clearScreen;
  printf("MEILLEUR CASE : %d\t\t\t SCORE : %d\n", T.max, T.score);
  for(int j = 0; j < T.taille; j++){
      printf("-------");
    }
    printf("-\n");
  for(int i = 0; i < T.taille; i++){
    for(int j = 0; j < T.taille; j++){
      printf("|      ");
    }
    printf("|\n");
    for(int j = 0; j < T.taille; j++){
      if(T.tab[i][j] == -1){
	printf("|      ");
      }else{
	if(T.tab[i][j] < 10){
	  printf("|   %d  ", T.tab[i][j]);
	}else if(T.tab[i][j] < 100){
	  printf("|  %d  ", T.tab[i][j]);
	}else if(T.tab[i][j] < 1000){
	  printf("|  %d ", T.tab[i][j]);
	}else if(T.tab[i][j] < 10000){
	  printf("| %d ", T.tab[i][j]);
	}else if(T.tab[i][j] < 100000){
	  printf("|%d ", T.tab[i][j]);
	}else if(T.tab[i][j] < 1000000){
	  printf("|%d", T.tab[i][j]);
	}else{
	  printf("| Nan  ");
	}
      }
    }
    printf("|\n");
    for(int j = 0; j < T.taille; j++){
      printf("|      ");
    }
    printf("|\n");
    for(int j = 0; j < T.taille; j++){
      printf("-------");
    }
    printf("-\n");
  }
}


int SetRandomCase(ter *T, int n){
  // Dans le cas ou il reste de la place sur le terrain
  if(T->vide != 0){
    int mis = 0; // Un case a t elle etais ajoute
    int videVue = -1; // case ou l'on peu mettre
    int r = random()%T->vide; // numero de la case ou l on va mettre
    // ajout de la nouvelle case au fichier de sauvegarde
    FILE *save = fopen(SAVE_NAME, "a");
    fprintf(save, "%d %d ", n, r);
    fflush(save);
    fclose(save);
    //pour toute les cases
    for(int i = 0; i < T->taille; i++){
      for(int j = 0; j < T->taille; j++){
	// si la case est vide
	if(T->tab[i][j] == -1){
	  // Une case de plus ou on peu mettre
	  videVue++;
	  // si le numero de case coincide avec le numero de cette case on met a cette case
	  if(videVue == r){
	    T->tab[i][j] = n;
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
      T->vide--;
      return 1;
    // sinon il doit y avoir une erreur
    }else{
      printf("Erreur lors de la mise d'un block : %d %d\n", T->vide, r);
      return 0;
    }
  // Si il n'y a plus de case libre... cas du game over
  }else{
    return 0;
  }
}



// Verifie si le deplacement dans le sens depNum est possible
int CanDep(ter T, int depNum){
  // Initialisation des variable qui change en fonction du sens de deplacement
  int Xnext = (depNum == 1)?1:(depNum == 3)?-1:0;
  int Ynext = (depNum == 4)?1:(depNum == 2)?-1:0;
  // Pour chacune des cases
  for(int i = 0; i < T.taille; i++){
    for(int j = 0; j < T.taille; j++){
      // Si la case n est pas vide
      if(T.tab[i][j] != -1){
	if(i + Xnext < T.taille && i+Xnext > -1 && j + Ynext < T.taille && j + Ynext > -1){
	  // si la case sur laquelle elle se dirige est vide ou de sa taille
	  if(T.tab[i+Xnext][j+Ynext] == -1 || T.tab[i+Xnext][j+Ynext] == T.tab[i][j]){
	    // on retourne que c est possible
	    return 1;
	  }
	}
      }
    }
  }
  //sinon on retourne que c est impossible
  return 0;
}


void CaseMouve(ter *T, int depNum){
  // Initialisation des variable qui change en fonction du sens de deplacement
  int Xnext = (depNum == 1)?1:(depNum == 3)?-1:0;
  int Ynext = (depNum == 4)?1:(depNum == 2)?-1:0;
  // varible necessaire a pas que 0 2 2 4 se transforme en 0 0 0 8 mais bien en 0 0 4 4
  int *isfusX = (int *)malloc(T->taille * T->taille/2 * sizeof(int));
  int *isfusY = (int *)malloc(T->taille * T->taille/2 * sizeof(int));
  for(int i = 0; i < T->taille * T->taille/2; i++){
    // les tableaux sont initialises vide
    isfusX[i] = -1;
    isfusY[i] = -1;
  }
  // pour chacune des cases (le sens de la boucle depend de Xnext et Ynext)
  for(int i = (Xnext == 1)?T->taille-1:(Xnext == -1)?1:0; (Xnext == 1)?i > -1:i < T->taille; (Xnext == 1)?i--:i++){
    for(int j = (Ynext == 1)?T->taille-1:(Ynext == -1)?1:0; (Ynext == 1)?j > -1:j < T->taille; (Ynext == 1)?j--:j++){
      // si la case n est pas vide
      if(T->tab[i][j] != -1){
	// nombre de deplacement entre la case et son point d arrive
	int whereGo = 0;
	// pour toute les cases dans la direction appuye par le joueur
	for(int k = 1; k < T->taille; k++){
	  if(i+(Xnext*k) < T->taille && i+(Xnext*k) > -1 && j+(Ynext*k) < T->taille && j+(Ynext*k) > -1){
	    // si la case est vide
	    if(T->tab[i+(Xnext*k)][j+(Ynext*k)] == -1){
	      // la case devient la nouvelle plus loin sur laquelle elle peu aller
	      whereGo = k;
	    // si la case est de meme valeurs
	    }else if(T->tab[i+(Xnext*k)][j+(Ynext*k)] == T->tab[i][j]){
	      // peut elle se combiner
	      int PeuDuo = 1;
	      // pour toute les cases deja combine lors de se deplacement
	      for(int m = 0; m < T->taille * T->taille/2; m++){
		if(isfusX[m] == -1){
		  break;
	        // si c la case appartient
		}else if(isfusX[m] == i+(Xnext*k) && isfusY[m] == j+(Ynext*k)){
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
	  //si la nouvelle case n est pas vide
	  if(T->tab[i+(Xnext*whereGo)][j+(Ynext*whereGo)] != -1){
	    //on met les deux cases dans la nouvelle
	    T->tab[i+(Xnext*whereGo)][j+(Ynext*whereGo)] += T->tab[i][j];
	    //on vide l ancienne case
	    T->tab[i][j] = -1;
	    // on met a jour le score
	    T->score += T->tab[i+(Xnext*whereGo)][j+(Ynext*whereGo)];
	    // on met a jour le maximum
	    T->max = MAX(T->max, T->tab[i+(Xnext*whereGo)][j+(Ynext*whereGo)]);
	    // on met que une case vient d etre libere
	    T->vide++;
	    // on met que la case vient d etre fusionne sur ce tour
	    int new = 0;
	    while(isfusX[new] != -1){new++;}
	    isfusX[new] = i+(Xnext*whereGo);
	    isfusY[new] = j+(Ynext*whereGo);
	  }else{
	    // sinon on met la nouvelle case a la valeur de notre case et la
	    // precedente a vide
	    T->tab[i+(Xnext*whereGo)][j+(Ynext*whereGo)] = T->tab[i][j];
	    T->tab[i][j] = -1;
	  }
	}
      }
    }
  }
  // on libere la memoire
  free(isfusX);
  free(isfusY);
  //on fais apparaitre un 2 a 75 pourcent de chance, un 4 sinon
  SetRandomCase(T, (rand()%100 < 75)?2:4);
}


void LibereTer(ter *T){
  // On libere toute la memoire mis sur le tableau
  if(T->tab != NULL){
    for(int i = 0; i < T->taille; i++){
      if(T->tab[i] != NULL){
	free(T->tab[i]);
      }
    }
    free(T->tab);
  }
  // On met toute les caracteristique a 0 au cas ou quelque chose les appelles
  T->taille = 0;
  T->max = 0;
  T->score = 0;
  T->vide = 0;
}




ter ReadEnCoursSave(){
  ter T;
  T.taille = -1;
  FILE *save = fopen(SAVE_NAME, "r");
  if(save == NULL){
    printf("C'est assez cocasse... Un fichier ne devrais pas disparaitre...\n");
    return T;
  }

  printf("TODOOO : Read saved file");

  TODO
  
  return T;
}

