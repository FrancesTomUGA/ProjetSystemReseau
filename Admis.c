#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define TAILLE_TYPE 30 // Taille du char* type mime = 30

int main(int argc, char *argv[])
{
    int numberOfTypes = 3; // on considère pour le moment qu'il n'y a pas plus de 3 types mime dans le fichier txt
    char** typeSSSMime = malloc(sizeof(char*) * numberOfTypes); // Tableau de types mime

    /* Récupération des types mime (ici, typeMime) dans un tableau (ici, typeSSSMime) (en haut niveau) 
        (pour le bas niveau, voir la partie commentée ci-dessous) */

    /*
    Le fichier MimeTypes.txt ici est dans le même répertoire que le fichier Admis.c 
    */

    FILE* types = NULL;
    types = fopen("MimeTypes.txt", "r");
    
    if (types != NULL) 
    {
        while(!feof(types))
        {
            for (int i = 0; i < numberOfTypes; i++) 
            {
                char* typeMime = malloc(sizeof(char) * TAILLE_TYPE);
                typeSSSMime[i] = fgets(typeMime, TAILLE_TYPE, types);
                // printf("%s\n", typeMime);
            }
        }
        fclose(types);
    }
    else
    {
        printf("Lecture du fichier des Types Mime impossible.\n");
    }

    /* 
    // Récupération des types mime dans un tableau (en bas niveau)
    int source = open("MimeTypes.txt", O_RDONLY);
    char tampon[TAILLE_TYPE];

    if (source != -1) 
    {
        for (int i = 0; i < numberOfTypes; i++) 
        {
            char* typeMime = malloc(sizeof(char) * TAILLE_TYPE);
            typeSSSMime[i] = read(source, &tampon, TAILLE_TYPE); //tampon à vérifier ???
            // ou peut être 
            // read(source, &typeSSSMime[i], TAILLE_TYPE); //???
        }
        close (source);
    }
    else
    {
        printf(stderr, "Problème de lecture du fichier des Types Mime\n");
    }
    */
   

    // Vérification d'admissibilité du fichier

    /*
    Le fichier (l'image) ici est dans le même répertoire que le fichier Admis.c 
    */

    char* nomFichier = argv[1];
    int bool = 0;

    char* cmd[3] = {"file", "--mime-type", nomFichier}; // commande à être exécutée par le processus fils 
                                                        // pour récupérer le type mime du fichier
    int f[2];
    pipe(f);

    switch (fork())
    {
    case -1 :
        fprintf(stderr, "Erreur de fork\n");
        exit(-1);
    case 0 :
        //comportement du fils // Vérification d'admissibilité du fichier
        close(1); //redirection de la sortie standard
        dup(f[1]);
        close(f[0]); //fermeture du descripteur inutilisé
        execvp("file", cmd);
        //la sortie de "file" (a.k.a. le type mime du fichier) ira dans le pipe
    default:
        //comportement du père
        close(f[1]); //fermeture du descripteur inutilisé
        char* typeFichierRecup = "";
        for (int i = 0; i < numberOfTypes; i++)
        {
            read(f[0], &typeFichierRecup, sizeof(char*));
            if ( typeFichierRecup == typeSSSMime[i]) /* Lecture du type de fichier sur la sortie du pipe
                                                                        et vérification de l'existance de ce type dans le tableau parcouru 
                                                                        des types mime admissibles */
            {
                bool = 1;
                // printf("réussite\n");
            }
        }
        while (wait(NULL) != -1);
    }

}
