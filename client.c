/******************************************************************************/
/*			Application: ...					*/
/******************************************************************************/
/*									      */
/*			 programme  CLIENT				      */
/*									      */
/******************************************************************************/
/*									      */
/*		Auteurs : ... 					*/
/*									      */
/******************************************************************************/	


#include <stdio.h>
#include <curses.h> 		/* Primitives de gestion d'ecran */
#include <sys/signal.h>
#include <sys/wait.h>
#include<stdlib.h>

#include "fon.h"   		/* primitives de la boite a outils */

#define SERVICE_DEFAUT "1111"
#define SERVEUR_DEFAUT "127.0.0.1"
#define MAXTENTATIVES 10

void client_appli (char *serveur, char *service);


/*****************************************************************************/
/*--------------- programme client -----------------------*/

int main(int argc, char *argv[])
{

	char *serveur= SERVEUR_DEFAUT; /* serveur par defaut */
	char *service= SERVICE_DEFAUT; /* numero de service par defaut (no de port) */


	/* Permet de passer un nombre de parametre variable a l'executable */
	switch(argc)
	{
 	case 1 :		/* arguments par defaut */
		  printf("serveur par defaut: %s\n",serveur);
		  printf("service par defaut: %s\n",service);
		  break;
  	case 2 :		/* serveur renseigne  */
		  serveur=argv[1];
		  printf("service par defaut: %s\n",service);
		  break;
  	case 3 :		/* serveur, service renseignes */
		  serveur=argv[1];
		  service=argv[2];
		  break;
    default:
		  printf("Usage:client serveur(nom ou @IP)  service (nom ou port) \n");
		  exit(1);
	}

	/* serveur est le nom (ou l'adresse IP) auquel le client va acceder */
	/* service le numero de port sur le serveur correspondant au  */
	/* service desire par le client */
	
	client_appli(serveur,service);
}

/*****************************************************************************/
void client_appli (char *serveur,char *service)

/* procedure correspondant au traitement du client de votre application */

{
  
  pid_t pid;
  int id;
  char msg[255];
  int socketID;
  int socket_dist;
  char tampon[80];
  char envoi[80];
  char localName[25+1];
  char distName[25+1];
  int arreterPartie = 0;
  int nbcouleurs;
  int nbtentatives = 1;
  
  struct sockaddr_in *infoServeur;
  struct sockaddr_in *infoClient;
  
  /* locaux */
  /*gethostname(localName, MAXHOSTNAMELEN);
  adr_socket("0", localName, SOCK_STREAM, &infoClient);
   h_bind(socket, &infoServeur);*/
  socketID = h_socket(AF_INET, SOCK_STREAM); 

  /* distant*/

  adr_socket(service, serveur, SOCK_STREAM, &infoServeur);

  /*if(socketID == -1)
    {
      printf("la creation socket echoué\n");
      exit(0);
    } verification faite par adr_socket*/

  h_connect(socketID, infoServeur);
  printf("Bienvenue a jeu du mastermind\n");
  printf("Regles du jeu: on choisit d'abord le nombre de couleurs a deviner(entre 1 et 9). On doit ensuite deviner les couleurs identifiées par un chiffre entre 0 et 7. On nous indique ensuite  à quelle position on a juste (0 pour faux, 1 pour juste).\n");
  /*initialisation partie: le client envoie le nombre de couleurs qu'il souhaite deviner (entre 1 et 9) */
  printf("Entrez le nombre de couleurs a deviner (entre 1 et 9):\n");
  scanf("%1s",envoi);
  /*Transformer le char en int*/
  nbcouleurs= envoi[0] -'0';
  /*Envoyer le nombre de couleurs a deviner au serveur*/
  write(socketID, envoi, sizeof(envoi));
  /*Jouer au jeu: on s'arrete lorsque l'on a deviné toutes les couleurs (arreterpartie = 1) ou lorsqu'on  a dépassé le nombre maximal de tentatives*/
  while(!arreterPartie && nbtentatives<=MAXTENTATIVES) {
	printf("Il vous reste %d tentatives\n", nbtentatives - MAXTENTATIVES);
	/*Affichage d'un message pour indiquer au client de saisir une tentative au clavier*/
	printf("Entrez une serie de %d couleurs identifiées par un numero entre 0 et 7 \n",nbcouleurs);
	/*Recuperer la tentative et la stocker dans un tableau de char*/
  	scanf("%s",envoi);
	printf("donnees envoyees: %s\n",envoi);
	/*envoyer la tenative au serveur pour qu'il l'évalue*/
  	write(socketID, envoi, sizeof(envoi));
	/*Le serveur envoie d'abord un char qui permet de savoir si l'on a gagné ou pas*/
	read(socketID, tampon, sizeof(tampon));
	/*On transforme ce char en int pour pouvoir l'utiliser comme un booleen*/ 
	arreterPartie = tampon[0] - '0';	
  	printf("Etat de la partie: %d\n",arreterPartie);
	/*Le serveur envoie ensuite un tableau de char permettant de savoir quelles couleurs ont été trouvées: 0 pour une couleur fausse, 1 pour une couleur juste*/
	read(socketID, tampon, sizeof(tampon));
  	printf("Couleurs trouvées: %s\n",tampon);
	nbtentatives++;
  }
/*Le serveur envoie au client un message lui indiquant si il a gagné ou perdu*/  
  read(socketID, tampon, sizeof(tampon));
  printf("%s\n",tampon);
/*Le client coupe la communication*/
  h_close(socketID);

 }

/*****************************************************************************/

