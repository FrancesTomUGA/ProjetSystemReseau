#ifndef ENVOICLIENT_H
#define ENVOICLIENT_H



/**
 * @brief Récupère la liste des noms des images stockées sur le serveur
 * 
 * @param nbFichier le nombre d'images stockées sur le serveur (modifié par cette fonction)
 * @return char** la liste des noms d'images
 */
char **recupereListeImagesServeur(int *nbFichier);

/**
 * @brief Envoi au client la liste des noms des images stockées sur le serveur
 * Envoi dans un premier temps le nombre d'images stockées, puis pour chaque nom d'image
 * envoi la taille du nom, puis le nom
 * 
 * @param socketService la socket
 * @param listeImagesServeur la liste des noms d'images
 * @param nbImagesServeur le nombre d'images sur le serveur
 */
void envoiListeImagesServeurClient(int socketService, char **listeImagesServeur, int nbImagesServeur);

/**
 * @brief Envoi les images dont le nom est contenu dans la listeImagesAEnvoyer au client.
 * Dans un premier temps, envoi le nombre d'images, puis pour chaque image, envoi le nom avec chemin de l'image,
 * puis sa taille et enfin l'image elle-même
 *
 * @param socketService la socket
 * @param listeImagesAEnvoyer la liste des noms d'images
 * @param nbFichier le nombre d'images à envoyer
 */
void envoiImages(int socketService, char **listeImagesAEnvoyer, int nbFichier);

#endif