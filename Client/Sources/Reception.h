#ifndef RECEPTION_H
#define RECEPTION_H

/**
 * @brief 
 * Pour le moment rien
 */
void receptionFichier(int socketCommClient);
void telechargementServeur(int socketCommClient);
void telechargeImages(int socketCommClient, char** listeImagesATelecharger, int nbFichiers);
char **choixImagesATelecharger(char **listeImagesServeur, int* tailleListeImagesServeur, int *nbFichiers,int socketCommClient);
char **receptionListeImagesServeur(int socketCommClient, int* nbImagesServeur);
#endif