# Installation
## Récupération du fichier :
Il suffit de dézipper l'ensemble du dossier que nous vous avons fournie en
laissant les fichiers dans les bons sous répertoires(notamment les images dans
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
Un makefile vous est fourni avec les fichiers de code du jeu, il suffit donc de
l'appeler. Pour cela la commande "make" est suffisante. Le makefile va, si
toutes les librairies cités ci-dessus sont présentes, ne retourner aucune erreur
et vas créer un fichier nommé jeu.
## Pour lancer le jeu :
Une fois le fichier "jeu" dans votre répertoire, (si vous n'avez pas le fichier
jeu, référez-vous à l'étape "Pour compiler"), il suffit de l'executer. Pour cela
le plus simple est d'utiliser la commande "./jeu" qui devrait lancer le 2048
dans son interface graphique.
# Information
## Touches
### F11 :
Permet de mettre le jeu en plein écran.
### Echape :
Permet de quitter le jeu si sur le menu.
### b, h, g, d :
Touche de déplacement dans toutes les parties de jeu.
### Flèches directionnels :
Touches de déplacements dans le cas d'une partie graphique du 2048.
## Autre :
### Les sliders dans les paramètres :
Ceci ne sont pas des slidder, il faut donc cliquer à l'endroit où l'on désire
mettre le curseur.
### La selection des touches :
Quelques touches ne peuvent pas être assignées à un déplacement, notamment : ' ',
'-', 'ESC'. Car ' ' créé un bug dans le fichier de sauvegarde.'-' est la
touche qui correspond à aucune touche liée et 'ESC' a un comportement
prédéfini différent du déplacement dans toutes les "scènes du jeu".
De plus, vous pouvez lier plusieurs fois la même touche si cela vous dit mais la
touche liée plusieurs fois auras toujours le même comportement.
### Boutons :
L'affichage sur les boutons est à titre indicatif, il n'y a pas besoin
d'attendre que le bouton ait un affichage de "séléctionné" avant de pouvoir
cliquer dessus.
# Cas de bug due à SDL :
## Boutons non fonctionnel en demarrage en plein ecran :
Parfois SDL ne récupère pas les déplacements de la souris lors de
l'initialisation en plein écran. Pour régler le problème, un double appuie sur
la touche f11 (ce qui va quitter puis revenir en mode plein écran) réglera le
problème.
## L'affichage ne s'adapte plus à la taille de l'écran :
Parfois SDL arrète de considérer l'event "resize" de l'écran. Le code n'a
donc plus aucun moyen de récupérer la taille de l'écran mis par
l'utilisateur. Une solution à ce "bug" est un double f11 qui va reset la fenetre
SDL et l'event fonctionnera à nouveau après cela. 
# Pour tout autre question :
N'hésitez pas à contacter l'un des deux développeurs de ce projet :
 - Laurencot Mathieu : mathieu.laurencot@etu.uca.fr
 - Payard Clément : clement.payard@etu.uca.fr
