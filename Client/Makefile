#Variables
CC = gcc
EXECCLIENT = bin/Client
INC_DIR = ./headers
CFLAGS=-I$(INC_DIR)
SFOLD = Sources
OFOLD = Objets
SRC = $(wildcard $(SFOLD)/*.c)
OBJ = $(SRC:$(SFOLD)/.c=$(OFOLD)/.o)


all : $(EXECCLIENT)

$(EXECCLIENT) : $(OBJ)
	$(CC) -g -o $(EXECCLIENT) $^ $(CFLAGS)

%.o : %.c
	$(CC) -c -o $(OBJ) $(SRC)

fullClean : clean
	rm -rf Client
clean :
	#rem -rf Objets/*.o
