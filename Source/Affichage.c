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

int recupererListeFichier(char** listeFichier)
{
     int nbFichier = 0;

     struct dirent *lecture;
     DIR *rep; //permet de stocker les informations du répertoire
     rep = opendir("./Files");
     while ((lecture = readdir(rep))) //Pour chaque fichiers trouver
     {
          //Si cest bien un fichier (== DT_REG) et que pas un fichier cacher(Commence pas par .)
          if (lecture->d_type == DT_REG && (lecture->d_name)[0] != '.')
          {
               nbFichier++;
          }
     }
     //Allocation de l'espace mémoire dont on aura besoin pour stocker les chemins par la suite
     listeFichier = (char **)malloc(sizeof(char *) * nbFichier);

     //Boucle identique mais cette fois on enregistres les chemins de fichiers puisque l'on a désormais la place
     nbFichier = 0;
     rep = opendir("./Files");
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
     return nbFichier; //renvoie le nombre de fichier;
}

void selectionEnvoie(int socketCommClient, char** listeFichier, char** tabFichiersAEnvoyer)
{
     tabFichiersAEnvoyer = malloc(sizeof(char *) * 4);

     int numFichier = 0; //Numero Du fichier selectionner
     int i = 0;          //Nombre de fichier a envoyer
     printf("Saisissez le numéro du fichier que vous voulez ajouter (-1 pour terminer) : ");
     scanf("%d", &numFichier);
     while (i < 4 && numFichier != -1)
     {
          tabFichiersAEnvoyer[i] = malloc(sizeof(char) * strlen(listeFichier[numFichier - 1])); //Allocation memoire pour la nouvelle chaine
          strcpy(tabFichiersAEnvoyer[i], listeFichier[numFichier - 1]);                         //on stocke le text correspondant au numéro choisit
          i++;
          printf("Saisissez le numéro du fichier que vous voulez ajouter (-1 pour terminer) : ");
          scanf("%d", &numFichier);
     }

     write(socketCommClient, &i, sizeof(int)); //envoi au serveur le nombre de fichiers a recevoir

     for (int j = 0; j < i; j++)
     {
          printf("envoi fichier %d\n", j);
          int tailleChaine = strlen(tabFichiersAEnvoyer[j]);
          write(socketCommClient, &tailleChaine, sizeof(int));                                                              //on envoi d'abord la taille de la chaine pour plus de simplicitée
          printf("taille ecrite : %ld\n", write(socketCommClient, tabFichiersAEnvoyer[j], strlen(tabFichiersAEnvoyer[j]))); //envoi la chaine au serveur
     }
     printf("J'ai fini d'envoyer les fichiers\n"); //Envoi fini
}

void affichageListeFichier(int socketCommClient, int nbFichier, char** listeFichier, char** tabFichiersAEnvoyer)
{

     int page = 0;   //page courante
     int action = 0; //represente le choix fait par l'utilisateur
     while (action != -1)
     {
          clear();                //vide le terminal
          int debut = (page * 4); //ce place sur le premier fichier de la page
          for (debut; debut < debut + 4 && debut < nbFichier; debut++)
          {
               printf("%d . %s\n", debut, listeFichier[debut]);
          }
          printf("1.page précédente\n2.selectionner des fichiers dans la page\n3.page suivante\n-1.Retour au menu(Default)\n");
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
               selectionEnvoie(socketCommClient, listeFichier, tabFichiersAEnvoyer);
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

void envoieFichier(int socketCommClient, char** listeFichier, char** tabFichiersAEnvoyer)
{
     int nbFichier = 0;
     nbFichier = recupererListeFichier(listeFichier);
     affichageListeFichier(socketCommClient, nbFichier, listeFichier, tabFichiersAEnvoyer);
     free(listeFichier);
     free(tabFichiersAEnvoyer);
     printf("Vous-avez choisis d'envoyer des fichiers\n");
}