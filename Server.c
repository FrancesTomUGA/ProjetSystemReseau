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
     struct sockaddr_in sock_adresse;   
     
     // pointeur structure : (->) pour accès aux champs
     sock_adresse.sin_family = AF_INET;
     sock_adresse.sin_port = htons(6067);
     sock_adresse.sin_addr.s_addr = htonl(INADDR_ANY);
     
     if(bind(socketEcoute, (struct sockaddr*) &sock_adresse, sizeof(sock_adresse)) == -1){
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
     
     

     return 0;
}
