#ifndef RECEPTIONCLIENT_C
#define RECEPTIONCLIENT_C
#include "TransfertClient.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#endif


char** recupereListeImagesATelecharger(int socketService, int* nbImagesATelecharger)
{
    while (read(socketService, nbImagesATelecharger, sizeof(int)) == -1);
    printf("après recption images à dl, nbfichier = %d\n", *nbImagesATelecharger);
    char **listeImagesATelecharger = malloc(sizeof(char *) * (*nbImagesATelecharger));
    int tailleChaine = 0;
    for (int i = 0; i < *nbImagesATelecharger; i++)
    {
        while (read(socketService, &tailleChaine, sizeof(int)) == -1);
        printf("taille chaine recu : %d\n", tailleChaine);
        listeImagesATelecharger[i] = malloc(sizeof(char) * tailleChaine + 1);
        read(socketService, listeImagesATelecharger[i], tailleChaine);
        listeImagesATelecharger[i][tailleChaine] = '\0';
    }
    printf("fin recupe liste images a telecharger\n");
    return listeImagesATelecharger;
}