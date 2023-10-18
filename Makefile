DIR = implementacao

all.o: main.o utilitarios.o
	@gcc main.o utilitarios.o -o exe
	@rm main.o utilitarios.o 

main.o: main.c
	@gcc -c main.c -Wall

utilitarios.o: $(DIR)/utilitarios.c
	@gcc -c $(DIR)/utilitarios.c -Wall