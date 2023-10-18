#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #include "sequencial.h"
#include "utilitarios.h"
// #include "generate/generator.h"

void printB(FILE *arq)
{
    Registros reg;

    rewind(arq);

    while(fread(&reg, sizeof(Registros), 1, arq) == 1)
        printf("%-10d\t%-15ld\t%s\n", reg.chave, reg.dado1, reg.dado2);
}

int main(int argc, char *argv[]) 
{
    if (argc < 5)
    {
        printf("Quantidade de argumentos inválida.\n");
        return 0;
    }
    
    Entrada entrada;

    entrada.metodo = atoi(argv[1]);
    entrada.quantidade = atoi(argv[2]);
    entrada.situacao = atoi(argv[3]);
    entrada.chave = atoi(argv[4]);
    printf("%d", entrada.metodo);

    FILE *arq = fopen("generate/registros.bin", "r./b");
    
    if ((strcmp(argv[5], "-P") == 0) || (strcmp(argv[5], "-p") == 0))
        printB(arq);
    
    
    
    
    
    return 0;
}

