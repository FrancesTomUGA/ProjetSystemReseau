#ifndef ENVOISERVEUR_H
#define ENVOISERVEUR_H



/**
 * @brief envoi le nombre d'image à télécharger au serveur puis, pour chaque image à envoyer
 * envoi la taille de la chaîne suivie de la chaîne
 * 
 * @param socketCommClient la socket de communication avec le serveur
 * @param listeImagesATelecharger le tableau contenant les noms des images que l'on souhaite télécharger
 * @param nbImagesATelecharger le nombre d'image(s) que l'on veut télécharger
 */
void envoiListeImagesATelecharger(int socketCommClient, char **listeImagesATelecharger, int nbImagesATelecharger);


/**
 * @brief permet à l'utilisateur de sélectionner les images qu'il souhaite télécharger
 * 
 * @param listeImagesServeur la liste des images disponibles sur le Serveur
 * @param nbImagesServeur le nombre d'image sur le Serveur
 * @param nbImagesATelecharger pointeur qui permettra de récuperer le nombre d'images qu'a selectionné l'utilisateur (nombre d'image(s) à télécharger)
 * @return char** renvoie le tableau contenant la liste des images à télécharger
 */
char **choixImagesATelecharger(char **listeImagesServeur, int nbImagesServeur, int *nbImagesATelecharger);


/**
 * @brief récupère la liste des images présentes dans le dossier du Client
 * 
 * @param nbFichier pointeur qui permettra de récupérer le nombre d'images total
 * @return char** la liste des images contenues dans le dossier du Client
 */
char **recupereListeImagesClient(int *nbFichier);


/**
 * @brief permet à l'utilisateur de sélectionner les images qu'il souhaite envoyer
 * 
 * @param listeImagesClient la liste des images disponibles dans le dossier du Client
 * @param nbFichier le nombre d'images dans le dossier
 * @param nbImageClient pointeur qui permettra de récuperer le nombre d'images qu'a sélectionné l'utilisateur (nombre d'images à envoyer)
 * @return char** renvoie le tableau contenant la liste des images à envoyer
 */
char **recupereListeImagesAEnvoyer(char **listeImagesClient, int *nbFichier, int nbImageClient);


/**
 * @brief envoi au Serveur le nombre d'images qui vont lui être transferées, puis pour chaque images
 * appelle la fonction envoiImage() qui envoi l'image
 * 
 * @param socketCommClient la socket de communication avec le serveur
 * @param listeImagesAEnvoyer le tableau contenant le nom des images à envoyer au Serveur
 * @param nbFichier le nombre d'images à envoyer au Serveur
 */
void envoiImages(int socketCommClient, char **listeImagesAEnvoyer, int nbFichier);


/**
 * @brief méthode générale qui permet à l'utilisateur de se déplacer entre les différentes pages.
 * Ces pages sont constituées de la liste d'images récuperées dans le dossier du client.
 * Elle permet aussi à l'utilisateur de choisir les images à envoyer (choix 2)
 * et si une sélection a été faite, appelle l'envoi des images en question
 * @param socketCommClient la socket de communication avec le Serveur
 */
void envoiServeur(int socketCommClient);

#endif