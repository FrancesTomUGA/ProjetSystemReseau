#ifndef RECEPTION_C
#define RECEPTION_C
#include "Reception.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#endif

void receptionFichier(int socketCommClient)
{
     int code = 2;

     write(socketCommClient, &code, sizeof(int));
     int nbFichier = 0;
     while (read(socketCommClient, &nbFichier, sizeof(int)) == -1);
     
     char **laListeDesFichiers = malloc(sizeof(char *) * nbFichier);
     int tailleChaine = 0;
     for (int i = 0; i < nbFichier; i++)
     {

          while (read(socketCommClient, &tailleChaine, sizeof(int)) == -1)
               ;

          laListeDesFichiers[i] = malloc(sizeof(char) * tailleChaine);
          printf("j'ai a vide : %s\n", laListeDesFichiers[i]);
          read(socketCommClient, laListeDesFichiers[i], tailleChaine);
          printf("j'ai stocker : %s d'une longueur de %d\n", laListeDesFichiers[i], tailleChaine);
     }

     int page = 0;   //Page courante
     int action = 0; //Représente le choix fait par l'utilisateur
     while (action != -1)
     {
          //clear();                //Vide le terminal
          printf("*** Liste des fichiers disponibles pour le dépôt ***\n");
          int debut = (page * 4); //Se place sur le premier fichier de la page
          int fin = debut + 4;
          while (debut < fin && debut < nbFichier)
          {
               printf("[%d] %s\n", debut + 1, laListeDesFichiers[debut]);
               debut++;
          }
          printf("\n(1) Page précédente\n(2) Choisir des fichiers à déposer\n(3) Page suivante\n(-1) Retour au menu principal\n");
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
               char** tabFichiersEnvoi = (char **)malloc(sizeof(char *) * 4);
               int numFichier = 0; //Numéro du fichier selectionné
               int i = 0;          //Nombre de fichiers à envoyer

               printf("Saisissez le numéro du fichier que vous voulez ajouter (-1 pour terminer) : ");
               scanf("%d", &numFichier);
               while (i < 4 && numFichier != -1)
               {
                    tabFichiersEnvoi[i] = malloc(sizeof(char) * strlen(laListeDesFichiers[numFichier - 1])); //Allocation mémoire pour la nouvelle chaîne
                    strcpy(tabFichiersEnvoi[i], laListeDesFichiers[numFichier - 1]);                         //On stocke le texte correspondant au numéro du fichier choisi
                    i++;
                    printf("Saisissez le numéro du fichier que vous voulez ajouter (-1 pour terminer) : ");
                    scanf("%d", &numFichier);
               }

               write(socketCommClient, &i, sizeof(int)); //Envoi au serveur le nombre de fichiers qu'il va recevoir

               for (int j = 0; j < i; j++)
               {    
                    int longueurChaine = strlen(tabFichiersEnvoi[j]);
                    write(socketCommClient,&longueurChaine,sizeof(int));
                    write(socketCommClient,tabFichiersEnvoi[j],longueurChaine);
               }
               break;
          case 3:
               if (page != (nbFichier / 4))
               {
                    page++;
                    //page = (nbFichier / 4) - 1;
               }
               //debut++;

               break;
          default:
               break;
          }
     }
     printf("Vous-avez choisi de récuperer des fichiers\n");
}