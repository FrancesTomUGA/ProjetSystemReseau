#Variables
CC = gcc
EXECCLIENT = Client
SFOLD = Sources
OFOLD = Objets
SRC = $(wildcard $(SFOLD)/*.c)
OBJ = $(SRC:$(SFOLD)/.c=$(OFOLD)/.o)


all : $(EXECCLIENT)

$(EXECCLIENT) : $(OBJ)
	$(CC) -g -o $(EXECCLIENT) $^

%.o : %.c
	$(CC) -c -o $(OBJ) $(SRC)

fullClean : clean
	rm -rf Client
clean :
	#rem -rf Objets/*.o
