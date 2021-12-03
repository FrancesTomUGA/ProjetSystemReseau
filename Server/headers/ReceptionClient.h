#ifndef RECEPTIONCLIENT_H
#define RECEPTIONCLIENT_H



/**
 * @brief Récupère la liste des noms des images que le client souhaite télécharger depuis le serveur
 * Dans un premier temps, récupère le nombre d'images. Ensuite pour chaque image, récupère
 * la taille de son nom et son nom 
 *
 * @param socketService la socket
 * @param nbImagesATelecharger le nombre d'images à télécharger (modifié par cette fonction)
 * @return char** la liste des noms d'images à télécharger
 */
char **recupereListeImagesATelecharger(int socketService, int *nbImagesATelecharger);

#endif