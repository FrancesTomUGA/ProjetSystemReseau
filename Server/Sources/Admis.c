#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/wait.h>
#include <inttypes.h>

#define TAILLE_TYPE 50 // Taille du char* type mime = 50

char **recupereListeTypeMime(int *nbType){
     *nbType = 0;
     char **typeSSSMime = (char **)malloc(0); // Tableau de types mime
     // Récupération des types mime (ici typeMime) dans un tableau (ici typeSSSMime) en haut niveau
     char typeMime[TAILLE_TYPE];
     FILE *types = NULL;
     types = fopen("./MimeTypes.txt", "r");

     if (types != NULL){
          while (!feof(types)){
               typeMime[0] = '\0';
               fgets(typeMime, TAILLE_TYPE, types);

               if (typeMime[strlen(typeMime) - 1] == '\n'){
                    typeMime[strlen(typeMime) - 1] = '\0';
               }

               typeSSSMime = (char **)realloc(typeSSSMime, sizeof(char *) * (*nbType + 1));
               typeSSSMime[*nbType] = malloc(sizeof(char) * strlen(typeMime));
               strcpy(typeSSSMime[*nbType], typeMime);
               *nbType += 1;
          }

          fclose(types);
     }else{
          printf("Lecture du fichier des Types Mime impossible.\n");
     }

     return typeSSSMime;
}

int checksumInterdite(char *cheminImage){

     FILE *fichier = NULL;
     fichier = fopen(cheminImage, "r");
     if (fichier == NULL){
          fprintf(stderr, "Erreur ouverture image checksum\n");
          return 0;
     }

     FILE *listeChecksumInterdite = NULL;
     listeChecksumInterdite = fopen("./checksum.txt", "r");
     if (listeChecksumInterdite == NULL){
          fprintf(stderr, "Erreur ouverture checksum.txt\n");
          return 0;
     }

     char chaine[5];
     // on lit et ignore le P3
     fgets(chaine, 5, fichier);
     chaine[0] = '\0';
     int dim[2];
     fscanf(fichier, "%d %d\n", &dim[0], &dim[1]);

     // on lit et ignore le 255
     fgets(chaine, 5, fichier);
     chaine[0] = '\0';

     int nbPixel = dim[0] * dim[1];
     int **tabPixel = malloc(sizeof(int *) * nbPixel);
     for (int i = 0; i < nbPixel; i++){
          tabPixel[i] = malloc(sizeof(int) * 3);
          fscanf(fichier, "%d ", &tabPixel[i][0]);
          fscanf(fichier, "%d ", &tabPixel[i][1]);
          fscanf(fichier, "%d ", &tabPixel[i][2]);
     }

     srand(100);
     int blue = 0;
     int green = 0;
     int red = 0;
     int numPixel;

     for (int i = 0; i < 100; i++){
          numPixel = rand() % nbPixel;
          blue += tabPixel[numPixel][0];
          green += tabPixel[numPixel][1];
          red += tabPixel[numPixel][2];
     }

     char *checksum = malloc(sizeof(char) * 16);
     sprintf(checksum, "%04X%04X%04X%04X", 0, red, green, blue);

     char checksumInterditLu[17];
     int checksumInterdite = 0;
     while (!feof(listeChecksumInterdite) && !checksumInterdite){
          checksumInterditLu[0] = '\0';
          fgets(checksumInterditLu, 17, listeChecksumInterdite);
          if(strcmp(checksumInterditLu, checksum) == 0){
               checksumInterdite = 1;
          }
     }

     free(tabPixel);
     fclose(fichier);
     fclose(listeChecksumInterdite);

     return checksumInterdite;
}

int admissible(char *nomImage){
     int nbType = 0;
     char **listeTypeMime = recupereListeTypeMime(&nbType);
     int typeAdmis = 0;
     int checksumRefusee = 0;
     char cheminImageTmp[306];
     char cheminImageFinal[306];
     sprintf(cheminImageTmp, "./tmp/%s", nomImage);
     sprintf(cheminImageFinal, "./FilesServeur/%s", nomImage);
     int f[2];
     pipe(f);

     switch (fork()){
     case -1:
          fprintf(stderr, "Erreur de fork\n");
          exit(-1);

     case 0:
          // Comportement du fils // Vérification de l'admissibilité du fichier
          close(1); // Redirection de la sortie standard
          dup(f[1]);
          close(f[0]); // Fermeture du descripteur non utilisé
          close(f[1]);

          // Commande à être exécuté par le processus fils pour récupérer le type mime du fichier
          execlp("file", "file", "--mime-type", cheminImageTmp, (char *)0);
          // La sortie de "file" (a.k.a. le type mime du fichier) ira dans le pipe

     default:
          switch (fork()){
          case -1:
               exit(-1);

          case 0:
               // Comportement du père
               close(f[1]); // Fermeture du descripteur non utilisé

               char typeFichiertype[100];
               read(f[0], typeFichiertype, sizeof(typeFichiertype));
               char *type = strrchr(typeFichiertype, ' '); // Récupération du type en récupérant tout ce qui est situé après la dernière occurence du caractère 'espace'
               char *delim = strtok(typeFichiertype, "\n");
               char *tablOutput[2];
               int i = 0;

               while (delim != NULL){
                    tablOutput[i++] = delim;
                    delim = strtok(NULL, " ");
               }

               int l;
               for (l = 1; l < strlen(type); l++){
                    type[l - 1] = type[l]; // Suppression de l'espace en début de la chaîne de caractères
               }
               type[l - 1] = '\0';

               int j = 0;
               while (j < nbType && !typeAdmis){
                    /* Vérification de l'existence de ce type dans le tableau parcouru des types mime admissibles */
                    if (strcmp(type, listeTypeMime[j]) == 0){
                         typeAdmis = 1;
                         if(strcmp(type, "image/x-portable-pixmap") == 0 && (checksumRefusee = checksumInterdite(cheminImageTmp))){
                              typeAdmis = 0;
                         }
                         
                    }
                    j++;
               }

               if (typeAdmis){
                    printf("Fichier accepté\n");
                    rename(cheminImageTmp, cheminImageFinal);
               }else{
                    if (unlink(cheminImageTmp) == 0){
                         if(checksumRefusee){
                              printf("Fichier PPM refusé (checksum interdite)\n");
                         }else{
                              printf("Fichier refusé (type %s non admis)\n", type);
                         }
                    }
               }
               exit(0);

          default:
               while (wait(NULL) != -1);
          }
     }

     if (listeTypeMime != NULL){
          free(listeTypeMime);
     }
}