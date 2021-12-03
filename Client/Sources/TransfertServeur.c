#ifndef TRANSFERTSERVEUR_C
#define TRANSFERTSERVEUR_C
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include "TransfertServeur.h"
#include "Utils.h"
#include <sys/wait.h>
#include <sys/stat.h>
#include <strings.h>
#endif



void envoiImage(int socketCommClient, char *nomImage) {
     int imageLue;
     write(socketCommClient, nomImage, 306); // Envoi du nom de l'image au serveur
     printf("Nom : %s\n",nomImage);

     char cheminImageLue[306];
     bzero(cheminImageLue, 306);
     sprintf(cheminImageLue, "./FilesClient/%s", nomImage);
     imageLue = open(cheminImageLue, O_RDONLY);

     if (imageLue == -1) {
          printf("Erreur lors de l'ouverture de l'image\n");
          exit(-1);
     }

     int imageSize = 0;
     struct stat st;
     if (stat(cheminImageLue, &st) == 0) {
          imageSize = st.st_size; // Récupère la taille de l'image
     }

     write(socketCommClient, &imageSize, sizeof(int));
     
     // Envoi de l'image au serveur
     char chaine[4096];
     int size = 0;
     while ((size = read(imageLue, chaine, sizeof(chaine))) > 0) {
          write(socketCommClient, chaine, size);
          chaine[0] = '\0';
     }

     int sortie;
     while (read(socketCommClient, &sortie, sizeof(int)) == -1);
     printf("Lecture de l'image %s et envoi au serveur terminés\n", nomImage);
}


void receptionImage(int socketCommClient) {
     int imageEcrite;
     // Lecture du nom de l'image
     char *cheminImageTransfert = malloc(sizeof(char) * 306);
     read(socketCommClient, cheminImageTransfert, 306);
     int imageSize;
     read(socketCommClient, &imageSize, sizeof(int));

     // Création de l'image
     switch (fork()) {
          case -1:
               exit(-1);

          case 0:
               execlp("touch", "touch", cheminImageTransfert, (char *)0);

          default:
               wait(NULL);
     }

     imageEcrite = open(cheminImageTransfert, O_WRONLY);

     if (imageEcrite == -1) {
          printf("Erreur lors de l'ouverture de l'image\n");
          exit(-1);
     }

     // Lecture des données de l'image
     char chaine[4096];
     int redSizeTotal = 0;
     int redSize = 0;

     while (redSizeTotal < imageSize) {
          redSize = read(socketCommClient, chaine, sizeof(chaine));
          redSizeTotal += redSize;
          write(imageEcrite, chaine, redSize);
     }

     int fini = 1;
     write(socketCommClient, &fini, sizeof(int));
     printf("Téléchargement de l'image terminé\n");

     if(cheminImageTransfert!=NULL) {
          free(cheminImageTransfert);
     }
}