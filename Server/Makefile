#Variables
CC = gcc
EXECSERVER = bin/Server
INC_DIR = ./headers
CFLAGS=-I$(INC_DIR)
SFOLD = Sources
OFOLD = Objets
SRC = $(wildcard $(SFOLD)/*.c)
OBJ = $(SRC:$(SFOLD)/.c=$(OFOLD)/.o)


all : $(EXECSERVER)

$(EXECSERVER) : $(OBJ)
	$(CC) -g -o $(EXECSERVER) $^ $(CFLAGS)

%.o : %.c
	$(CC) -c -o $(OBJ) $(SRC)

fullClean : clean
	rm -rf Server

clean :
	rem -rf Objets/*.o

