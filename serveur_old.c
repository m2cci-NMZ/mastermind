/******************************************************************************/
/*			Application: ....			              */
/******************************************************************************/
/*									      */
/*			 programme  SERVEUR 				      */
/*									      */
/******************************************************************************/
/*									      */
/*		Auteurs :  ....						      */
/*		Date :  ....						      */
/*									      */
/******************************************************************************/	

#include<stdio.h>
#include <curses.h>

#include<sys/signal.h>
#include<sys/wait.h>
#include<stdlib.h>

#include "fon.h"     		/* Primitives de la boite a outils */

#define SERVICE_DEFAUT "1111"
#define MAXTENTATIVES 10

void serveur_appli (char *service);   /* programme serveur */



/******************************************************************************/	
/*---------------- programme serveur ------------------------------*/

int main(int argc,char *argv[])
{

	char *service= SERVICE_DEFAUT; /* numero de service par defaut */


	/* Permet de passer un nombre de parametre variable a l'executable */
	switch (argc)
 	{
   	case 1:
		  printf("defaut service = %s\n", service);
		  		  break;
 	case 2:
		  service=argv[1];
            break;

   	default :
		  printf("Usage:serveur service (nom ou port) \n");
		  exit(1);
 	}

	/* service est le service (ou numero de port) auquel sera affecte
	ce serveur*/
	
	serveur_appli(service);
}


/******************************************************************************/	
void serveur_appli(char *service)

/* Procedure correspondant au traitemnt du serveur de votre application */

{

int n;
int socketID;
int domaine = AF_INET;
int mode_comm = SOCK_STREAM;
char tampon[1000];

int sock_comm;
int nb_couleurs;
int nbtentatives = 0;

struct sockaddr_in *info_serveur;
struct sockaddr_in info_client;

/* avoir */
adr_socket(service, NULL, SOCK_STREAM, &info_serveur);

/* initialiser une socket pour un connexion TCP*/
socketID = h_socket(domaine,mode_comm);
h_bind(socketID, info_serveur);

/* ecouter sur la socket et accepter la connexion*/
h_listen(socketID, 1);
sock_comm = h_accept(socketID, &info_client);

/* ecrire le premier message dans le tampon (demander au client le nobre de couleurs)*/
sprintf(tampon, "Nombre de couleurs?\n");
write(sock_comm, tampon, sizeof(tampon));

/*lire la reponse du client*/
read(sock_comm, tampon, sizeof(tampon));

/* recupérer le nombre de couleurs envoyés par le client*/
printf("nbcouleurs= %c\n",tampon[0]);
nb_couleurs =  tampon[0]-'0';
int resultat[nb_couleurs];
char comparaison[nb_couleurs];
int reponseOK = 1;
char comparaison_tampon[nb_couleurs];

/*remplir le tableau des resultats*/
int i;
for (i=0; i<nb_couleurs; i++){
	resultat[i] = 0;
	}
/*jouer le jeu*/
while(nbtentatives<MAXTENTATIVES && reponseOK){

	nbtentatives++;
	printf("TENTATIVE %d\n", nbtentatives);
	sprintf(tampon,"Entrez la tentative numero %d\n",nbtentatives);
	write(sock_comm, tampon, sizeof(tampon));

  	/*lire la reponse du client*/
	read(sock_comm, tampon, sizeof(tampon));
	printf("resultat recu:%s\n",tampon);

	/*evaluer sa reponse*/
	for (int i=0; i< nb_couleurs; i++){
		sprintf(&comparaison[i] ,"%i", tampon[i] && (resultat[i]+'0'));
		printf("resultat: %d\n",resultat[i]);
		printf("client: %c\n",tampon[i]);
		printf("comparaison: %c\n",comparaison[i]);
		reponseOK = comparaison[i] -'0' && reponseOK;
	}

	tampon[0] = reponseOK + '0';
	/*lui dire si il a gagné ou non*/
	printf("reponseOK:%d\n",reponseOK);
	printf("tampon%c\n",tampon[0]);
	write(sock_comm, tampon, sizeof(tampon));

	/*lui indiquer où il a juste*/
	printf("valeurs trouvées: %s\n",comparaison);
	write(sock_comm, comparaison, sizeof(comparaison));
	}
if (nbtentatives < MAXTENTATIVES){
	sprintf(tampon, "Vous avez gagné en %d tentatives :)\n", nbtentatives+1);
	write(sock_comm, tampon, sizeof(tampon));
	}
	else{
	sprintf(tampon, "Vous avez perdu :(\n");
	write(sock_comm,tampon,sizeof(tampon));
	}
}
 	
/******************************************************************************/	

