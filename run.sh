gcc -c -o Objets/Affichage.o Source/Affichage.c
gcc -c -o Objets/Reception.o Source/Reception.c
gcc -c -o Objets/Client.o Source/Client.c
gcc -o Exec/client Objets/Client.o Objets/Affichage.o Objets/Reception.o 
gcc -o Exec/server Source/Server.c
gnome-terminal --tab -e Exec/server
Exec/client