#ifndef ENVOISERVEUR_H
#define ENVOISERVEUR_H
/**
 * @brief Vide le terminal pourque l'affichage suivant soit propre
 * 
 */
void clear();
/**
 * @brief envoi le nombre de image a télécharger au serveur puis pour chaque images a envoyer,
 * envoi la taille de la chaine suivi de la chaine
 * 
 * @param socketCommClient la socket de communication avec le serveur
 * @param listeImagesATelecharger le tableau contenant les noms des images que l'on souhaite télécharger
 * @param nbImagesATelecharger le nombre d'image que l'on veux télécharger
 */
void envoiListeImagesATelecharger(int socketCommClient, char **listeImagesATelecharger, int nbImagesATelecharger);
/**
 * @brief permet a l'utilisateur de selectionner les images qu'il souhaite télécharger
 * 
 * @param listeImagesServeur la liste des images disponibles sur le Serveur
 * @param nbImagesServeur le nombre d'image sur le Serveur
 * @param nbImagesATelecharger pointeur qui permettra de récuperer le nombre d'image qu'a selectionner l'utilisateur(nombre d'image a telecharger)
 * @return char** renvoi le tableau contenant la liste des images a télécharger
 */
char **choixImagesATelecharger(char **listeImagesServeur, int nbImagesServeur, int *nbImagesATelecharger);
/**
 * @brief récupere la liste des images présentes dans le dossier du Client
 * 
 * @param nbFichier pointeur qui permettra de récuperer le nombre de image total
 * @return char** la liste des images contenu dans le dossier du Client
 */
char **recupereListeImagesClient(int *nbFichier);
/**
 * @brief permet a l'utilisateur de selectionner les images qu'il souhaite envoyer
 * 
 * @param listeImagesClient la liste des images disponibles dans le dossier du Client
 * @param nbFichier le nombre d'image dans le dossier
 * @param nbImageClient pointeur qui permettra de récuperer le nombre d'image qu'a selectionner l'utilisateur(nombre d'image a envoyer)
 * @return char** renvoi le tableau contenant la liste des images a envoyer
 */
char **recupereListeImagesAEnvoyer(char **listeImagesClient, int *nbFichier, int nbImageClient);
/**
 * @brief envoi au Serveur le nombre de image qui vont lui etre transferer puis pour chaque images
 * appelle la fonction envoiImage() qui envoi l'image
 * 
 * @param socketCommClient la socket de communication avec le serveur
 * @param listeImagesAEnvoyer le tableau contenant le nom des images a envoyer au Serveur
 * @param nbFichier le nombre d'images a envoyer au Serveur
 */
void envoiImages(int socketCommClient, char **listeImagesAEnvoyer, int nbFichier);
/**
 * @brief méthode générale qui permet a l'utilisateur de ce déplacer entre les differente page
 * ces pages sont constituée de la liste d'image récuperer dans le dossier du client
 * elle permet aussi a l'utilisateur de choisir les images a envoyer(code 2)
 * et si une sélection a etait faite,appelle l'envoi des images en questions
 * @param socketCommClient la socket de communication avec le Serveur
 */
void envoiServeur(int socketCommClient);

#endif