#ifndef ENVOISERVEUR_H
#define ENVOISERVEUR_H

void clear();
void envoiListeImagesATelecharger(int socketCommClient, char **listeImagesATelecharger, int nbImagesATelecharger);
char **choixImagesATelecharger(char **listeImagesServeur, int nbImagesServeur, int *nbImagesATelecharger);
char **recupereListeImagesClient(int *nbFichier);
char **recupereListeImagesAEnvoyer(char **listeImagesClient, int *nbFichier, int nbImageClient);
void envoiImages(int socketCommClient, char **listeImagesAEnvoyer, int nbFichier);
void envoiServeur(int socketCommClient);

#endif