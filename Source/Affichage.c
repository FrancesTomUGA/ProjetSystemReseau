#ifndef AFFICHAGE_C
#define AFFICHAGE_C
#include "Affichage.h"
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

int recupererListeFichier()
{
     int nbFichier = 0;
     struct dirent *lecture;
     DIR *rep; //Permet de stocker les informations du répertoire

     rep = opendir("../Files");
     while ((lecture = readdir(rep))) //Pour chaque fichier trouvé
     {
          //Si c'est bien un fichier (== DT_REG) et que pas un fichier caché (ne commence pas par un .)
          if (lecture->d_type == DT_REG && (lecture->d_name)[0] != '.')
          {
               nbFichier++;
          }
     }

     //Allocation de l'espace mémoire dont on aura besoin pour stocker les chemins par la suite
     listeFichier = (char **)malloc(sizeof(char *) * nbFichier);

     //Boucle identique mais cette fois on enregistre les chemins de fichiers puisque l'on a désormais la place pour le faire
     nbFichier = 0;
     rep = opendir("../Files");
     while ((lecture = readdir(rep)))
     {
          if (lecture->d_type == DT_REG && (lecture->d_name)[0] != '.')
          {
               listeFichier[nbFichier] = lecture->d_name;
               nbFichier++;
          }
     }

     closedir(rep);

     /*printf("Liste des fichiers : \n");
     for (int j = 0; j < nbFichier; j++)
     {
          printf("%d : %s\n", j + 1, listeFichier[j]);
     }*/

     return nbFichier; //Renvoie le nombre de fichier;
}

void selectionEnvoie(int socketCommClient)
{
     tabFichiersAEnvoyer = malloc(sizeof(char *) * 4);
     int numFichier = 0; //Numéro du fichier selectionné
     int i = 0;          //Nombre de fichiers à envoyer

     printf("Saisissez le numéro du fichier que vous voulez ajouter (-1 pour terminer) : ");
     scanf("%d", &numFichier);
     while (i < 4 && numFichier != -1)
     {
          tabFichiersAEnvoyer[i] = malloc(sizeof(char) * strlen(listeFichier[numFichier - 1])); //Allocation mémoire pour la nouvelle chaîne
          strcpy(tabFichiersAEnvoyer[i], listeFichier[numFichier - 1]);                         //On stocke le texte correspondant au numéro du fichier choisi
          i++;
          printf("Saisissez le numéro du fichier que vous voulez ajouter (-1 pour terminer) : ");
          scanf("%d", &numFichier);
     }

     write(socketCommClient, &i, sizeof(int)); //Envoi au serveur le nombre de fichiers qu'il va recevoir

     for (int j = 0; j < i; j++)
     {
          printf("Envoi fichier(s) %d\n", j);
          int tailleChaine = strlen(tabFichiersAEnvoyer[j]);
          write(socketCommClient, &tailleChaine, sizeof(int));                                                              //On envoi d'abord la taille de la chaine pour plus de simplicité
          printf("Taille écrite : %ld\n", write(socketCommClient, tabFichiersAEnvoyer[j], sizeof(char)*tailleChaine)); //Envoi la chaine au serveur
     }
     printf("J'ai fini d'envoyer les fichiers\n"); //Envoi terminé
}

void affichageListeFichier(int socketCommClient, int nbFichier)
{
     int page = 0;   //Page courante
     int action = 0; //Représente le choix fait par l'utilisateur
     while (action != -1)
     {
          //clear();                //Vide le terminal
          printf("*** Liste des fichiers disponibles pour le dépôt ***\n");
          int debut = (page * 4); //Se place sur le premier fichier de la page
          int fin = debut + 4;
          for (debut; debut < fin && debut < nbFichier; debut++)
          {
               printf("[%d] %s\n", debut + 1, listeFichier[debut]);
          }
          printf("\n(1) Page précédente\n(2) Choisir des fichiers à déposer\n(3) Page suivante\n(-1) Retour au menu principal\n");
          scanf("%d", &action); //Demande l'action suivante
          switch (action)
          {
          case 1:
               if (page == 0)
               {
                    page = 1;
               }
               page--;
               break;
          case 2:
               selectionEnvoie(socketCommClient);
               break;
          case 3:
               if (page == (nbFichier / 4))
               {
                    page = (nbFichier / 4) - 1;
               }
               debut++;
               page++;
               break;
          default:
               break;
          }
     }
}

void envoieFichier(int socketCommClient)
{
     int nbFichier = 0;
     nbFichier = recupererListeFichier();
     affichageListeFichier(socketCommClient, nbFichier);
     free(listeFichier);
     free(tabFichiersAEnvoyer);
     printf("Vous-avez choisi d'envoyer des fichiers\n");
}