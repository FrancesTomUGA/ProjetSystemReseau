#ifndef ENVOICLIENT_H
#define ENVOICLIENT_H

char **recupereListeImagesServeur(int *nbFichier);
void envoiImages(int socketService, char **listeImagesAEnvoyer, int nbFichier);
void envoiListeImagesServeurClient(int socketService, char **listeImagesServeur, int nbImagesServeur);

#endif