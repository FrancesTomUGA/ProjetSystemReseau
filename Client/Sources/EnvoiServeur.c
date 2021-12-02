#ifndef ENVOISERVEUR_C
#define ENVOISERVEUR_C
#define ENVOI 1
#include "EnvoiServeur.h"
#include "TransfertServeur.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

void clear(){
     system("clear");
}

char** recupereListeImagesClient(int *nbFichier){
     struct dirent *lecture;
     DIR *rep; //Permet de stocker les informations du répertoire

     rep = opendir("./FilesClient");
     if (rep == NULL){
          printf("erreur ouverture repo client\n");
          exit(-1);
     }

     char** listeImagesClient = (char **) malloc(0);
     while ((lecture = readdir(rep))) //Pour chaque fichier trouvé
     {
          //Si c'est bien un fichier (== DT_REG) et que pas un fichier caché (ne commence pas par un .)
          if (lecture->d_type == DT_REG && (lecture->d_name)[0] != '.'){
               listeImagesClient = (char **) realloc(listeImagesClient, sizeof(char*) * (*nbFichier + 1) );
               listeImagesClient[*nbFichier] = malloc(sizeof(char) * strlen(lecture->d_name));
               listeImagesClient[*nbFichier] = lecture->d_name;
               *nbFichier += 1;
          }
     }
     closedir(rep);
     return listeImagesClient; //Renvoie la liste des fichiers;
}

char** recupereListeImagesAEnvoyer(int socketCommClient, char **listeImagesClient, int* nbFichier)
{
     char** listeImagesAEnvoyer = (char **) malloc(0);
     int numFichier = 0; //Numéro du fichier selectionné
     *nbFichier = 0;          //Nombre de fichiers à envoyer

     printf("Saisissez le numéro du fichier que vous voulez ajouter (-1 pour terminer) : ");
     scanf("%d", &numFichier);
     while (numFichier != -1)
     {
          *nbFichier += 1;
          listeImagesAEnvoyer = (char **) realloc(listeImagesAEnvoyer, sizeof(char*) * (*nbFichier));
          listeImagesAEnvoyer[*nbFichier-1] = malloc(sizeof(char) * 100); //Allocation mémoire pour la nouvelle chaîne
          strcpy(listeImagesAEnvoyer[*nbFichier-1], listeImagesClient[numFichier - 1]);                         //On stocke le texte correspondant au numéro du fichier choisi
          printf("Saisissez le numéro du fichier que vous voulez ajouter (-1 pour terminer) : ");
          scanf("%d", &numFichier);
     }

     return listeImagesAEnvoyer;
}

void envoiImages(int socketCommClient, char** listeImagesAEnvoyer, int nbFichier){
     if(nbFichier > 0){ 
          write(socketCommClient, &nbFichier, sizeof(int)); //Envoi au serveur le nombre de fichiers qu'il va recevoir
          for (int j = 0; j < nbFichier; j++){
               envoiImage(socketCommClient, listeImagesAEnvoyer[j]);
          }
          printf("Envoi des fichiers terminé\n"); //Envoi terminé    
     }else{
          printf("Vous n'avez pas selectionne d'image\n");
     }
}

void envoiServeur(int socketCommClient){

     int tailleListeImagesClient = 0;
     char** listeImagesClient = recupereListeImagesClient(&tailleListeImagesClient);

     int page = 0;   //Page courante
     int action = 0; //Représente le choix fait par l'utilisateur
     while (action != -1)
     {
          //clear();                //Vide le terminal
          printf("*** Liste des fichiers disponibles pour le dépôt ***\n");
          int debut = (page * 4); //Se place sur le premier fichier de la page
          int fin = debut + 4;
          while(debut < fin && debut < tailleListeImagesClient)
          {
               printf("[%d] %s\n", debut + 1, listeImagesClient[debut]);
               debut++;
          }
          printf("\n(1) Page précédente\n(2) Choisir des fichiers à déposer\n(3) Page suivante\n(-1) Retour au menu principal\n");
          scanf("%d", &action); //Demande l'action suivante
          switch (action)
          {
          case 1:
               if (page != 0){
                    page--;
               }
               break;
          case 2: ;
               int code = ENVOI;
               write(socketCommClient, &code, sizeof(int));
               int tailleListeImagesAEnvoyer = 0;
               char** listeImagesAEnvoyer = recupereListeImagesAEnvoyer(socketCommClient, listeImagesClient, &tailleListeImagesAEnvoyer);
               envoiImages(socketCommClient, listeImagesAEnvoyer, tailleListeImagesAEnvoyer);
               break;
          case 3:
               if (page != (tailleListeImagesClient / 4)){
                    page++;
               }
               break;
          default:
               break;
          }
     }
}
