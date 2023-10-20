#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "implementacao/utilitarios.h"
#include "implementacao/sequencial.h"

int main(int argc, char* argv[])
{
    short retorno_funcao;
    FILE *arq_bin;
    Entrada entrada;
    Registro registro_saida;

    // Le entrada enquanto verifica se eh valida. Retorna "true" se for valida e "false" caso contrario.
    if(lerEntrada(&entrada, argc, argv) == false)
    {
        printf("O modo de executar o programa esta errado ou algum argumento inserido nao e valido, execute de acordo:\n\n");
        printf("%s <metodo> <quantidade> <situacao> <chave> [-P|-p]\n", argv[0]);

        exit(1);
    }

    /*
        Se o arquivo "registros.bin" nao existir no diretorio corrente,
        termina a execucao e para.
    */
    if((arq_bin = fopen("registros.bin", "rb")) == NULL)
    {
        printf("\nNao foi possivel abrir o arquivo binario \"registros.bin\". Abortando o programa...\n");
        return 0;
    }

    switch(entrada.metodo)
    {
        // Acesso Sequencial Indexado
        case 1:
            // Se o arquivo passado esta desordenado, nao eh possivel utilizar este metodo.
            if(entrada.situacao == 3)
                printf("O metodo escolhido nao funciona se o arquivo estiver desordenado! Abortando o programa...\n");
            else
            {
                retorno_funcao = sequencial(arq_bin, &entrada, &registro_saida);

                if(retorno_funcao == -1)
                    printf("Nao foi possivel alocar dinamicamente um vetor em memoria principal. Abortando o programa...\n");
                else if(retorno_funcao == 0)
                    printf("A chave passada como argumento (%d) não existe no arquivo.\n", entrada.chave_buscada);
                else
                    printf("A chave %d foi encontrada no arquivo!\n", entrada.chave_buscada);
            }
        break;

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