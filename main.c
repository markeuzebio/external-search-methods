#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "implementacao/utilitarios.h"
#include "implementacao/sequencial.h"
#include "implementacao/arvore_binaria.h"
#include "implementacao/arvoreB.h"

int main(int argc, char* argv[])
{
    short int retorno_funcao;
    FILE *arq_bin, *arq_arv_bin;
    Entrada entrada;

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
                retorno_funcao = sequencial(arq_bin, &entrada);

                if(retorno_funcao == -1)
                    printf("Nao foi possivel alocar dinamicamente um vetor em memoria principal. Abortando o programa...\n");
                else if(retorno_funcao == 0)
                    printf("A chave passada como argumento (%d) não existe no arquivo.\n", entrada.chave_buscada);
                else
                    printf("A chave %d foi encontrada no arquivo!\n", entrada.chave_buscada);
            }
        break;

        // Arvore Binaria de Pesquisa
        case 2:
            // Caso nao consiga criar o novo arquivo binario de estrutura arvore binaria
            if((arq_arv_bin = fopen("registros_arvore.bin", "w+b")) == NULL)
                printf("Nao foi possivel abrir/gerar o arquivo binario de arvore. Abortando o programa...\n");
            else
            {
                printf("GERANDO O ARQUIVO BINARIO...\n\n");
                retorno_funcao = arvoreBinariaGerar(arq_bin, arq_arv_bin, &entrada);

                if(retorno_funcao == 1)
                {
                    if(arvoreBinaria(arq_arv_bin, entrada.chave_buscada))
                        printf("A chave %d foi encontrada no arquivo!\n", entrada.chave_buscada);
                    else
                        printf("A chave passada como argumento (%d) não existe no arquivo.\n", entrada.chave_buscada);
                }
                else
                    printf("Nao foi possivel alocar dinamicamente um vetor em memoria principal. Abortando o programa...\n");
            }
        break;

        // Arvore B
        case 3:
        {
            Pagina *raiz;

            printf("GERANDO A ARVORE B A PARTIR DO ARQUIVO OFERECIDO...\n\n");
            raiz = gerarArvoreB(arq_bin, &entrada);

            if(raiz == NULL)
                printf("Nao foi possivel alocar memoria dinamicamente em alguma parte do processo. Abortando o programa...\n");
            else
            {
                if(arvoreB(raiz, entrada.chave_buscada))
                    printf("A chave %d foi encontrada no arquivo!\n", entrada.chave_buscada);
                else
                    printf("A chave passada como argumento (%d) não existe no arquivo.\n", entrada.chave_buscada);

                desalocarArvoreB(&raiz);
            }
        }
        break;

        // // Arvore B*
        // case 4:

        // break;

        default:
            printf("TESTE\n");
    }

    fclose(arq_bin);

    return 0;
}