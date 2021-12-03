#ifndef ENVOICLIENT_C
#define ENVOICLIENT_C
#define ENVOI 1
#include "EnvoiClient.h"
#include "TransfertClient.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#endif



void clear() {
     system("clear");
}


char **recupereListeImagesServeur(int *nbFichier) {
     struct dirent *lecture;
     DIR *rep; // Permet de stocker les informations du répertoire
     rep = opendir("./FilesServeur");

     if (rep == NULL) {
          printf("erreur ouverture repo serveur\n");
          exit(-1);
     }

     char **listeImagesServeur = (char **) malloc(0);
     while ((lecture = readdir(rep))) { // Pour chaque fichier trouvé
          // Si c'est bien un fichier (== DT_REG) et que ce n'est pas un fichier caché (ne commence pas par un .)
          if (lecture->d_type == DT_REG && (lecture->d_name)[0] != '.') {
               listeImagesServeur = (char **)realloc(listeImagesServeur, sizeof(char *) * (*nbFichier + 1));
               listeImagesServeur[*nbFichier] = malloc(sizeof(char) * strlen(lecture->d_name));
               listeImagesServeur[*nbFichier] = lecture->d_name;
               *nbFichier += 1;
          }
     }
     closedir(rep);

     return listeImagesServeur; // Renvoi la liste des fichiers
}


void envoiListeImagesServeurClient(int socketService, char **listeImagesServeur, int nbImagesServeur) {
     write(socketService, &nbImagesServeur, sizeof(int));

     for (int i = 0; i < nbImagesServeur; i++) {
          int length = strlen(listeImagesServeur[i]);
          write(socketService, &length, sizeof(int));
          write(socketService, listeImagesServeur[i], length);
     }
}


void envoiImages(int socketService, char **listeImagesAEnvoyer, int nbFichier) {
     if (nbFichier > 0) {
          write(socketService, &nbFichier, sizeof(int)); // Envoi au serveur le nombre de fichiers qu'il va recevoir
          for (int j = 0; j < nbFichier; j++) {
               envoiImage(socketService, listeImagesAEnvoyer[j]);
          }
          printf("Envoi des images terminé\n");
     }
     else {
          printf("Vous n'avez pas selectionné d'image\n");
     }
}