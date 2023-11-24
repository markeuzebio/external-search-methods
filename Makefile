DIR = implementacao

all.o: main.o utilitarios.o sequencial.o arvore_binaria.o arvoreB.o arvoreBs.o
	@gcc main.o utilitarios.o sequencial.o arvore_binaria.o arvoreB.o arvoreBs.o -o pesquisa
	@rm main.o utilitarios.o sequencial.o arvore_binaria.o arvoreB.o arvoreBs.o

main.o: main.c
	@gcc -g -c main.c -Wall

utilitarios.o: $(DIR)/utilitarios.c $(DIR)/utilitarios.h
	@gcc -g -c $(DIR)/utilitarios.c -Wall

sequencial.o: $(DIR)/sequencial.c $(DIR)/sequencial.h
	@gcc -g -c $(DIR)/sequencial.c -Wall

arvore_binaria.o: $(DIR)/arvore_binaria.c $(DIR)/arvore_binaria.h
	@gcc -g -c $(DIR)/arvore_binaria.c -Wall

arvoreB.o: $(DIR)/arvoreB.c $(DIR)/arvoreB.h
	@gcc -g -c $(DIR)/arvoreB.c -Wall

arvoreBs.o: $(DIR)/arvoreBs.c $(DIR)/arvoreBs.h
	@gcc -g -c $(DIR)/arvoreBs.c -Wall
