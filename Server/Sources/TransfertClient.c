#ifndef TRANSFERTCLIENT_C
#define TRANSFERTCLIENT_C
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include "TransfertClient.h"
#include <sys/wait.h>
#include <sys/stat.h>
#include "Admis.h"
#endif

void envoiImage(int socketTransfert, char *nomImage)
{
     int imageLue;

     char cheminImageLue[306];
     cheminImageLue[0] = '\0';

     strcat(cheminImageLue, "./FilesServeur/");
     strcat(cheminImageLue, nomImage);

     char cheminImageTransfert[306];
     cheminImageTransfert[0] = '\0';
     strcat(cheminImageTransfert, "./FilesClient/");
     strcat(cheminImageTransfert, nomImage);

     //envoi du nom de l'image au serveur
     write(socketTransfert, cheminImageTransfert, 306);

     imageLue = open(cheminImageLue, O_RDONLY);

     if (imageLue == -1){
          printf("Erreur ouverture image\n");
          exit(-1);
     }

     int imageSize = 0;
     //récupère la taille de l'image
     struct stat st;
     if (stat(cheminImageLue, &st) == 0){
          imageSize = st.st_size;
     }
     
     write(socketTransfert, &imageSize, sizeof(int));
     //envoi de l'image au serveur
     char chaine[4096];
     int size = 0;
     while ((size = read(imageLue, chaine, sizeof(chaine))) > 0)
     {
          write(socketTransfert, chaine, size);
          chaine[0] = '\0';
     }
     int sortie;
     while (read(socketTransfert, &sortie, sizeof(int)) == -1);
     printf("Lecture image et transmission terminées\n");
}

void receptionImage(int socketService)
{
     int imageEcrite;
     //lecture nom image
     char *nomImage = malloc(sizeof(char) * 100);
     nomImage[0]='\0';
     read(socketService, nomImage, 100);
     char cheminTmp[strlen(nomImage)+6];
     cheminTmp[0]='\0';
     strcat(cheminTmp,"./tmp/");
     strcat(cheminTmp,nomImage);
     int imageSize;
     read(socketService, &imageSize, sizeof(int));

     //creation de l'image
     switch (fork()){
     case -1:
          exit(-1);
     case 0:
          execlp("touch", "touch", cheminTmp, (char *)0);
     default:
          wait(NULL);
     }

     imageEcrite = open(cheminTmp, O_WRONLY);

     if (imageEcrite == -1){
          printf("Erreur ouverture image\n");
          exit(-1);
     }

     //lecture des données de l'image
     char chaine[4096];
     int redSizeTotal = 0;
     int redSize = 0;
     while (redSizeTotal < imageSize)
     {
          redSize = read(socketService, chaine, sizeof(chaine));
          redSizeTotal += redSize;
          write(imageEcrite, chaine, redSize);
     }
     int fini = 1;
     admissible(nomImage);
     write(socketService, &fini, sizeof(int));
     printf("Réception données terminée\n");
}