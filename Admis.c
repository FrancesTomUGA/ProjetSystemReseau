#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define TAILLE_TYPE 30 // Taille du char* type mime = 30

int main(int argc, char *argv[])
{
    int numberOfTypes = 3;                                       // on considère pour le moment qu'il n'y a pas plus de 3 types mime dans le fichier txt
    char **typeSSSMime = malloc(sizeof(char *) * numberOfTypes); // Tableau de types mime

    /* Récupération des types mime (ici, typeMime) dans un tableau (ici, typeSSSMime) (en haut niveau) 
        (pour le bas niveau, voir la partie commentée ci-dessous) */

    /*
    Le fichier MimeTypes.txt ici est dans le même répertoire que le fichier Admis.c 
    */

    FILE *types = NULL;
    types = fopen("MimeTypes.txt", "r");
    int i = 0;

    if (types != NULL)
    {
        while (!feof(types) && i < numberOfTypes)
        {
            printf("i = %d\n", i);
            char *typeMime = malloc(sizeof(char) * TAILLE_TYPE);
            typeSSSMime[i] = fgets(typeMime, TAILLE_TYPE, types);
            typeSSSMime[i][strlen(typeSSSMime[i])-1] = '\0';
            i++;
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

    int bool = 0;

    char *nomFichier = argv[1];

    char *cmd[4] = {"file", "--mime-type", nomFichier, (char *)0}; // commande à être exécutée par le processus fils
                                                                   // pour récupérer le type mime du fichier
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

            char* recup = strrchr(typeFichierRecup, ' '); // Récupération du type en récupérant tout se qui est situé après la dernière occurence du caractère 'espace'
            
            char *delim = strtok(typeFichierRecup, "\n");
            char *tablOutput[2];
            int i = 0;
            while (delim != NULL){
                tablOutput[i++] = delim;
                delim = strtok(NULL, " ");
            }

            printf("recup : %s\n", recup);
            printf("tableoutput : %s\n", tablOutput[0]);
            int l;
            
            for(l = 1; l < strlen(recup); l++){
               recup[l-1] = recup[l]; // Suppression de l'espace en début de la chaîne de caractères
            }
            recup[l-1] = '\0';
            printf("type lu : %s\n", recup);

            int j = 0;
            while (j < numberOfTypes && bool == 0)
            {
                if (strcmp(recup, typeSSSMime[j]) == 0) /* vérification de l'existance de ce type dans le tableau parcouru 
                                                                                des types mime admissibles */
                {
                    bool = 1;
                    printf("type admissible\n bool = %d\n", bool);
                }
                else
                {
                    printf("pas le bon type\n bool = %d\n", bool);
                }
                j++;
            }
            exit(0);
        default:
            while (wait(NULL) != -1)
                ;
        }
    }
    
}
