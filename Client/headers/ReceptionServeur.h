#ifndef RECEPTIONSERVEUR_H
#define RECEPTIONSERVEUR_H

/**
 * @brief dans un premier temps récupere la liste des noms d'images du serveur avec receptionListeImagesServeur()
 * puis affiche cette lsite a l'utilisateur de ce balader dans les pages de fichiers.
 * l'utilisateur peut également choisir de télécharger une/des image(s) auquel cas la
 * fonction choixImagesATelecharger() est appeler afin de savoir quelles images on souhaite télécharger.
 * Après l'envoi de la liste si elle existe on télécharge les images.
 * 
 * @param socketCommClient la socket de communication utilisée pour dialoguer avec le serveur
 */
void telechargementServeur(int socketCommClient);


/**
 * @brief envoi au serveur le nombre de nom de fichier qu'il doit lire
 * puis pour chaque nom envoi en premier la taille de la chaine suivi de la chaine en question.
 * A la fin attend le retour du serveur indiquant la reception de tous les noms. 
 * 
 * @param socketCommClient la socket de communication utilisée pour dialoguer avec le serveur
 * @param listeImagesATelecharger la liste contenant le nom des images que l'on souhaite
 * @param nbFichiers le nombre d'image que l'on souhaite télécharger
 */
void telechargeImages(int socketCommClient, char **listeImagesATelecharger, int nbFichiers);


/**
 * @brief récupere du serveur le nombre de nom d'images a recevoir puis pour chaque nom
 * lit en premier la taille de la chaine, avant d'allouer l'espace mémoire permettant ensuite la stocker
 * 
 * @param socketCommClient la socket de communication utilisée pour dialoguer avec le serveur
 * @param nbImagesServeur pointeur qui permet de récuperer le nombre d'images que possède le serveur
 * @return char** 
 */
char **receptionListeImagesServeur(int socketCommClient, int *nbImagesServeur);

#endif