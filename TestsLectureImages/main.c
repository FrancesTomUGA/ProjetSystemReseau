#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define TAILLE_MAX 10000

int main(int argc, char const *argv[])
{

    DIR *rep = opendir("files"); /* Ouverture d'un dossier */

    if (rep == NULL) /* Si le dossier n'a pas pu être ouvert */
        exit(1);     /* (mauvais chemin par exemple) */

    printf("Le dossier a ete ouvert avec succes\n");

    struct dirent *fichierLu = NULL;
    fichierLu = readdir(rep);

    printf("Nom fichier lu : %s\n", fichierLu->d_name);

    char nomFichierLu[300];
    strcat(nomFichierLu, "files/");
    strcat(nomFichierLu, fichierLu->d_name);
    printf("nom fichier lu: %s\n", nomFichierLu);

    char nomFichierTransfert[300];
    strcat(nomFichierTransfert, "files_transfert/");
    strcat(nomFichierTransfert, fichierLu->d_name);

    switch (fork())
    {
    case -1:
        exit(-1);
    case 0:
        execlp("touch", "touch", nomFichierTransfert, (char *)0);
    default:
        wait(NULL);
    }

    sleep(1);

    int imageLue = open(nomFichierLu, O_RDONLY);
    int imageEcrite = open(nomFichierTransfert, O_WRONLY);

    if (imageLue == -1 || imageEcrite == -1)
    {
        printf("Erreur ouverture image\n");
        exit(-1);
    }

    int fd[2];
    pipe(fd);
    char chaine[TAILLE_MAX] = "";

    switch (fork())
    {
    case -1:
        exit(-1);
    case 0:
        close(fd[0]);
        while (read(imageLue, chaine, 256) > 0)
        {
            write(fd[1], chaine, 256);
        }
        close(fd[1]);
        printf("Lecture image et transmission terminées\n");
        exit(0);
    default:
        close(fd[1]);
        while (read(fd[0], chaine, 256) > 0)
        {
            write(imageEcrite, chaine, 256);
        }
        close(fd[0]);
        printf("Lecture données transmises écriture nouvelle image terminées\n");
        exit(0);
        wait(NULL);
    }

    if (closedir(rep) == -1) /* S'il y a eu un souci avec la fermeture */
        exit(-1);

    close(imageLue);
    close(imageEcrite);
    printf("Le dossier a ete ferme avec succes\n");

    return 0;
}
