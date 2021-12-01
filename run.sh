gcc -c -o Objets/Envoi.o Source/Envoi.c
gcc -c -o Objets/Reception.o Source/Reception.c
gcc -c -o Objets/Client.o Source/Client.c
gcc -c -o Objets/Server.o Source/Server.c
gcc -c -o Objets/Transfert.o Source/Transfert.c
gcc -g -o Exec/client Objets/Client.o Objets/Envoi.o Objets/Reception.o Objets/Transfert.o
gcc -g -o Exec/server Objets/Server.o Objets/Transfert.o
