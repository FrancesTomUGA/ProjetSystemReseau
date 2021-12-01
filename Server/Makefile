#Variables
CC = gcc
EXECSERVER = Server
SFOLD = Sources
OFOLD = Objets
SRC = $(wildcard $(SFOLD)/*.c)
OBJ = $(SRC:$(SFOLD)/.c=$(OFOLD)/.o)


all : $(EXECSERVER)

$(EXECSERVER) : $(OBJ)
	$(CC) -o $(EXECSERVER) $^

%.o : %.c
	$(CC) -c -o $(OBJ) $(SRC)

fullClean : clean
	rm -rf Server

clean :
	rem -rf Objets/*.o

