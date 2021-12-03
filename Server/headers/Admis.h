#ifndef ADMIS_C
#define ADMIS_C



/**
 * @brief Indique si une image possède un type autorisé par le serveur et peut être stockée.
 * 
 * @param nomImage le nom de l'image dont le type est vérifié
 * @return int 1 (true) si le type de l'image est autorisé, 0 (false) sinon
 */
int admissible(char* nomImage);

/**
 * @brief Indique si la checksum de l'image est interdite ou non
 * 
 * @param cheminImage le chemin de l'image dont on veut vérifier la checksum
 */
int checksumInterdite(char *cheminImage);

/**
 * @brief Récupère la liste des types d'images autorisés pour le stockage 
 * sur le serveur. Ces types sont écrits dans le fichier MimeTypes.txt
 * 
 * @param nbType contient le nombre de types contenus dans le fichier MimeTypes.txt
 * @return char** la liste des types autorisés
 */
char **recupereListeTypeMime(int *nbType);

#endif
