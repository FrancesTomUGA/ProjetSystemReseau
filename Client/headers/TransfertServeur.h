#ifndef TRANSFERTSERVEUR_H
#define TRANSFERTSERVEUR_H



/**
 * @brief Envoi au serveur le nom de l'image qu'il va devoir télécharger ainsi que la
 * taille de la l'image
 * tant que le fichier image n'a pas etait entierement lu, envoi le fichier par bloc.
 * Attend un retour du serveur avant de pouvoir terminer.
 * 
 * @param socketCommClient Socket de communication utilisée pour dialoguer avec le serveur
 * @param nomImage le nom de l'image que l'on va envoyer
 */
void envoiImage(int socketCommClient, char *nomImage);


/**
 * @brief lit depuis le serveur le nom de l'image ainsi que la taille de l'image
 * ensuite a l'aide de touch crée un fichier du même nom vide
 * tant que l'intégralité de l'image n'a pas était lu, lit et écrit
 * dans le fichier.
 * Renvoi au serveur un code indiquant que la reception c'est bien passée.
 * 
 * @param socketCommClient Socket de communication utilisée pour dialoguer avec le serveur
 */
void receptionImage(int socketCommClient);

#endif