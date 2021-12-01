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

    DIR *rep = opendir("files/"); /* Ouverture d'un dossier */

    if (rep == NULL) /* Si le dossier n'a pas pu être ouvert */
        exit(1);     /* (mauvais chemin par exemple) */

    printf("Le dossier a ete ouvert avec succes\n");

    char *nomImage;
    char *cheminImageLue;
    char *cheminImageTransfert;
    int imageLue;
    int imageEcrite;
    int fd[2];
    
    char *chaine;

    struct dirent *imageLueDir = NULL;
    /*
    imageLueDir = readdir(rep);
    printf("nom : %s\n", imageLueDir->d_name);
    imageLueDir = readdir(rep);
    printf("nom : %s\n", imageLueDir->d_name);
    imageLueDir = readdir(rep);
    printf("nom : %s\n", imageLueDir->d_name);
    imageLueDir = readdir(rep);
    printf("nom : %s\n", imageLueDir->d_name);
    imageLueDir = readdir(rep);
    printf("nom : %s\n", imageLueDir->d_name);

    if ((imageLueDir = readdir(rep)) == NULL)
    {
        printf("nl\n");
    }
    */
    while ((imageLueDir = readdir(rep)) != NULL)
    {

        if (imageLueDir->d_type == DT_REG && imageLueDir->d_name[0] != '.')
        {

            nomImage = malloc(sizeof(char) * 256);
            strcat(nomImage, imageLueDir->d_name);
            printf("Nom fichier lu : %s\n", nomImage);

            cheminImageLue = malloc(sizeof(char) * 306);
            strcat(cheminImageLue, "files/");
            strcat(cheminImageLue, nomImage);
            printf("chemin image lue: %s\n", cheminImageLue);

            cheminImageTransfert = malloc(sizeof(char) * 306);
            strcat(cheminImageTransfert, "files_transfert/");
            strcat(cheminImageTransfert, nomImage);

            switch (fork())
            {
            case -1:
                exit(-1);
            case 0:
                execlp("touch", "touch", cheminImageTransfert, (char *)0);
            default:
                wait(NULL);
            }

            imageLue = open(cheminImageLue, O_RDONLY);
            //separation cotes client serveur
            
            imageEcrite = open(cheminImageTransfert, O_WRONLY);

            if (imageLue == -1 || imageEcrite == -1)
            {
                printf("Erreur ouverture image\n");
                exit(-1);
            }

            pipe(fd);
            chaine = malloc(sizeof(char) * 4096);
            switch (fork())
            {
            case -1:
                exit(-1);
            case 0:
                close(fd[0]);
                while (read(imageLue, chaine, sizeof(chaine)) > 0)
                {
                    //printf("envoie données image %s\n", nomImage);
                    write(fd[1], chaine, sizeof(chaine));
                }
                close(fd[1]);
                printf("Lecture image et transmission terminées\n");
                exit(0);
            default:
                close(fd[1]);
                while (read(fd[0], chaine, sizeof(chaine)) > 0)
                {
                    //printf("réception données image %s\n", nomImage);
                    write(imageEcrite, chaine, sizeof(chaine));
                }
                close(fd[0]);
                printf("Lecture données transmises écriture nouvelle image terminées\n");
                wait(NULL);
            }

            
            close(imageLue);
            close(imageEcrite);
            printf("Le dossier a ete ferme avec succes\n");
            //free(cheminImageLue);
            cheminImageLue[0] = '\0';
            //free(nomImage);
            nomImage[0] = '\0';
            //free(cheminImageTransfert);
            cheminImageTransfert[0] = '\0';
            //free(chaine);
            chaine[0] = '\0';
        }
        printf("fin d'un tour\n");
    }

    if (closedir(rep) == -1) /* S'il y a eu un souci avec la fermeture */
        exit(-1);

    return 0;
}
