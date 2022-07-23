#----Run-All-Progams----#
run: all
	./busca_1a 
	./busca_1b  
	./busca_1c  
	./busca_1d 
	./busca_2a 
	./busca_2b 
	./busca_2c

#----Compile-Flags----#
all: busca_1a busca_1b busca_1c busca_1d busca_2a busca_2b busca_2c

busca_1a:
	gcc -o busca_1a busca_1a.c

busca_1b:
	gcc -o busca_1b busca_1b.c

busca_1c:
	gcc -o busca_1c busca_1c.c

busca_1d:
	gcc -o busca_1d busca_1d.c

busca_2a:
	gcc -o busca_2a busca_2a.c

busca_2b:
	gcc -o busca_2b busca_2b.c

busca_2c:
	gcc -o busca_2c busca_2c.c

#----clear---#
clear:
	rm *.exe && clear