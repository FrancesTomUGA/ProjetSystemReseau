#ifndef RECEPTIONSERVEUR_H
#define RECEPTIONSERVEUR_H

char **choixImagesATelecharger(char **listeImagesServeur, int nbImagesServeur, int *nbImagesATelecharger);
void receptionFichier(int socketCommClient);
void telechargementServeur(int socketCommClient);
void telechargeImages(int socketCommClient, char** listeImagesATelecharger, int nbFichiers);
char **receptionListeImagesServeur(int socketCommClient, int* nbImagesServeur);

#endif