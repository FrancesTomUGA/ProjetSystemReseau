#ifndef RECEPTIONSERVEUR_C
#define RECEPTIONSERVEUR_C
#include "ReceptionServeur.h"
#include "TransfertServeur.h"
#include "EnvoiServeur.h"
#include "Utils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define FIN_RECEPTION -1
#endif



char **receptionListeImagesServeur(int socketCommClient, int *nbImagesServeur) {
     // Lecture du nombre d'images sur le serveur dans la socket
     while (read(socketCommClient, nbImagesServeur, sizeof(int)) == -1);

     // Lecture des images du serveur et renvoi de la liste
     char **listeImagesServeur = malloc(sizeof(char *) * (*nbImagesServeur));
     int tailleChaine = 0;
     for (int i = 0; i < *nbImagesServeur; i++) {
          while (read(socketCommClient, &tailleChaine, sizeof(int)) == -1);
          listeImagesServeur[i] = malloc(sizeof(char) * tailleChaine + 1);
          read(socketCommClient, listeImagesServeur[i], tailleChaine);
          listeImagesServeur[i][tailleChaine] = '\0';
     }

     return listeImagesServeur;
}


void telechargeImages(int socketCommClient, char **listeImagesATelecharger, int nbFichiers) {
     // Envoi au serveur le nombre de fichiers qu'il va recevoir
     write(socketCommClient, &nbFichiers, sizeof(int));

     for (int j = 0; j < nbFichiers; j++) {
          int longueurChaine = strlen(listeImagesATelecharger[j]);
          write(socketCommClient, &longueurChaine, sizeof(int));
          write(socketCommClient, listeImagesATelecharger[j], longueurChaine);
     }

     int serveurPretEnvoi;
     while (read(socketCommClient, &serveurPretEnvoi, sizeof(int)) == -1);
}


void telechargementServeur(int socketCommClient) {
     int nbImagesATelecharger = 0;
     int code = 2;
     write(socketCommClient, &code, sizeof(int));
     int nbImagesServeur = 0;
     char **listeImagesServeur = receptionListeImagesServeur(socketCommClient, &nbImagesServeur);
     int page = 0; // Page courante
     int action = 0; // Représente le choix fait par l'utilisateur
     char **listeImagesATelecharger = NULL; // Liste des images que le client voudra telecharger

     while (action != FIN_RECEPTION) {
          clear(); // Vide le terminal
          printf("*** Liste des images du serveur ***\n");
          int debut = (page * 4); // Se place sur le premier fichier de la page
          int fin = debut + 4;

          while (debut < nbImagesServeur && debut < fin) {
               printf("[%d] %s\n", debut + 1, listeImagesServeur[debut]);
               debut++;
          }

          printf("\nPage %d\n", page+1);

          printf("\n(1) Page précédente\n(2) Choisir des fichiers télécharger\n(3) Page suivante\n(-1) Retour au menu principal\n");
          action = saisieEntier(); // Demande l'action suivante

          switch (action) {
               case 1:;
                    if (page != 0) {
                         page--;
                    }
                    break;

               case 2:;
                    listeImagesATelecharger = choixImagesATelecharger(listeImagesServeur, nbImagesServeur, &nbImagesATelecharger);
                    envoiListeImagesATelecharger(socketCommClient, listeImagesATelecharger, nbImagesATelecharger);

                    for (int i = 0; i < nbImagesATelecharger; i++) {
                         receptionImage(socketCommClient);
                    }

                    if (nbImagesATelecharger > 0) {
                         action = FIN_RECEPTION;
                         int fini = 1;
                         write(socketCommClient, &fini, sizeof(int));
                    }
                    break;

               case 3:;
                    if (page != (nbImagesServeur / 4)) {
                         page++;
                    }
                    break;

               default:
                    break;
               }
     }

     if (listeImagesServeur != NULL) {
          for (int i = 0; i < nbImagesServeur; i++) {
               free(listeImagesServeur[i]);
          }
          free(listeImagesServeur);
     }

     if (listeImagesATelecharger != NULL) {
          for(int i = 0; i < nbImagesATelecharger; i++) {
               free(listeImagesATelecharger[i]);
          }
          free(listeImagesATelecharger);
     }
}