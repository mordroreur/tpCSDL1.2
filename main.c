#include "main.h"
#include "FonctionJeu.h"
#include <stdio.h>

int main()
{
  srand(time(NULL));

  int hightscore;
  int endedGamesWin;
  int endedLoseGame;
  char mouvsup[4];
  int nbReplay;

  FILE *param = fopen(PARAM_NAME, "r");
  if (param == NULL)
  {
    hightscore = 0;
    endedGamesWin = 0;
    endedLoseGame = 0;
    mouvsup[0] = '-';
    mouvsup[1] = '-';
    mouvsup[2] = '-';
    mouvsup[3] = '-';
    nbReplay = 0;
    writesaveFile(hightscore, endedGamesWin, endedLoseGame, mouvsup, nbReplay);
  }
  else
  {
    fscanf(param, "%d\n%d\n%d\n%c\n%c\n%c\n%c\n%d\n", &hightscore,
           &endedGamesWin, &endedLoseGame, &mouvsup[0], &mouvsup[1],
           &mouvsup[2], &mouvsup[3], &nbReplay);
    fclose(param);
  }

  int EtapeDuJeu = -1;
  int isSave = 0;
  ter plateau;
  char play = 'O';

  FILE *save = fopen(SAVE_NAME, "r");
  if (save != NULL)
  {
    isSave = 1;
    fclose(save);
  }

  EtapeDuJeu = 2;

  while (EtapeDuJeu)
  {

    switch (EtapeDuJeu)
    {

    case 2: /* Cas d'arriver dans le jeu */
      // TODO changer ca pour tout setup
      // et ajouter tailleY

      clearScreen;
      printf("Bonjours et bienvenue sur ce jeu de 2048 version console.\n");
      // Si une savegarde a ete apercu en cours
      if (isSave)
      {
        printf("Voulez vous chargé la partie sauvegardé ?\nTapez o pour oui ou "
               "n pour non : ");
        scanf("%c", &play);
        while ((getchar()) != '\n')
          ;
        if (play == 'o')
        {
          plateau = ReadEnCoursSave(0.5);

          if (plateau.tailleX == -1)
          {
            printf("Il y a eu un problème d'allocation de la mémoire. Le jeu "
                   "ne peu continuer dans ces conditions.\n");
            EtapeDuJeu = -1;
          }
          else
          {
            afficheTer(plateau);
            printf("Appuyez sur une touche pour reprendre la partie.");
            getchar();
            // on cosidere que le joueur ne peu pas bouger
            int GameOver = 1;
            // pour les quatre deplacement
            for (int i = 1; i < 5; i++)
            {
              if (CanDep(plateau, i))
              {
                // si le joueur peu bouger on change cette variable
                GameOver = 0;
                break;
              }
            }
            // si GameOver est toujours a 1 alors le joueur a perdu
            if (GameOver)
            {
              // il va donc a l ecran 4
              EtapeDuJeu = 4;
            }
            else
            {
              EtapeDuJeu = 3;
            }
          }
        }
        else
        {
          printf(
              "La partie à été supprimé au profit de la prochaine partie.\n");
          EtapeDuJeu = 5;
        }
        // si pas de partie en cours a charge
      }
      else
      {
        EtapeDuJeu = 5;
      }
      break;

    case 3: /* Cas du jeu en affichage console */
      printf("Entrez le déplacement(d = droite, h = haut, g = gauche, b = bas) "
             "\n\tou q pour quitter : ");
      // recuperation de l entree joueur
      scanf("%c", &play);
      while ((getchar()) != '\n')
        ;
      if (play == mouvsup[0] && mouvsup[0] != '-')
      {
        play = 'g';
      }
      else if (play == mouvsup[1] && mouvsup[1] != '-')
      {
        play = 'h';
      }
      else if (play == mouvsup[2] && mouvsup[2] != '-')
      {
        play = 'd';
      }
      else if (play == mouvsup[3] && mouvsup[3] != '-')
      {
        play = 'b';
      }
      // si c est une entree de deplacement
      if (play == 'd' || play == 'h' || play == 'g' || play == 'b')
      {
        int depNum =
            (play == 'b') ? 1 : (play == 'g') ? 2 : (play == 'h') ? 3 : 4;
        // si le joueur peu se deplacer dans la direction qu il a choisi
        if (CanDep(plateau, depNum))
        {
          // on ajoute le deplacement a la sauvegarde
          FILE *save = fopen(SAVE_NAME, "a");
          fprintf(save, "%c ", play);
          fflush(save);
          fclose(save);
          // on deplace
          CaseMouve(&plateau, depNum);
          // on fais apparaitre un 2 a 75 pourcent de chance, un 4 sinon
          SetRandomCase(&plateau, (rand() % 100 < 90) ? 2 : 4);
          // on affiche le nouveau jeu
          afficheTer(plateau);
          // on cosidere que le joueur ne peu pas bouger
          int GameOver = 1;
          // pour les quatre deplacement
          for (int i = 1; i < 5; i++)
          {
            if (CanDep(plateau, i))
            {
              // si le joueur peu bouger on change cette variable
              GameOver = 0;
              break;
            }
          }
          // si GameOver est toujours a 1 alors le joueur a perdu
          if (GameOver)
          {
            // il va donc a l ecran 4
            EtapeDuJeu = 4;
          }
        }
        else
        {
          printf("Ce déplacement n'est pas valide!!!");
        }
        // si le joueur selectionne de sortir du jeu
      }
      else if (play == 'q')
      {
        // il va a l etape de fermeture du jeu
        EtapeDuJeu = -1;
      }
      else
      {
        printf("ce n'est pas un déplacement reconnu\n");
      }
      break;

      // cas de fin de partie
    case 4:
      afficheTer(plateau);
      if (plateau.max < 2048)
      {
        printf("Vous avez perdu! ");
        endedLoseGame++;
      }
      else
      {
        printf("Vous avez gagné! ");
        endedGamesWin++;
      }
      printf("Votre meilleur score est de %d ", plateau.score);
      if (plateau.tailleX == plateau.tailleY && plateau.tailleX == 4 &&
          plateau.score < hightscore)
      {
        printf("et donc inferieur au meilleur score de %d.\n", hightscore);
      }
      else if (plateau.tailleX == plateau.tailleY && plateau.tailleX == 4)
      {
        printf("et donc suppérieur au meilleur score de %d.\n", hightscore);
        hightscore = plateau.score;
      }
      printf("Voulez-vous sauvegarder le replay?(o = oui, n = non) : ");
      scanf("%c", &play);
      while ((getchar()) != '\n')
        ;

      if (play == 'o')
      {
        char tmp[100];
        sprintf(tmp, "%s%d", SAVE_REPLAY, nbReplay);
        copiFile(SAVE_NAME, tmp);
        if (remove(SAVE_NAME) != 0)
        {
          printf("Il y a un probleme inattendu...\n");
        }
        nbReplay++;
      }
      else
      {
        if (remove(SAVE_NAME) != 0)
        {
          printf("Il y a un probleme inattendu...\n");
        }
      }
      writesaveFile(hightscore, endedGamesWin, endedLoseGame, mouvsup,
                    nbReplay);
      printf("Voulez-vous recommencer une partie?(o = oui, n = non) : ");
      scanf("%c", &play);
      while ((getchar()) != '\n')
        ;
      if (play == 'o')
      {
        LibereTer(&plateau);
        EtapeDuJeu = 5;
      }
      else
      {
        EtapeDuJeu = -1;
      }
      break;

    case 5:
      // on lance une partie avec un terrain de taille 4
      printf("Appuyez sur entrer pour lancer le jeu.");
      plateau = InitVide(4, 2);
      // cas ou l initialisation du terrain c est mal passe on quitte le jeu
      if (plateau.tailleX == -1)
      {
        printf("Il y a eu un problème d'allocation de la mémoire. Le jeu ne "
               "peu continuer dans ces conditions.\n");
        EtapeDuJeu = -1;
      }
      else
      {
        getchar();
        afficheTer(plateau);
        EtapeDuJeu = 3;
      }
      break;

    case -1: // Cas de fermeture du jeu : liberation de la memoire et on sort de
             // la boucle
      printf("Merci d'avoir joué, nous espérons vous revoir bientôt!!!\n");
      LibereTer(&plateau);
      getchar();
      clearScreen;
      EtapeDuJeu = 0;
      break;

    default:
      printf("Pas implementé!!!\n");
      EtapeDuJeu = 0;
      break;
    }
  }

  return 0;
}
