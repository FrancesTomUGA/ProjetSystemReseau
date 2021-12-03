#ifndef CLIENT_H
#define CLIENT_H
/**
 * @brief Demande à l'utilisateur ce qu'il souhaite effectuer et renvoie le code d'action correspondant
 * 
 * @return int <- le choix fait par l'utilisatuer
 */
int choixAction();
/**
 * @brief Coeur du programme CLIENT il crée la socket permettant de discuter avec le serveur
 * puis interagi avec l'utilisateur afin de satisfaire ses demandes
 * 
 * @param argc <- le nombre de parametre (si différent de 2 erreur)
 * @param argv <- liste des arguments (ici le nom du serveur ou l'ip et le port utilisé)
 * @return int 
 */
int main(int argc, char const *argv[]);
#endif