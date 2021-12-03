#ifndef CLIENT_H
#define CLIENT_H



/**
 * @brief Affiche à l'écran un menu pour l'utilisateur et lui demande de faire un choix en fonction des propositions
 * 
 * 
 * @return int le choix saisi par l'utilisateur
 */
int choixAction();


/**
 * @brief Coeur du programme CLIENT, il crée la socket permettant de discuter avec le serveur
 * puis interagit avec l'utilisateur afin de satisfaire ses demandes
 * 
 * @param argc Le nombre de paramètres (si différent de 2, erreur)
 * @param argv Liste des arguments (ici le nom du serveur et le port utilisé)
 * @return int
 */
int main(int argc, char const *argv[]);

#endif