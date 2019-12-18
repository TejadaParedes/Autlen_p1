CC=gcc
FLAGS = -Wall -ansi
OBJECTSe1 = main.o afnd.o transforma.o new_estado.o 

#Ejecutable
all: main

main: $(OBJECTSe1)
	$(CC) -g -Wall -ansi -pedantic -o main $(OBJECTSe1)

#Objetos
main.o: main.c afnd.h
	$(CC)  -g -c main.c
afnd.o: afnd.c afnd.h
	$(CC) -g -c afnd.c
transforma.o: transforma.c transforma.h afnd.h
	$(CC)  -g -c transforma.c
new_estado.o: new_estado.c new_estado.h transforma.h
	$(CC) -g -c new_estado.c

#Limpieza
clean:
	rm -rf *.o main


valgrind:
	valgrind -v --leak-check=full ./main

run:
	./main