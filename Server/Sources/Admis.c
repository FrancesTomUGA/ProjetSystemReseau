#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "Admis.h"
#define TAILLE_TYPE 30 // Taille du char* type mime = 30

char** listeMymes(int* nbTypes)
{
     char** typeSSSMime = (char **)malloc(0);
     *nbTypes = 0;
     /* Récupération des types mime (ici, typeMime) dans un tableau (ici, typeSSSMime) (en haut niveau) 
        (pour le bas niveau, voir la partie commentée ci-dessous) */

     /*
     Le fichier MimeTypes.txt ici est dans le même répertoire que le fichier Admis.c 
     */

     FILE *types = NULL;
     types = fopen("./MimeTypes.txt", "r");
     int i = 0;

     if (types != NULL)
     {
          printf("inside\n");
          while (!feof(types))
          {
               printf("i = %d\n", i);
               typeSSSMime = (char **)realloc(typeSSSMime, sizeof(char *) * (*nbTypes + 1));

               char *typeMime = malloc(sizeof(char) * TAILLE_TYPE);
               typeSSSMime[*nbTypes] = malloc(sizeof(char) * TAILLE_TYPE);
               typeSSSMime[*nbTypes] = fgets(typeMime, TAILLE_TYPE, types);
               typeSSSMime[*nbTypes][strlen(typeSSSMime[*nbTypes]) - 1] = '\0';
               *nbTypes += 1;
          }
          fclose(types);
     }
     else
     {
          printf("Lecture du fichier des Types Mime impossible.\n");
     }

     //affichage tableau
     for (int i = 0; i < 3; i++)
     {
          printf("type : %s\n", typeSSSMime[i]);
     }
     return typeSSSMime;
}

int admissible(char *nomImage)
{
     int numberOfTypes = 0;
     char **typeSSSMime = listeMymes(&numberOfTypes);      // on considère pour le moment qu'il n'y a pas plus de 3 types mime dans le fichier txt

     int bool = 0;
     char cheminImage[strlen(nomImage)+6];
     cheminImage[0]='\0';
     strcat(cheminImage,"./tmp/");	//tmp est le fichier de stockage temporel
     strcat(cheminImage,nomImage);
     // commande à être exécutée par le processus fils
     // pour récupérer le type mime du fichier
     char *cmd[4] = {"file", "--mime-type", cheminImage, (char *)0};

     int f[2];
     pipe(f);

     switch (fork())
     {
     case -1:
          fprintf(stderr, "Erreur de fork\n");
          exit(-1);
     case 0:
          //comportement du fils // Vérification d'admissibilité du fichier
          close(1); //redirection de la sortie standard
          dup(f[1]);
          close(f[1]);
          close(f[0]); //fermeture des descripteurs inutilisés
          execvp("file", cmd);

     default:
          switch (fork())
          {
          case -1:
               exit(-1);
          case 0:

               //comportement du père
               close(f[1]); //fermeture du descripteur inutilisé

               char typeFichierRecup[100];
               read(f[0], typeFichierRecup, sizeof(typeFichierRecup));

               char *recup = strrchr(typeFichierRecup, ' '); // Récupération du type en récupérant tout se qui est situé après la dernière occurence du caractère 'espace'

               int l;

               for (l = 1; l < strlen(recup); l++)
               {
                    recup[l - 1] = recup[l]; // Suppression de l'espace en début de la chaîne de caractères
               }
               //On retire le caractère \n à la fin de la chaine
               recup[l - 2] = '\0';
               printf("type lu : %s\n", recup);

               int j = 0;
               while (j < numberOfTypes && bool == 0)
               {
                    /* vérification de l'existance de ce type dans le tableau parcouru 
                    *des types mime admissibles 
                    */
                    if (strcmp(recup, typeSSSMime[j]) == 0)
                    {
                         bool = 1;
                         printf("Le fichier est admissible (type MIME admissible), il a été placé dans le répertoire FilesServeur\nbool = %d\n", bool);
                         switch(fork()){
                              case -1:
                                   exit(-1);
                              case 0:
                                   char cheminFinal[15];
                                   cheminFinal[0]='\0';
                                   strcat(cheminFinal, "./FilesServeur/");
                                   execlp("mv", "mv", cheminImage, cheminFinal, (char *)0);
                              default:
                                   wait(NULL);
                         }
                    }
                    else
                    {
                         printf("Le fichier N'est PAS admissible (type MIME inadmissible), il n'a pu être transféré (ou plutôt, il a été placé dans le répertoire temporaire, mais il a été supprimé après vérification)\n bool = %d\n", bool);
                         switch(fork()){
                              case -1:
                                   exit(-1);
                              case 0:
                                   execlp("rm", "rm", cheminImage, (char *)0);
                              default:
                                   wait(NULL);
                         }
                    }
                    j++;
               }
               exit(0);
          default:
               while (wait(NULL) != -1)
                    ;
          }
     }
     return bool;
}
