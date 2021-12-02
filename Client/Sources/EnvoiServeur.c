#ifndef ENVOISERVEUR_C
#define ENVOISERVEUR_C
#define ENVOI 1
#define FIN_ENVOI -1
#include "EnvoiServeur.h"
#include "TransfertServeur.h"
#include "Utils.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

void clear()
{
     system("clear");
}

void envoiListeImagesATelecharger(int socketCommClient, char **listeImagesATelecharger, int nbImagesATelecharger)
{
     if (nbImagesATelecharger > 0)
     {
          write(socketCommClient, &nbImagesATelecharger, sizeof(int));
          for (int i = 0; i < nbImagesATelecharger; i++)
          {
               int length = strlen(listeImagesATelecharger[i]);
               write(socketCommClient, &length, sizeof(int));
               write(socketCommClient, listeImagesATelecharger[i], strlen(listeImagesATelecharger[i]));
          }
          printf("fin envoi liste images a telecharger\n");
     }
     else
     {
          printf("Vous n'avez pas sélectionné d'image a télécharger\n");
     }
}

char **choixImagesATelecharger(char **listeImagesServeur, int nbImagesServeur, int *nbImagesATelecharger)
{
     char **listeImagesATelecharger = (char **)malloc(0);
     int numFichier = 0; // Numéro du fichier selectionné
     printf("Saisissez le numéro du fichier que vous voulez ajouter (0 pour terminer) : ");
     numFichier = saisieEntier();

     while (numFichier != 0)
     {
          *nbImagesATelecharger += 1;
          listeImagesATelecharger = (char **)realloc(listeImagesATelecharger, sizeof(char *) * (*nbImagesATelecharger));
          listeImagesATelecharger[*nbImagesATelecharger - 1] = malloc(sizeof(char) * 100);                // Allocation mémoire pour la nouvelle chaîne
          strcpy(listeImagesATelecharger[*nbImagesATelecharger - 1], listeImagesServeur[numFichier - 1]); // On stocke le texte correspondant au numéro du fichier choisi
          printf("Saisissez le numéro du fichier que vous voulez ajouter (0 pour terminer) : ");
          numFichier = saisieEntier();
     }
     printf("fin choix images\n");
     return listeImagesATelecharger;
}

char **recupereListeImagesClient(int *nbFichier)
{
     struct dirent *lecture;
     DIR *rep; //Permet de stocker les informations du répertoire

     rep = opendir("./FilesClient");
     if (rep == NULL)
     {
          printf("erreur ouverture repo client\n");
          exit(-1);
     }

     char **listeImagesClient = (char **)malloc(0);
     while ((lecture = readdir(rep))) //Pour chaque fichier trouvé
     {
          //Si c'est bien un fichier (== DT_REG) et que pas un fichier caché (ne commence pas par un .)
          if (lecture->d_type == DT_REG && (lecture->d_name)[0] != '.')
          {
               listeImagesClient = (char **)realloc(listeImagesClient, sizeof(char *) * (*nbFichier + 1));
               listeImagesClient[*nbFichier] = malloc(sizeof(char) * strlen(lecture->d_name));
               listeImagesClient[*nbFichier] = lecture->d_name;
               *nbFichier += 1;
          }
     }
     closedir(rep);
     return listeImagesClient; //Renvoie la liste des fichiers;
}

char **recupereListeImagesAEnvoyer(char **listeImagesClient, int *nbFichier, int nbImageClient)
{
     char **listeImagesAEnvoyer = (char **)malloc(0);
     int numFichier = 0; //Numéro du fichier selectionné
     *nbFichier = 0;     //Nombre de fichiers à envoyer

     printf("Saisissez le numéro du fichier que vous voulez ajouter (0 pour terminer) : ");
     numFichier = saisieEntier();
     while (numFichier > 0 && numFichier <= nbImageClient)
     {
          *nbFichier += 1;
          listeImagesAEnvoyer = (char **)realloc(listeImagesAEnvoyer, sizeof(char *) * (*nbFichier));
          listeImagesAEnvoyer[*nbFichier - 1] = malloc(sizeof(char) * 100);               //Allocation mémoire pour la nouvelle chaîne
          strcpy(listeImagesAEnvoyer[*nbFichier - 1], listeImagesClient[numFichier - 1]); //On stocke le texte correspondant au numéro du fichier choisi
          printf("Saisissez le numéro du fichier que vous voulez ajouter (0 pour terminer) : ");
          numFichier = saisieEntier();
     }

     return listeImagesAEnvoyer;
}

void envoiImages(int socketCommClient, char **listeImagesAEnvoyer, int nbFichier)
{
     if (nbFichier > 0)
     {
          write(socketCommClient, &nbFichier, sizeof(int)); //Envoi au serveur le nombre de fichiers qu'il va recevoir
          for (int j = 0; j < nbFichier; j++)
          {
               envoiImage(socketCommClient, listeImagesAEnvoyer[j]);
          }
          printf("Envoi des fichiers terminé\n"); //Envoi terminé
     }
}

void envoiServeur(int socketCommClient)
{

     int tailleListeImagesClient = 0;
     char **listeImagesClient = recupereListeImagesClient(&tailleListeImagesClient);

     int page = 0;   //Page courante
     int action = 0; //Représente le choix fait par l'utilisateur
     while (action != FIN_ENVOI)
     {
          //clear();                //Vide le terminal
          printf("*** Liste des fichiers disponibles pour le dépôt ***\n");
          int debut = (page * 4); //Se place sur le premier fichier de la page
          int fin = debut + 4;
          while (debut < fin && debut < tailleListeImagesClient)
          {
               printf("[%d] %s\n", debut + 1, listeImagesClient[debut]);
               debut++;
          }
          printf("\n(1) Page précédente\n(2) Choisir des fichiers à déposer\n(3) Page suivante\n(0) Retour au menu principal\n");
          action = saisieEntier(); //Demande l'action suivante
          switch (action)
          {
          case 1:
               if (page != 0)
               {
                    page--;
               }
               break;
          case 2:;
               int tailleListeImagesAEnvoyer = 0;
               char **listeImagesAEnvoyer = recupereListeImagesAEnvoyer(listeImagesClient, &tailleListeImagesAEnvoyer,tailleListeImagesClient);
               if (tailleListeImagesAEnvoyer > 0)
               {
                    int code = ENVOI;
                    write(socketCommClient, &code, sizeof(int));
                    envoiImages(socketCommClient, listeImagesAEnvoyer, tailleListeImagesAEnvoyer);
               }
               else
               {
                    printf("Vous n'avez sélectionné aucune image\n");
               }
               action = FIN_ENVOI;
               break;
          case 3:
               if (page != (tailleListeImagesClient / 4))
               {
                    page++;
               }
               break;
          default:
               break;
          }
     }
     /*clear();
     printf("Téléchargement terminé\n");
     printf("Appuyer sur Entrée pour revenir au menu principal\n");
     char s[30];
     scanf("%s\n",s);*/
}
