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
#include "TransfertClient.h"
#include "EnvoiClient.h"
#include "ReceptionClient.h"
#include <pthread.h>
#include <dirent.h>
#include <sys/time.h>
#include <fcntl.h>
#define ERREUR_READ 0
#define RECEVOIR 1
#define ENVOI 2
#define FIN_CONNEXION -1
/**
 * @brief Traitement effectué lors de la réception du signal SIGCHLD
 *
 * @param s //Signal
 */
void handler()
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
     if (argc != 2)
     {
          fprintf(stderr, "Erreur usage, 1 paramètres\n");
          exit(-1);
     }
     // Définition du comportement pour le traitement du signal SIGCHLD
     struct sigaction ac;
     ac.sa_handler = handler;
     ac.sa_flags = SA_RESTART;

     sigaction(SIGCHLD, &ac, NULL);
     // Création d'une socket d'écoute
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

     // Attachement de la socket à un port
     struct sockaddr_in socketClient;

     socketClient.sin_family = AF_INET;
     socketClient.sin_port = htons(atoi(argv[1]));
     socketClient.sin_addr.s_addr = htonl(INADDR_ANY);

     // paramétrage de la socket pour ne plus avoir l'erreur bind(): adress already in use
     // SO_REUSEADDR permet la réutilisation d'une adresse locale
     // A verifier sur les machines de la fac
     int option_value = 1;
     if (setsockopt(socketEcoute, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(int)) == -1)
     {
          perror("setsockopt()");
          pthread_exit(NULL);
     }

     if (bind(socketEcoute, (struct sockaddr *)&socketClient, sizeof(socketClient)) == -1)
     {
          perror("bind()");
          exit(-1);
     }
     else
     {
          printf("Bind socket\n");
     }

     // Ouverture du service
     // Maximum 5 connexions en attente
     if (listen(socketEcoute, 5) == -1)
     {
          perror("listen()");
          exit(-1);
     }
     else
     {
          printf("Listen socket\n");
     }

     // Acceptation de la connection
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

          // timeout socket
          struct timeval timeout;
          timeout.tv_sec = 5;
          timeout.tv_usec = 0;
          if (setsockopt(socketService, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1)
          {
               perror("setsockopt()");
          }
          if (setsockopt(socketService, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) == -1)
          {
               perror("setsockopt()");
          }

          switch (fork())
          {
          case -1:
               // Erreur
               perror("fork()");
               exit(-1);
          case 0:
               // Comportement du fils
               close(socketEcoute); // Fermeture de la socket d'ecoute du pere

               signal(SIGCHLD, SIG_DFL); // Redéfinition (à défaut) du comportement du signal SIGCHLD pour ne pas hériter de celui du père = réinitialisation

               int action;
               while (read(socketService, &action, sizeof(int)) == -1)
                    ;

               while (action != FIN_CONNEXION)
               {
                    switch (action)
                    {
                    case RECEVOIR:;
                         int nbFichiersALire = 0;
                         while (read(socketService, &nbFichiersALire, sizeof(int)) == -1)
                              ;
     
                         int i = 0;
                         while (i < nbFichiersALire)
                         {
                              printf("\n\n");
                              receptionImage(socketService);
                              i++;
                         }
                         printf("Enregistrement terminé\n");
                         break;
                    case ENVOI:;
                         printf("Début envoi\n");

                         // récupération des images sur le serveur
                         int nbImagesServeur = 0;
                         char **listeImagesServeur = recupereListeImagesServeur(&nbImagesServeur);
                         // envoi du nom des images sur le serveur au client
                         envoiListeImagesServeurClient(socketService, listeImagesServeur, nbImagesServeur);
                         // récupération du nom des images que le client veut télécharger
                         int nbImagesATelecharger = 0;
                         char **listeImagesATelecharger = recupereListeImagesATelecharger(socketService, &nbImagesATelecharger);

                         for (int i = 0; i < nbImagesATelecharger; i++)
                         {
                              envoiImage(socketService, listeImagesATelecharger[i]);
                         }
                         int fini = 0;
                         while (read(socketService, &fini, sizeof(int)) == -1)
                              ;
                         if (listeImagesServeur != NULL)
                         {
                              for(int i = 0; i < nbImagesServeur; i++){
                                   free(listeImagesServeur[i]);
                              }
                              free(listeImagesServeur);
                         }

                         if (listeImagesATelecharger != NULL)
                         {
                              for(int i = 0; i < nbImagesATelecharger; i++){
                                   free(listeImagesATelecharger[i]);
                              }
                              free(listeImagesATelecharger);
                         }
                         break;
                    case FIN_CONNEXION:
                         // fermeture de la socket et mort du fils
                         close(socketService);
                         exit(0);
                         break;
                    }
                    printf("Envoi terminé\n");
                    // Si le client a fermé la connection brutalement, le read renverra 0, et l'action sera mise à FINCONNECTION
                    int size_read_action;
                    while ((size_read_action = read(socketService, &action, sizeof(int))) == -1)
                         ;

                    if (size_read_action == ERREUR_READ)
                    {
                         action = FIN_CONNEXION;
                    }
                    printf("action : %d\n", action);
               }
               printf("Fin de la connexion avec le client\n");
               exit(0);
               break;
          default:
               // Comportement du père
               break;
          }
     }

     return 0;
}
