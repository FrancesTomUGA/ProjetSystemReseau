#ifndef TRANSFERT_C
#define TRANSFERT_C
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include "Transfert.h"
#include <sys/wait.h>
#include <sys/stat.h>
#endif

void envoiImageClientServeur(char *nomImage, int socketTransfert)
{
     int imageLue;

     printf("nom image recu1 : %s\n", nomImage);
     char cheminImageLue[306];
     cheminImageLue[0] = '\0';

     printf("nom image recu2 : %s\n", cheminImageLue);
     strcat(cheminImageLue, "./FilesClient/");
     printf("nom image recu3 : %s\n", cheminImageLue);
     strcat(cheminImageLue, nomImage);
     printf("chemin image lue: %s\n", cheminImageLue);

     char cheminImageTransfert[306];
     cheminImageTransfert[0] = '\0';
     strcat(cheminImageTransfert, "./FilesServeur/");
     strcat(cheminImageTransfert, nomImage);
     printf("chemin image transfert: %s\n", cheminImageTransfert);

     //envoi du nom de l'image au serveur
     write(socketTransfert, cheminImageTransfert, 306);

     imageLue = open(cheminImageLue, O_RDONLY);

     if (imageLue == -1)
     {
          printf("Erreur ouverture image\n");
          exit(-1);
     }

     int imageSize = 0;
     //récupère la taille de l'image
     struct stat st;
     if (stat(cheminImageLue, &st) == 0)
     {
          imageSize = st.st_size;
     }
     else
     {
          printf("taille introuvable\n");
     }
     write(socketTransfert, &imageSize, sizeof(int));
     //envoi de l'image au serveur
     char chaine[512];
     int nbPaquet = 0;
     int size = 0;
     while ((size = read(imageLue, chaine, 512)) > 0)
     {
          //printf("envoie données image %s\n", nomImage);
          write(socketTransfert, chaine, size);
          chaine[0] = '\0';
          nbPaquet++;
     }
     int sortie;
     while (read(socketTransfert, &sortie, sizeof(int)) == -1)
          ;
     printf("j'ai envoyer %d paquet\n", nbPaquet);
     printf("Lecture image et transmission terminées\n");
}

void receptionImageServeur(int socketService)
{
     int imageEcrite;
     //lecture nom image
     char *cheminImageTransfert = malloc(sizeof(char) * 306);
     read(socketService, cheminImageTransfert, 306);
     printf("j'ai recu ca : %s\n", cheminImageTransfert);
     int imageSize;
     read(socketService, &imageSize, sizeof(int));

     //creation de l'image
     switch (fork())
     {
     case -1:
          exit(-1);
     case 0:
          execlp("touch", "touch", cheminImageTransfert, (char *)0);
     default:
          wait(NULL);
     }

     imageEcrite = open(cheminImageTransfert, O_WRONLY);

     if (imageEcrite == -1)
     {
          printf("Erreur ouverture image\n");
          exit(-1);
     }

     //lecture des données de l'image
     char chaine[512];
     int indice = 0;
     int redSizeTotal = 0;
     int redSize = 0;
     while (redSizeTotal < imageSize)
     {

          redSize = read(socketService, chaine, 512);
          redSizeTotal += redSize;
          printf("réception données image %s\n", cheminImageTransfert);
          write(imageEcrite, chaine, redSize);
          printf("taille lu : %d et taille de l'image : %d\n", redSizeTotal, imageSize);
     }
     int fini = 1;
     write(socketService, &fini, sizeof(int));
     printf("Réception données terminée\n");
}