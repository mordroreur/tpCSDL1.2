# Installation
## Récupération du fichier :
Il suffit de dézipper l'ensemble du dossier que nous vous avons fournie en
laissant les fichier dans les bon sous répertoire(notamment les images dans
"Res/Image/" et le son dans "Res/Sond").
Ou de faire la commande :
"git clone https://github.com/mordroreur/tpCSDL1.2.git ".
## Librairyes nécéssaire :
Afin de compiler, les librairies necessaires doivent-etre installées sur votre ordinateur :
 - libsdl1.2
 - libsdl1.2-dev
 - libsdl-mixer1.2
 - libsdl-mixer1.2-dev
 - libsdl-ttf1.2
 - libsdl-ttf1.2-dev
 - libsdl-image1.2
 - libsdl-image1.2-dev
 - libsdl-gfx1.2-5
 - libsdl-gfx1.2-dev
## Pour compiler :
Un makefile vous est fourni avec les fichier de code du jeu, il suffit donc de
l'appeler. Pour cela la commande "make" est suffisante. Le makefile vas, si
toute les librairies cités ci dessus sont présentes, ne retourner aucune erreur
et vas créer un fichier nommé jeu.
## Pour lancer le jeu :
Un fois le fichier "jeu" dans votre répertoire, (si vous n'avez pas le fichier
jeu, référez vous à l'étape "Pour compiler"), il suffit de l'executer. Pour cela
le plus simple est d'utiliser la commande "./jeu" qui devrais lancer le 2048
dans son interface graphique.
# Information
## Touches
### F11 :
Permet de mettre le jeu en plein écran.
### Echape :
Permet de quitter le jeu si sur le menu.
### b, h, g, d :
Touche de déplacement dans toute les parties de jeu.
### flèches directionnels :
Touche de déplacement dans le cas d'une partie graphique du 2048.
## Autre :
### Les slider dans les paramètres :
Se ne sont pas des slidder, il faut donc cliquer à l'endroit on l'on désire
mettre le curseur.
### La selection des touches :
Quelques touches ne peuvent pas être assignés à un déplacement, notamment : ' ',
'-', 'ESC'. Car ' ' créais un bug dans le fichier de sauvegarde, '-' est la
touche qui correspond à aucune touche n'est lié et 'ESC' a un comportement
prédéfini différent du déplacement dans toute les "scènes du jeu".
Deplus vous pouvez lier plusieurs fois la même touche si cela vous dit mais la
touche lié plusieurs fois auras toujours le même comportement.
### Bouttons :
L'affichage sur les boutons est à titre indicatif, il n'y a pas besoins
d'attendre que le bouton est un affichage de "séléctionné" avant de pouvoir
cliquer dessus.
# Cas de bug due à SDL :
## Boutons non fonctionnel en demarrage en plein ecran :
Parfois SDL ne récupère pas les déplacement de la souris lors de
l'initialisation en plein écran. Pour régler le probleme, un double appuye sur
la touche f11 (ce qui va quitter puis reven,ir en mode plein écran) régleras le
problème.
## L'affichage ne s'adapte plus à la taille de l'écran :
Parfois SDL arrète de considérer l'event "resize" de l'écran. Mon code n'as
donc plus aucun moyen de récupérer la taille de l'écran mis par
l'utilisateur. Un solution à ce "bug" est un double f11 qui va reset la fenetre
SDL et l'event fonctionnera à nouveau après cela. 
# Pour tout autre question :
N'hésitez pas à contacter l'un des deux développeurs de ce projet :
 - Payard Clément : clement.payard@etu.uca.fr
 - Laurencot Mathieu : mathieu.laurencot@etu.uca.fr
