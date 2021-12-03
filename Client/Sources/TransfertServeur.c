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



void envoiImage(int socketTransfert, char *nomImage) {
     int imageLue;
     write(socketTransfert, nomImage, 306); // Envoi du nom de l'image au serveur
     printf("Nom : %s\n",nomImage);

     char cheminImageLue[306];
     bzero(cheminImageLue, 306);
     sprintf(cheminImageLue, "./FilesClient/%s", nomImage);
     printf("CheminImageLue : %s\n", cheminImageLue);
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

     write(socketTransfert, &imageSize, sizeof(int));
     
     // Envoi de l'image au serveur
     char chaine[4096];
     int size = 0;
     while ((size = read(imageLue, chaine, sizeof(chaine))) > 0) {
          write(socketTransfert, chaine, size);
          chaine[0] = '\0';
     }

     int sortie;
     while (read(socketTransfert, &sortie, sizeof(int)) == -1);
     printf("Lecture de l'image et transmission terminées\n");
}


void receptionImage(int socketService) {
     int imageEcrite;
     // Lecture du nom de l'image
     char *cheminImageTransfert = malloc(sizeof(char) * 306);
     read(socketService, cheminImageTransfert, 306);
     int imageSize;
     read(socketService, &imageSize, sizeof(int));

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
          redSize = read(socketService, chaine, sizeof(chaine));
          redSizeTotal += redSize;
          write(imageEcrite, chaine, redSize);
     }

     int fini = 1;
     write(socketService, &fini, sizeof(int));
     printf("Réception des données terminée\n");

     if(cheminImageTransfert!=NULL) {
          free(cheminImageTransfert);
     }
}