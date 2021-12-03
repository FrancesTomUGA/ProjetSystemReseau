#include <stdlib.h>
#include <stdio.h>
#include "Utils.h"



int saisieEntier() {
     char temp[10];
     scanf("%s", temp);
     
     return atoi(temp);
}