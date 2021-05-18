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
#include <time.h>

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
char tampon[80];

int sock_comm;
int nb_couleurs;
int nbtentatives = 1;
int arreterPartie = 1;

struct sockaddr_in *info_serveur;
struct sockaddr_in info_client;

char resultat[nb_couleurs];
char comparaison[80];
char comparaison_tampon[nb_couleurs];

/*---------------------------------------------------------------
 * Creation des sockets
 *--------------------------------------------------------------*/

/* avoir */
adr_socket(service, NULL, SOCK_STREAM, &info_serveur);

/* initialiser une socket pour un connexion TCP*/
socketID = h_socket(domaine,mode_comm);
h_bind(socketID, info_serveur);

/* ecouter sur la socket et accepter la connexion*/
h_listen(socketID, 1);
sock_comm = h_accept(socketID, &info_client);

/*lire la reponse du client sur le nombre de couleurs*/
read(sock_comm, tampon, sizeof(tampon));

/* recupérer le nombre de couleurs envoyés par le client*/
printf("nbcouleurs= %c\n",tampon[0]);
nb_couleurs =  tampon[0]-'0';


/*remplir le tableau des resultats*/
int i;
srand (time (NULL));
for (i=0; i<nb_couleurs; i++){
	/*le tableau des resulats contient des char entre 0 et 6, representant les 7 couleurs a deviner*/
	resultat[i] = '0'+ (random()%7);
	}
printf("tableau des resultats: %s\n", resultat);
/*jouer le jeu*/
do
{
	printf("TENTATIVE %d\n", nbtentatives);

  	/*lire la reponse du client*/
	read(sock_comm, tampon, sizeof(tampon));
	printf("resultat recu:%s\n",tampon);
	/*on initialise un booleen qui nous permettra de sortir de la boucle une fois que le client a juste*/
	arreterPartie = 1;
	/*evaluer sa reponse*/
	for (int i=0; i< nb_couleurs; i++){
		/*On stocke d'abord les reponses justes et fausses dans un tableau*/
		sprintf(&comparaison[i] ,"%c", (tampon[i] == resultat[i])+'0');
		printf("resultat: %c\n",resultat[i]);
		printf("client: %c\n",tampon[i]);
		printf("comparaison: %i\n",comparaison[i]-'0');
		/*On evalue la reponse, pour savoir is le client a gagné ou pas*/
		arreterPartie = (comparaison[i] -'0') && arreterPartie;
	}
	printf("arreterPartie: %d\n",arreterPartie);
	tampon[0] = arreterPartie + '0'; /* convesion en char */
	/*On indique au client si il a gagné ou pas*/
	write(sock_comm, tampon, sizeof(tampon));

	/*On lui envoie le tavleau lui permettant de savoir quelles positions sont justes*/
	printf("valeurs trouvées: %s\n",comparaison);
	write(sock_comm, comparaison, sizeof(comparaison));

	nbtentatives++;
	}while((nbtentatives <= MAXTENTATIVES) && !arreterPartie);
	/*On sort de la boucle lorsque l'on a atteint le nombre maximale de tentatives, ou si le client a gagné*/

/* Le client a gagné si l'on est sorti de la boucle avant d'avoir le nombre max de tentatives, ou si il a juste*/
if (arreterPartie){
	sprintf(tampon, "Vous avez gagné en %d tentatives :)\n", nbtentatives-1);
	write(sock_comm, tampon, sizeof(tampon));
	}
	else{
	sprintf(tampon, "Vous avez perdu :(\n");
	write(sock_comm,tampon,sizeof(tampon));
	}
}
 	
/******************************************************************************/	

