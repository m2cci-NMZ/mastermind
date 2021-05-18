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
  char tampon[1000];
  char envoi[2000];
  char localName[25+1];
  char distName[25+1];
  int finPartie = 1;
  int nbcouleurs;
  
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

  /*initialisation partie */
  read(socketID, tampon, sizeof(tampon));
  printf("%s\n",tampon);
  scanf("%s",envoi);
  nbcouleurs= envoi[0] -'0';
  write(socketID, envoi, sizeof(envoi));

  while(finPartie) {
	
	/* lire question */
	read(socketID, tampon, sizeof(tampon));
  	printf("%s\n",tampon);
	/* envoyer reponse */
  	scanf("%s",envoi);
	printf("donnees envoyees: %s\n",envoi);
  	write(socketID, envoi, sizeof(envoi));
	/* lire si gagné */
	read(socketID, tampon, sizeof(tampon));
	finPartie = tampon[0] - '0';
  	printf("Etat de la partie: %d\n",finPartie);
	/* afficher reponse */
	read(socketID, tampon, sizeof(tampon));
  	printf("Affichage reponse: %s\n",tampon);
  }
  
 
  

  h_close;

 }

/*****************************************************************************/

