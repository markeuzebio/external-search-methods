DIR = implementacao

all.o: main.o utilitarios.o sequencial.o
	@gcc main.o utilitarios.o sequencial.o -o exe
	@rm main.o utilitarios.o sequencial.o

main.o: main.c
	@gcc -c main.c -Wall

utilitarios.o: $(DIR)/utilitarios.c
	@gcc -c $(DIR)/utilitarios.c -Wall

sequencial.o: $(DIR)/sequencial.c
	@gcc -c $(DIR)/sequencial.c -Wall
