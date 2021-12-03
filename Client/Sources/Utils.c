#include <stdlib.h>
#include <stdio.h>
#include "Utils.h"

int saisieEntier() {
     char temp[50];
     scanf("%s", temp);
     
     return atoi(temp);
}

void clear() {
     system("clear");
}