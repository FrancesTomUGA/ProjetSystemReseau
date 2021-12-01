#ifndef AFFICHAGE_H
#define AFFICHAGE_H



/**
 * @brief Efface le terminal pour un affichage propre
 * system("clear")  ou  fork()->exec("clear")?
 */
void clear();

/**
 * @brief Récupération de la liste des fichiers contenus dans le dossier Files
 * Enregistrement des noms de fichier dans le tableau @ listeFichier
 * @param listeFIchier un tableau de chaines de caractères contenant la liste de tous les fichiers dans le répertoire d'images
 * @return int 
 */
int recupererListeFichier(char** listeFichier);

/**
 * @brief L'utilisateur sélectionne les images qu'il veux envoyer au serveur dans la liste afficher
 * L'utilisateur peux sélectionnier plusieurs fichiers qui seront ajouter dans le tableau afin d'etre envoyer par la suite
 * @param socketCommClient 
 */
void selectionEnvoie(int socketCommClient, char** listeFichier, char** tabFichiersAEnvoyer);

/**
 * @brief Affiche la liste des fichiers présent dans la listeFichier
 * L'utilisateur peut choisir plusieur option :
 * 1 - il accede a la page suivante (les 4 fichiers précédents)(si c'est la premiere page ne bouge pas)
 * 2 - permet a l'utilisateur de sélectionnier des fichiers dans la liste affichée
 * 3 - il accede a la page précédente (les 4 fichiers suivants)(si c'est la derniere page ne bouge pas)
 * @param socketCommClient le numéro correspondant au déscripteur de la socket
 * @param nbFichier le nombre de fichier contenu dans la listeFichier
 * @param listeFIchier un tableau de chaines de caractères contenant la liste de tous les fichiers dans le répertoire d'images
 * @param tabFichiersAEnvoyer un tableau de chaines de caractères contenant la liste des fichiers à envoyer
 */
void affichageListeFichier(int socketCommClient, int nbFichier, char** listeFichier, char** tabFichiersAEnvoyer);

/**
 * @brief Appelle toute les méthodes permettant le dialogue avec l'utilisateur concernant l'envoie de fichier vers le serveur
 * 
 * @param socketCommClient Le descripteur de fichier correspant a la socket pour communiquer avec le serveur
 * @param listeFIchier un tableau de chaines de caractères contenant la liste de tous les fichiers dans le répertoire d'images
 * @param tabFichiersAEnvoyer un tableau de chaines de caractères contenant la liste des fichiers à envoyer
 */
void envoieFichier(int socketCommClient, char** listeFichier, char** tabFichiersAEnvoyer);

#endif