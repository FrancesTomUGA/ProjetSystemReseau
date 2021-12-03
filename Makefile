#Variables

all :
	$(MAKE) -C Client $@
	$(MAKE) -C Server $@

globalClean : clean
	rm -rf Client/bin/Client
	rm -rf Server/bin/Server
clean :
	rm -rf Client/Objets/*
	rm -rf Server/Objets/*

# variables spéciales
# $* nom fichier sans l'extension
# $? liste dependance plus recente que la cible
# $^ liste dependance
# $< premiere dépendance
# $@ reference a la cible
# <cible>: <dependance>
#        <commande>