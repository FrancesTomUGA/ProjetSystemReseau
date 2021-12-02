#ifndef ENVOICLIENT_H
#define ENVOICLIENT_H

char **recupereListeImagesServeur(int *nbFichier);
char **recupereListeImagesAEnvoyer(char **listeImagesClient, int *nbFichier);
void envoiImages(int socketService, char **listeImagesAEnvoyer, int nbFichier);
void envoiClient(int socketService);

#endif