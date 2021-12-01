#ifndef TRANSFERT_H
#define TRANSFERT_H

void envoiImageClientServeur(char* nomImage, int socketTransfert);
void receptionImageServeur(int socketService);

#endif