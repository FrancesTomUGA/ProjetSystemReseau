#ifndef AFFICHAGE_H
#define AFFICHAGE_H
//pointeur qui permettra a tout le programme de récuperer la liste des fichiers du dossier
char** listeFichier;
//pointeur qui permettra a tout le programme de connaitre la liste des fichiers que l'utilisateur souhaite envoyer
char** tabFichiersAEnvoyer;
/**
 * @brief Efface le terminal pour un affichage propre
 * system("clear")  ou  fork()->exec("clear")?
 */
void clear();
/**
 * @brief Récupération de la liste des fichiers contenus dans le dossier Files
 * Enregistrement des noms de fichier dans le tableau @ listeFichier
 * @return int 
 */
int recupererListeFichier();
/**
 * @brief L'utilisateur sélectionne les images qu'il veux envoyer au serveur dans la liste afficher
 * L'utilisateur peux sélectionnier plusieurs fichiers qui seront ajouter dans le tableau afin d'etre envoyer par la suite
 * @param socketCommClient 
 */
void selectionEnvoie(int socketCommClient);
/**
 * @brief Affiche la liste des fichiers présent dans la listeFichier
 * L'utilisateur peut choisir plusieur option :
 * 1 - il accede a la page suivante (les 4 fichiers précédents)(si c'est la premiere page ne bouge pas)
 * 2 - permet a l'utilisateur de sélectionnier des fichiers dans la liste affichée
 * 3 - il accede a la page précédente (les 4 fichiers suivants)(si c'est la derniere page ne bouge pas)
 * @param socketCommClient le numéro correspondant au déscripteur de la socket
 * @param nbFichier le nombre de fichier contenu dans la listeFichier
 */
void affichageListeFichier(int socketCommClient,int nbFichier);
/**
 * @brief Appelle toute les méthodes permettant le dialogue avec l'utilisateur concernant l'envoie de fichier vers le serveur
 * 
 * @param socketCommClient Le descripteur de fichier correspant a la socket pour communiquer avec le serveur
 */
void envoieFichier(int socketCommClient);
#endif

