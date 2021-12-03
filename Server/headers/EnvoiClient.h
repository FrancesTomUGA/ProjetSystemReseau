#ifndef ENVOICLIENT_H
#define ENVOICLIENT_H

/**
 * @brief 
 * 
 * @param nbFichier 
 * @return char** 
 */
char **recupereListeImagesServeur(int *nbFichier);
/**
 * @brief 
 * 
 * @param socketService 
 * @param listeImagesAEnvoyer 
 * @param nbFichier 
 */
void envoiImages(int socketService, char **listeImagesAEnvoyer, int nbFichier);
/**
 * @brief 
 * 
 * @param socketService 
 * @param listeImagesServeur 
 * @param nbImagesServeur 
 */
void envoiListeImagesServeurClient(int socketService, char **listeImagesServeur, int nbImagesServeur);

#endif