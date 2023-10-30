DIR = implementacao

all.o: main.o utilitarios.o sequencial.o arvore_binaria.o
	@gcc main.o utilitarios.o sequencial.o arvore_binaria.o -o exe
	@rm main.o utilitarios.o sequencial.o arvore_binaria.o

main.o: main.c
	@gcc -c main.c -Wall

utilitarios.o: $(DIR)/utilitarios.c $(DIR)/utilitarios.h
	@gcc -c $(DIR)/utilitarios.c -Wall

sequencial.o: $(DIR)/sequencial.c $(DIR)/sequencial.h
	@gcc -c $(DIR)/sequencial.c -Wall

arvore_binaria.o: $(DIR)/arvore_binaria.c $(DIR)/arvore_binaria.h
	@gcc -c $(DIR)/arvore_binaria.c -Wall
