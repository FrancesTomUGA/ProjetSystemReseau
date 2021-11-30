#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//test de jeremie
int main(int argc, char const *argv[])
{
     
     //Création d'une socket communication client
     int socketCommClient = socket(AF_INET, SOCK_STREAM, 0);
     if (socketCommClient == -1)
     {
          // fprintf(stderr, "Erreur dans la creation de la socket");
          perror("socket()");
          exit(-1);
     } else{
          printf("Tout va bien création socket\n");
     }

     //récupère les informations du serveur grâce au nom de la machine
     struct hostent *infoServeur = gethostbyname("localhost");
     struct sockaddr_in socketServeur;

     socketServeur.sin_family = AF_INET;
     socketServeur.sin_port = htons(6067);
     memcpy(&socketServeur.sin_addr.s_addr, infoServeur->h_addr_list[0], infoServeur->h_length);//affectation à l'aide de memcpy

     printf("Adresse serveur : %s\n", inet_ntoa(socketServeur.sin_addr));

     //Connection du client au serveur
     if(connect(socketCommClient, (struct sockaddr*) &socketServeur, sizeof(socketServeur)) == -1){
          perror("connect()");
     }else{
          printf("Connection établie\n");
     }

     //Envoi d'un message
     int tampon = 17;
     write(socketCommClient, &tampon, sizeof(int));

     // while(read(socketCommClient, &tampon, sizeof(int)) == -1);
	read(socketCommClient, &tampon, sizeof(int));
     printf("Valeur reçue : %d\n", tampon);

     return 0;
}
