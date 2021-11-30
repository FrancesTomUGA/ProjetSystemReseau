#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(int argc, char const *argv[])
{

     //Création d'une socket communication client
     int socketCommClient = socket(AF_INET, SOCK_STREAM, 0);
     if (socketCommClient == -1)
     {
          // fprintf(stderr, "Erreur dans la creation de la socket");
          perror("socket()");
          exit(-1);
     }
     else
     {
          printf("Tout va bien création socket\n");
     }

     //récupère les informations du serveur grâce au nom de la machine
     struct hostent *infoServeur = gethostbyname("localhost");
     struct sockaddr_in socketServeur;

     socketServeur.sin_family = AF_INET;
     socketServeur.sin_port = htons(6067);
     memcpy(&socketServeur.sin_addr.s_addr, infoServeur->h_addr_list[0], infoServeur->h_length); //affectation à l'aide de memcpy

     printf("Adresse serveur : %s\n", inet_ntoa(socketServeur.sin_addr));

     //Connection du client au serveur
     if (connect(socketCommClient, (struct sockaddr *)&socketServeur, sizeof(socketServeur)) == -1)
     {
          perror("connect()");
     }
     else
     {
          printf("Connection établie\n");
     }

     //Envoi d'un message
     int tampon = 17;
     write(socketCommClient, &tampon, sizeof(int));

     // while(read(socketCommClient, &tampon, sizeof(int)) == -1);
     read(socketCommClient, &tampon, sizeof(int));
     printf("Valeur reçue : %d\n", tampon);


     // Création de la boucle d'envoi des fichiers dans le cas où l'utilisateur veut déposer des fichiers sur le serveur
     char** tabFichiersAEnvoyer = malloc(sizeof(char*) * 5); //tableau des numéros de fichiers à envoyer
     char** tabFichiersRepertoireClient = malloc(sizeof(char*) * 5);
     tabFichiersRepertoireClient[0] = malloc(sizeof(char) * 20);
     tabFichiersRepertoireClient[1] = malloc(sizeof(char) * 20);
     tabFichiersRepertoireClient[2] = malloc(sizeof(char) * 20);
     tabFichiersRepertoireClient[3] = malloc(sizeof(char) * 20);
     tabFichiersRepertoireClient[4] = malloc(sizeof(char) * 20);

     tabFichiersAEnvoyer[0] = malloc(sizeof(char) * 20);
     tabFichiersAEnvoyer[1] = malloc(sizeof(char) * 20);
     tabFichiersAEnvoyer[2] = malloc(sizeof(char) * 20);
     tabFichiersAEnvoyer[3] = malloc(sizeof(char) * 20);
     tabFichiersAEnvoyer[4] = malloc(sizeof(char) * 20);

     tabFichiersRepertoireClient[0] = "fleur.jpg";
     tabFichiersRepertoireClient[1] = "arbre.jpg";
     tabFichiersRepertoireClient[2] = "dauphin.jpg";
     tabFichiersRepertoireClient[3] = "tortue.jpg";
     tabFichiersRepertoireClient[4] = "lion.jpg";

     int numFichier;  //numéro de fichier à envoyer choisi par l'utilisateur
     int i = 0;
     printf("Saisissez le numéro du fichier que vous voulez ajouter (-1 pour terminer) : \n");
     scanf("%d\n", &numFichier);
     while (i < 10 && numFichier != -1)
     {
          printf("Numéro de fichier : %d\n", numFichier);
          tabFichiersAEnvoyer[i] = tabFichiersRepertoireClient[numFichier - 1]; //on stocke le numéro du fichier qu'on veut récupérer dans le tableau
          i++;
          printf("Saisissez le numéro du fichier que vous voulez ajouter (-1 pour terminer) : \n");
          scanf("%d\n", &numFichier);
     }

     write(socketCommClient, &i, sizeof(int)); //envoi du nombre de fichiers que va recevoir le serveur au serveur

     for (int j = 0; j < i; j++)
     {
          write(socketCommClient, tabFichiersAEnvoyer[j], sizeof(char*)); //envoi des fichiers au serveur
     }
     printf("J'ai fini d'envoyer les fichiers\n");

     return 0;
}
