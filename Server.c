#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

/**
 * @brief Fonction main du programme
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char const *argv[])
{

     //Création d'une socket d'écoute
     int socketEcoute = socket(AF_INET, SOCK_STREAM, 0);
     if (socketEcoute == -1)
     {
          perror("socket()");
          exit(-1);
     }else{
          printf("Tout va bien création socket\n");
     }

     //Attachement de la socket à un port
     struct sockaddr_in socketClient;   
     
     // pointeur structure : (->) pour accès aux champs
     socketClient.sin_family = AF_INET;
     socketClient.sin_port = htons(6067);
     socketClient.sin_addr.s_addr = htonl(INADDR_ANY);
     
     if(bind(socketEcoute, (struct sockaddr*) &socketClient, sizeof(socketClient)) == -1){
          perror("bind()");
          exit(-1);
     }else{
          printf("Tout va bien attachement socket\n");
     }

     //Ouverture du service
     //Maximum 5 connexions en attente
     if(listen(socketEcoute, 5) == -1){
          perror("listen()");
          exit(-1);
     }else{
          printf("Tout va bien ouverture\n");
     }
     
     int len = sizeof(socketClient);
     while (1){
          
          if(accept(socketEcoute, (struct sockaddr*) &socketClient, &len) == -1){
               perror("accept()");
               exit(-1);
          }else{
               printf("Accepte connection ok\n"); // Ici on a eu aucun affichage car on avait pas mit de \n
                                                  // penser à en mettre pour flush le buffer et afficher sur la sortie standard
          }
          
          //printf("taille de res accept %d\n", accept(socketEcoute, (struct sockaddr*) &socketClient, &len));
     }

     return 0;
}
