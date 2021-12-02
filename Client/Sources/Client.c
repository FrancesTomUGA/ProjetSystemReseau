#ifndef CLIENT_C
#define CLIENT_C
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include "EnvoiServeur.h"
#include "ReceptionServeur.h"
#include "Utils.h"
#define ENVOI 1
#define RECEPTION 2
#define ARRET 3
#define SORTIE -1
#endif

/**
 * @brief Demande à l'utilisateur ce qu'il souhaite effectuer et renvoie le code d'action correspondant
 * 
 * @return int 
 */
int choixAction()
{
     char temp[10];
     int choix;
     
     do{
          clear();
          printf("***** Que voulez-vous faire ? *****\n1- Déposer des fichiers \n2- Récupérer des fichiers\n3- Quitter\n");
          choix = saisieEntier();
          if (choix != ENVOI && choix != RECEPTION && choix != ARRET){
               choix = SORTIE;
          }
     }while(choix == 0);

     return choix;
}


/**
 * @brief Coeur du programme CLIENT il crée la socket permettant de discuter avec le serveur
 * puis interagi avec l'utilisateur afin de satisfaire ses demandes
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char const *argv[])
{

     if(argc != 3){
          fprintf(stderr, "Erreur usage, 2 paramètres\n");
          exit(-1);
     }


     //Création d'une socket communication client
     int socketCommClient = socket(AF_INET, SOCK_STREAM, 0);
     if (socketCommClient == -1){
          perror("socket()");
          exit(-1);
     }

     //Récupère les informations du serveur grâce au nom de la machine
     struct hostent *infoServeur = gethostbyname(argv[1]);
     if(infoServeur == NULL){
          fprintf(stderr, "Erreur hostname\n");
          exit(-1);
     }
     struct sockaddr_in socketServeur;

     socketServeur.sin_family = AF_INET;
     socketServeur.sin_port = htons(atoi(argv[2]));
     memcpy(&socketServeur.sin_addr.s_addr, infoServeur->h_addr_list[0], infoServeur->h_length); //Affectation à l'aide de memcpy

     printf("Adresse serveur : %s\n", inet_ntoa(socketServeur.sin_addr));

     //Connection du client au serveur
     if (connect(socketCommClient, (struct sockaddr *)&socketServeur, sizeof(socketServeur)) == -1){
          perror("connect()");
          exit(-1);
     }else{
          printf("Connection établie\n");
     }

     int action;
     while ((action = choixAction()) != ARRET) //Tant que l'utilisateur ne souhaite pas arrêter
     {
          clear();
          switch (action){
          case ENVOI:
               envoiServeur(socketCommClient/*, listeFichier, tabFichiersAEnvoyer*/);
               break;
          case RECEPTION:
               telechargementServeur(socketCommClient);
               break;
          case ARRET:
          default:
               break;
          }
          clear();
     }
     
     int sortie = SORTIE;
     write(socketCommClient, &sortie, sizeof(int));
     printf("Fin du programme\n");
     return 0;
}
