#ifndef TRANSFERTCLIENT_H
#define TRANSFERTCLIENT_H



/**
 * @brief Envoi une image au client. Envoi d'abord du nom avec chemin de l'image, puis la taille de l'image, et
 * enfin l'image
 * 
 * @param socketTransfert la socket
 * @param nomImage le nom de l'image avec chemin
 */
void envoiImage(int socketTransfert, char *nomImage);


/**
 * @brief Récupère et stocke ou non une image sur le serveur. Lit d'abord le nom avec chemin de l'image,
 * crée un fichier ayant le nom recu (d'abord dans un répertoire temporaire tmp), puis lit sa taille et
 * enfin recopie les données de l'image lue dans celle nouvellement créée.
 * Enfin, vérifie si l'image est admissible (type), si elle l'est la déplace dans le répertoire du serveur,
 * sinon la supprime 
 * 
 * @param socketService la socket
 */
void receptionImage(int socketService);

#endif