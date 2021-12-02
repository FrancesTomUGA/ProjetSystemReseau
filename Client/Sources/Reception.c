#ifndef RECEPTION_C
#define RECEPTION_C
#include "Reception.h"
#include "TransfertServeur.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#endif

char **receptionListeImagesServeur(int socketCommClient, int *nbImagesServeur)
{

     while (read(socketCommClient, nbImagesServeur, sizeof(int)) == -1)
          ;

     char **listeImagesServeur = malloc(sizeof(char *) * (*nbImagesServeur));
     int tailleChaine = 0;
     for (int i = 0; i < *nbImagesServeur; i++)
     {
          while (read(socketCommClient, &tailleChaine, sizeof(int)) == -1)
               ;
          printf("taille chaine recu : %d\n", tailleChaine);
          listeImagesServeur[i] = malloc(sizeof(char) * tailleChaine + 1);
          read(socketCommClient, listeImagesServeur[i], tailleChaine);
          listeImagesServeur[i][tailleChaine] = '\0';
          printf("j'ai stocké : %s d'une longueur de %d\n", listeImagesServeur[i], tailleChaine);
     }
     return listeImagesServeur;
}

char **choixImagesATelecharger(char **listeImagesServeur, int *taillelisteImagesServeur, int *nbFichiers, int socketCommClient)
{
     for (int i = 0; i < *taillelisteImagesServeur; i++)
     {
          printf("mot : %s\n", listeImagesServeur[i]);
     }

     int page = 0;                          //Page courante
     int action = 0;                        //Représente le choix fait par l'utilisateur
     char **listeImagesATelecharger = NULL; //liste des images que le client voudra telecharger
     while (action != -1)
     {
          //clear();                //Vide le terminal
          printf("*** Liste des images du serveur ***\n");
          int debut = (page * 4); //Se place sur le premier fichier de la page
          int fin = debut + 4;
          while (debut < *nbFichiers && debut < fin)
          {
               printf("je suis dans la boucle\n");
               printf("[%d] %s\n", debut + 1, listeImagesServeur[debut]);
               debut++;
          }
          printf("\n(1) Page précédente\n(2) Choisir des fichiers télécharger\n(3) Page suivante\n(-1) Retour au menu principal\n");
          scanf("%d", &action); //Demande l'action suivante
          switch (action)
          {
          case 1:
               if (page != 0)
               {
                    page--;
               }
               break;
          case 2:;
               char **listeImagesATelecharger = (char **)malloc(0);
               int numFichier = 0; //Numéro du fichier selectionné
               int tailleListeImagesATelecharger = 0;
               printf("Saisissez le numéro du fichier que vous voulez ajouter (-1 pour terminer) : ");
               scanf("%d", &numFichier);

               while (numFichier != -1)
               {
                    tailleListeImagesATelecharger += 1;
                    listeImagesATelecharger = (char **)realloc(listeImagesATelecharger, sizeof(char *) * (tailleListeImagesATelecharger));
                    listeImagesATelecharger[tailleListeImagesATelecharger - 1] = malloc(sizeof(char) * 100);                //Allocation mémoire pour la nouvelle chaîne
                    strcpy(listeImagesATelecharger[tailleListeImagesATelecharger - 1], listeImagesServeur[numFichier - 1]); //On stocke le texte correspondant au numéro du fichier choisi
                    printf("Saisissez le numéro du fichier que vous voulez ajouter (-1 pour terminer) : ");
                    scanf("%d", &numFichier);
               }
               printf("fin choix images\n");
               write(socketCommClient, &tailleListeImagesATelecharger, sizeof(int));
               printf("dl de %d fichiers\n", tailleListeImagesATelecharger);
               for (int i = 0; i < tailleListeImagesATelecharger; i++)
               {
                    int length = strlen(listeImagesATelecharger[i]);
                    write(socketCommClient, &length, sizeof(int));
                    write(socketCommClient, listeImagesATelecharger[i], strlen(listeImagesATelecharger[i]));
               }

               for (int i = 0; i < tailleListeImagesATelecharger; i++)
               {
                    receptionImage(socketCommClient);
               }
               break;
          case 3:
               if (page != (*nbFichiers / 4))
               {
                    page++;
               }
               break;
          default:
               break;
          }
     }
     printf("Vous-avez choisi de récuperer des fichiers\n");
     return listeImagesATelecharger;
}

void telechargeImages(int socketCommClient, char **listeImagesATelecharger, int nbFichiers)
{
     write(socketCommClient, &nbFichiers, sizeof(int)); //Envoi au serveur le nombre de fichiers qu'il va recevoir

     for (int j = 0; j < nbFichiers; j++)
     {
          int longueurChaine = strlen(listeImagesATelecharger[j]);
          write(socketCommClient, &longueurChaine, sizeof(int));
          write(socketCommClient, listeImagesATelecharger[j], longueurChaine);
     }

     int serveurPretEnvoi;
     while (read(socketCommClient, &serveurPretEnvoi, sizeof(int)) == -1)
          ;
}

void telechargementServeur(int socketCommClient)
{
     int code = 2;
     write(socketCommClient, &code, sizeof(int));

     int nbImagesServeur = 0;
     char **listeImagesServeur = receptionListeImagesServeur(socketCommClient, &nbImagesServeur);
     printf("nb images serveur : %d\n", nbImagesServeur);
     char **listeImagesATelecharger = choixImagesATelecharger(listeImagesServeur, &nbImagesServeur, &nbImagesServeur, socketCommClient);
}