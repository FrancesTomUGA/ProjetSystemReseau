#ifndef RECEPTIONCLIENT_C
#define RECEPTIONCLIENT_C
#include "TransfertClient.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#endif



char **recupereListeImagesATelecharger(int socketService, int *nbImagesATelecharger) {
     while (read(socketService, nbImagesATelecharger, sizeof(int)) == -1);
     char **listeImagesATelecharger = malloc(sizeof(char *) * (*nbImagesATelecharger));
     int tailleChaine = 0;

     for (int i = 0; i < *nbImagesATelecharger; i++) {
          while (read(socketService, &tailleChaine, sizeof(int)) == -1);
          listeImagesATelecharger[i] = malloc(sizeof(char) * tailleChaine + 1);
          read(socketService, listeImagesATelecharger[i], tailleChaine);
          listeImagesATelecharger[i][tailleChaine] = '\0';
     }
     
     return listeImagesATelecharger;
}