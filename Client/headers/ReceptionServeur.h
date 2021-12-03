#ifndef RECEPTIONSERVEUR_H
#define RECEPTIONSERVEUR_H

/**
 * @brief 
 * 
 * @param listeImagesServeur 
 * @param nbImagesServeur 
 * @param nbImagesATelecharger 
 * @return char** 
 */
char **choixImagesATelecharger(char **listeImagesServeur, int nbImagesServeur, int *nbImagesATelecharger);
/**
 * @brief 
 * 
 * @param socketCommClient 
 */
void receptionFichier(int socketCommClient);
/**
 * @brief 
 * 
 * @param socketCommClient 
 */
void telechargementServeur(int socketCommClient);
/**
 * @brief 
 * 
 * @param socketCommClient 
 * @param listeImagesATelecharger 
 * @param nbFichiers 
 */
void telechargeImages(int socketCommClient, char **listeImagesATelecharger, int nbFichiers);
/**
 * @brief 
 * 
 * @param socketCommClient 
 * @param nbImagesServeur 
 * @return char** 
 */
char **receptionListeImagesServeur(int socketCommClient, int *nbImagesServeur);

#endif