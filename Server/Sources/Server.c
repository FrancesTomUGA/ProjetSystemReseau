#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "Transfert.h"
#include <pthread.h>
#include <dirent.h>
#define RECEVOIR 1
#define ENVOYER 2
#define FINCONNEXION -1
/**
 * @brief Traitement effectué lors de la réception du signal SIGCHLD
 * 
 * @param s //Signal
 */
void handler(int s)
{
     wait(NULL);
}

/**
 * @brief Fonction main du programme
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char const *argv[])
{
     //Définition du comportement pour le traitement du signal SIGCHLD
     struct sigaction ac;
     ac.sa_handler = handler;
     ac.sa_flags = SA_RESTART;

     //Création d'une socket d'écoute
     int socketEcoute = socket(AF_INET, SOCK_STREAM, 0);
     if (socketEcoute == -1)
     {
          perror("socket()");
          exit(-1);
     }
     else
     {
          printf("Tout va bien création socket\n");
     }

     //Attachement de la socket à un port
     struct sockaddr_in socketClient;

     socketClient.sin_family = AF_INET; //Pointeur structure : (->) pour accès aux champs
     socketClient.sin_port = htons(6067);
     socketClient.sin_addr.s_addr = htonl(INADDR_ANY);
     /*int sockfd = 0;
     int yes = 1;
     if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
     {
          perror("setsockopt");
          pthread_exit(NULL);
     }
     */
     if (bind(socketEcoute, (struct sockaddr *)&socketClient, sizeof(socketClient)) == -1)
     {
          perror("bind()");
          exit(-1);
     }
     else
     {
          printf("Tout va bien attachement socket\n");
     }

     //Ouverture du service
     //Maximum 5 connexions en attente
     if (listen(socketEcoute, 5) == -1)
     {
          perror("listen()");
          exit(-1);
     }
     else
     {
          printf("Tout va bien ouverture\n");
     }

     //Acceptation de la connection
     int len = sizeof(socketClient);
     while (1)
     {
          int socketService;
          if ((socketService = accept(socketEcoute, (struct sockaddr *)&socketClient, &len)) == -1)
          { /* /!\ L'appel de la fonction accept est bloquant /!\ */
               perror("accept()");
               exit(-1);
          }
          else
          {
               printf("Accepte connection ok\n"); // Ici on a eu aucun affichage car on avait pas mit de \n
                                                  // penser à en mettre pour flush le buffer et afficher sur la sortie standard
          }

          switch (fork())
          {
          case -1:
               //Erreur
               perror("fork()");
               exit(-1);
               break;

          case 0:
               //Comportement du fils
               close(socketEcoute); //Fermeture du descripteur de fichier

               signal(SIGCHLD, SIG_DFL); //Redéfinition (à défaut) du comportement du signal SIGCHLD pour ne pas hériter de celui du père = réinitialisation

               int action;
               while (read(socketService, &action, sizeof(int)) == -1)
                    ;
               while (action != FINCONNEXION)
               {
                    switch (action)
                    {
                    case RECEVOIR:;
                         int nbFichiersALire = 0;
                         while (read(socketService, &nbFichiersALire, sizeof(int)) == -1)
                              ;
                         printf("Nb de fichiers à lire : %d\n", nbFichiersALire);

                         int i = 0;
                         while (i < nbFichiersALire)
                         {
                              receptionImageServeur(socketService);
                              i++;
                         }
                         printf("Enregistrement terminé\n");
                         break;
                    case ENVOYER:;
                         printf("On va envoyer des fichier au client\n");
                         struct dirent *lecture;
                         DIR *rep; //Permet de stocker les informations du répertoire

                         rep = opendir("./FilesServeur");
                         if (rep == NULL)
                         {
                              printf("erreur ouverture rep\n");
                              exit(-1);
                         }
                         int nbFichier = 0;
                         while ((lecture = readdir(rep))) //Pour chaque fichier trouvé
                         {

                              //Si c'est bien un fichier (== DT_REG) et que pas un fichier caché (ne commence pas par un .)
                              if (lecture->d_type == DT_REG && (lecture->d_name)[0] != '.')
                              {
                                   nbFichier = nbFichier + 1;
                              }
                         }
                         //Allocation de l'espace mémoire dont on aura besoin pour stocker les chemins par la suite
                         char **laListeDesFichiers = (char **)malloc(sizeof(char *) * (nbFichier));

                         //Boucle identique mais cette fois on enregistre les chemins de fichiers puisque l'on a désormais la place pour le faire
                         nbFichier = 0;
                         rep = opendir("./FilesServeur");
                         while ((lecture = readdir(rep)))
                         {
                              if (lecture->d_type == DT_REG && (lecture->d_name)[0] != '.')
                              {
                                   laListeDesFichiers[nbFichier] = malloc(sizeof(char) * strlen(lecture->d_name));
                                   //printf("dname = : %s\n",lecture->d_name);
                                   laListeDesFichiers[nbFichier] = lecture->d_name;
                                   //printf("resultat = : %s\n",laListeDesFichiers[nbFichier]);
                                   nbFichier = nbFichier + 1;
                              }
                         }
                         closedir(rep);
                         printf("Nb de fichiers à lire : %d\n", nbFichier);
                         write(socketService, &nbFichier, sizeof(int));
                         for (int i = 0; i < nbFichier; i++)
                         {
                              int longueur = strlen(laListeDesFichiers[i]);
                              printf("La longueur de %s est de %d\n", laListeDesFichiers[i], longueur);
                              write(socketService, &longueur, sizeof(int));
                              write(socketService, laListeDesFichiers[i], strlen(laListeDesFichiers[i]));
                         }
                         int fichierAEnvoyer;

                         while (read(socketService, &fichierAEnvoyer, sizeof(int)) == -1)
                              ;
                         char **listeFicAEnvoyer = malloc(sizeof(char *) * fichierAEnvoyer);
                         for (int i = 0; i < fichierAEnvoyer; i++)
                         {
                              int taille = 0;
                              read(socketService,&taille,sizeof(int));
                              listeFicAEnvoyer[i]= malloc(sizeof(char)*taille);
                              read(socketService,listeFicAEnvoyer[i],taille);
                              printf("le client veux telecharger : %s\n",listeFicAEnvoyer[i]);
                         }
                         break;
                    case FINCONNEXION:
                         break;
                    }
                    while (read(socketService, &action, sizeof(int)) == -1)
                         ;
               }
               printf("Fin de la connexion avec le client\n");
               exit(0);
               break;
          default:
               //Comportement du père
               break;
          }
     }

     return 0;
}
