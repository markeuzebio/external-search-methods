#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "implementacao/utilitarios.h"

int main(int argc, char* argv[])
{
    FILE *arq_bin;
    Entrada entrada;

    // Le entrada enquanto verifica se eh valida. Retorna "true" se for valida e "false" caso contrario.
    if(lerEntrada(&entrada, argc, argv) == false)
    {
        printf("O modo de executar o programa esta errado ou algum argumento inserido nao e valido, execute de acordo:\n\n");
        printf("%s <metodo> <quantidade> <situacao> <chave> [-P|-p]\n", argv[0]);

        exit(1);
    }

    /*
        Bloco de codigo relacionado aa leitura do arquivo.
        Se algum erro ocorrer, o programa eh abortado.
    */
    {
        char nome_arquivo[100];

        printf("Nome do arquivo binario localizado neste diretorio: "); scanf("%[\n]s", nome_arquivo);

        if((arq_bin = fopen(nome_arquivo, "rb")) == NULL)
        {
            printf("\nNao foi possivel abrir o arquivo binario %s informado. Abortando o programa...\n", nome_arquivo);
            return 0;
        }
    }

    switch(entrada.metodo)
    {
        // Acesso Sequencial Indexado
        // case 1:
        
        // break;

        // Arvore Binaria de Pesquisa
        // case 2:
            
        // break;

        // // Arvore B
        // case 3:

        // break;

        // // Arvore B*
        // case 4:

        // break;

        default:
            printf("TESTE\n");
    }

    fclose(arq_bin);

    return 0;
}