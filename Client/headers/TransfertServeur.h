#ifndef TRANSFERTSERVEUR_H
#define TRANSFERTSERVEUR_H



/**
 * @brief 
 * 
 * @param socketTransfert 
 * @param nomImage 
 */
void envoiImage(int socketTransfert, char *nomImage);


/**
 * @brief 
 * 
 * @param socketService 
 */
void receptionImage(int socketService);

#endif