# GhostLab

## Compilation

Dans le répértoire ghostlab exécuter le makefile : 
- make 
  
##  Exécution: 

### Exécution Serveur :  
- ./server portTCP verb

où port est le port tcp du serveur et verb avec 0 ou 1 (1 pour activer le mode verbeux). 

### Exécution Client: 
- java Client portUdp ip portserveur verb

où portUdp est le port udp du client
ip est l'adresse ip de la machine où tourne le serveur
portserveur est le port du serveur 
et verb avec 0 ou 1 (1 pour activer le mode verbeux). 

## Architecture

### Server 
- server.c : Contient le programme principal du serveur.
- send.c : Contient les fonctions qui envoient les réponses des requêtes des clients.
- protocol_check.c : Contient les fonctions servant à vérifier le respect du protocole des messages reçus par le serveur.
- game.c : Contient les fonctions manipulant la struct servant à représenter une partie.
- list_game.c : Contient les fonctions manipulant la liste des parties.
- labyrinth.c : Contient les fonctions générant et manipulant le labyrinthe.
- player.c : Contient les fonctions manipulant la struct servant à représenter un joueur.
- list_player.c : Contient les fonctions manipulant la liste des joueurs.

### Client
- Client.java : Contient le programme principal du client.
- Request.java : Contient les fonctions gérant la communication avec le serveur.

## Extensions :

### Mode ITEMS et CHEAT :
 
### Activation :
 
ITEMS : Seul le chef de partie peut l'activer en envoyant :
> ITMON***
 
Le serveur répond alors :
> ITMOK*** - si c'est bon
 
> DUNNO*** - si vous n'avez pas les droits
 
CHEAT : Seul le chef de partie peut l'activer en envoyant :
> CHEAT***
 
Le serveur répond alors :
> CHTOK*** - si c'est bon
 
> DUNNO*** - si vous n'avez pas les droits

### Commandes :
ITEM? :
 
> ITEM?*** - affiche l'inventaire d'items
 
Réponses :

> ITEM! item1 item2 item3*** - si c'est bon, item1, item2 et item3 sont des entiers codés sur 1 octet, 
 où item1 correspond à l'item BON (Bonus), item2 correspond à l'item TEL (téléportation) et item3 à TRA (piège) 
 
> ITOFF*** - si le mode ITEMS n'est pas activé
 
ITTEL :
 
> ITTEL x y*** - se téléporte aux coordonnées x y, avec x et y des chaînes de caractères codés sur 3 octets, qui correspondent à un numéro de ligne. 
 
Réponses :

> TELOK x y*** - si c'est bon, avec x et y des chaînes de caractères codés sur 3 octets, qui correspondent à un numéro de ligne.
 
> NOITM*** - si vous ne possédez pas d'item téléportation.
 
> DUNNO*** - si c'est un emplacement impossible (l'item n'est pas consommé).

> ITOFF*** - si le mode ITEMS n'est pas activé.
 
ITBON :
 
> ITBON*** - active un item bonus qui augmente les prochains points obtenus par la capture de fantomes, les points sont multipliés par le nombre de fantomes attrapés. Ne dure qu'un déplacement.
 
Réponses :

> BONOK*** - si c'est bon
 
> NOITM*** - si vous ne possédez pas d'item bonus
 
> DUNNO*** - si un bonus est toujours actif

> ITOFF*** - si le mode ITEMS n'est pas activé
 
ITTRA :
 
> ITTRA x y*** - pose un piège à l'emplacement x y, avec x et y des chaînes de caractères codés sur 3 octets, qui correspondent à un numéro de ligne.
 
Réponses :

> TRAOK x y*** - si c'est bon, avec x et y des chaînes de caractères codés sur 3 octets, qui correspondent à un numéro de ligne. 
 
> NOITM*** - si vous ne possédez pas d'item trap.
 
> DUNNO*** - si c'est un emplacement impossible (l'item n'est pas consommé).

> ITOFF*** - si le mode ITEMS n'est pas activé.
 
KILLF :
 
> KILLF*** - récupère tous les fantomes dans la partie, et termine la partie. 
 
Réponses :

> KILAL id p*** - si c'est bon, avec id codé sur 8 octets (l'id de l'utilisateur) qui est une chaîne de caractères formées de exactement 8 caractères et p codé sur 4 (le score) une chaîne de caractères formées de 4 caractères.
 
>FGONE id p+++ - message multi diffusé pour alerter de l'obtention des fantomes, avec id(l'id de l'utilisateur) qui est une chaîne de caractères formées de exactement 8 caractères et p(le score) une chaîne de caractères formées de 4 caractères.

> CHOFF*** - si le mode CHEAT n'est pas activé
 
LABSH :
 
> LABSH*** - affiche le labyrinthe courant
 
Réponses :

> SHOW! h w lab*** - si c'est bon, avec h et w des entiers en little endian codés sur 2 octets et lab une suite de h*w caractères  

> CHOFF*** - si le mode CHEAT n'est pas activé

GODME :
 
> GODME*** - rend insensible aux pièges
 
Réponses :

> GODON*** - si c'est bon

> ACTIF*** - si le god mode est déjà actif

> CHOFF*** - si le mode CHEAT n'est pas activé
 
UNGOD :
 
> GODME*** - rend insensible aux pièges
 
Réponses :

> GDOFF*** - si c'est bon

> NOTON*** - si le god mode n'était pas activé

> CHOFF*** - si le mode CHEAT n'est pas activé
 
GIVCH :
 
> GIVCH id*** - donne le cheat mode à un joueur avec id son identifiant qui est une chaîne de caractères formée d'exactement 8 caractères.
 
Réponses :

> GAVE! id*** - si c'est bon, avec id étant l'id du destinataire qui est une chaîne de caractères formée d'exactement 8 caractères. MESSP est aussi envoyé pour l'avertir

> HASCH*** - si le destinataire a déjà le cheat mode
 
> NOPLY*** - si aucun joueur ne correspond à l'id

> CHOFF*** - si le mode CHEAT n'est pas activé
 
ITINF :
 
> ITINF*** - items illimités, seulement activable dans le mode ITEMS et en possédant le cheat mode
 
Réponses :

> INFON*** - si c'est bon

> ACTIF*** - si il est déjà actif

> CHOFF*** - si le mode CHEAT n'est pas activé

> ITOFF*** - si le mode ITEMS n'est pas actif
 
NOINF :
 
> NOINF*** - désactive le mode items illimités
 
Réponses :

> INOFF*** - si c'est bon

> NOTON*** - si il n'était pas activé

> CHOFF*** - si le mode CHEAT n'est pas activé

> ITOFF*** - si le mode ITEMS n'est pas actif
