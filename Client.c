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
#include <dirent.h>
#include <assert.h>

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

     int nbFichier = 0;

     struct dirent *lecture;
     DIR *rep; //permet de stocker les informations du répertoire
     rep = opendir("./Files");
     while ((lecture = readdir(rep)))
     {
          if (lecture->d_type == DT_REG && (lecture->d_name)[0] != '.')
          {
               nbFichier++;
          }
     }
     char **listeFichier = (char **)malloc(sizeof(char *) * nbFichier);

     nbFichier = 0;
     rep = opendir("./Files");
     while ((lecture = readdir(rep)))
     {
          if (lecture->d_type == DT_REG && (lecture->d_name)[0] != '.')
          {
               listeFichier[nbFichier] = lecture->d_name;
               nbFichier++;
          }
     }
     //{}
     //\n
     //[]
     closedir(rep);
     printf("Liste des fichiers : \n");
     for (int j = 0; j < nbFichier; j++)
     {
          printf("%d : %s\n", j + 1, listeFichier[j]);
     }

     // Création de la boucle d'envoi des fichiers dans le cas où l'utilisateur veut déposer des fichiers sur le serveur
     char **tabFichiersAEnvoyer = malloc(sizeof(char *) * 5); //tableau des numéros de fichiers à envoyer

     int numFichier = 0; //numéro de fichier à envoyer choisi par l'utilisateur
     int i = 0;
     printf("Saisissez le numéro du fichier que vous voulez ajouter (-1 pour terminer) : ");
     scanf("%d", &numFichier);
     while (i < 10 && numFichier != -1)
     {
          tabFichiersAEnvoyer[i] = malloc(sizeof(char) * strlen(listeFichier[numFichier - 1]));
          strcpy(tabFichiersAEnvoyer[i], listeFichier[numFichier - 1]); //on stocke le numéro du fichier qu'on veut récupérer dans le tableau
          i++;
          printf("Saisissez le numéro du fichier que vous voulez ajouter (-1 pour terminer) : ");
          scanf("%d", &numFichier);
     }

     write(socketCommClient, &i, sizeof(int)); //envoi du nombre de fichiers que va recevoir le serveur au serveur

     for (int j = 0; j < i; j++)
     {
          printf("envoi fichier %d\n", j);
          int tailleChaine = strlen(tabFichiersAEnvoyer[j]);
          write(socketCommClient, &tailleChaine, sizeof(int));
          printf("taille ecrite : %ld\n", write(socketCommClient, tabFichiersAEnvoyer[j], strlen(tabFichiersAEnvoyer[j]))); //envoi des fichiers au serveur
     }
     printf("J'ai fini d'envoyer les fichiers\n");

     return 0;
}

void clear()
{
     system("clear");
}