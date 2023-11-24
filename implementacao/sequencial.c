#ifndef  __SEQUENCIAL__
#define __SEQUENCIAL__

#include <stdio.h>
#include <stdlib.h>
#include "sequencial.h"
#include "utilitarios.h"

static unsigned int ITENS_POR_PAGINA;

int calculaItensPorPagina(int qtde_registros_arquivo)
{
    return (qtde_registros_arquivo / MAX_TABELA) + 1;
}

/*
    Realiza uma impressao de todos os itens do
    arquivo sobre o qual o metodo indexado ira incidir.

    Essa impressao utiliza fundamentos do proprio metodo
    de pesquisa, tornando-se, assim, otimizado.
*/
void printSequencial(FILE *arq, Tabela *tabela, Entrada *entrada)
{
    unsigned long int n_reg_lidos;
    Registro *pagina;

    ITENS_POR_PAGINA = calculaItensPorPagina(entrada->quantidade_registros);

    pagina = alocarRegistros(ITENS_POR_PAGINA);

    rewind(arq);

    for(int i = 0 ; i < tabela->qtde_indices ; i++)
    {
        fseek(arq, i * ITENS_POR_PAGINA * sizeof(Registro), SEEK_SET);

        n_reg_lidos = fread(pagina, sizeof(Registro), ITENS_POR_PAGINA, arq);

        for(int j = 0 ; j < n_reg_lidos ; j++)
            printf("%d\n", pagina[j].chave);
    }

    printf("\n");

    desalocarRegistros(&pagina);

    rewind(arq);
}

short preencheTabela(FILE *arq_bin, Tabela* tabela, Entrada *entrada, Metrica *metricas)
{
    unsigned int contador;
    Registro *pagina;
    // Variaveis usadas para metricas
    clock_t inicio;
    clock_t fim;

    // --- INICIO PRE-PROCESSAMENTO --- //
    inicio = clock();

    ITENS_POR_PAGINA = calculaItensPorPagina(entrada->quantidade_registros);

    tabela->qtde_indices = 0;

    pagina = alocarRegistros(ITENS_POR_PAGINA);

    if(pagina == NULL)
        return -1;

    contador = 0;
    // Cria uma tabela de indices
    while((contador += fread(pagina, sizeof(Registro), ITENS_POR_PAGINA, arq_bin)) <= entrada->quantidade_registros)
    {
        tabela->indices[tabela->qtde_indices++] = pagina[0].chave;
        metricas->n_leitura_pre_processamento++;
    }

    fread(pagina, sizeof(Registro), entrada->quantidade_registros % ITENS_POR_PAGINA, arq_bin);

    desalocarRegistros(&pagina);

    fim = clock();

    metricas->t_pre_processamento += ((double) (fim - inicio)) / CLOCKS_PER_SEC;
    // --- FIM PRE-PROCESSAMENTO --- //

    return 1;
}

static bool pesquisaBinaria(Registro *pagina, int tamanho, Entrada *entrada, Registro *reg_retorno, Metrica *metricas)
{
    int esq, dir, meio;

    esq = 0;
    dir = tamanho - 1;
    
    // Se o vetor esta ordenado, realiza a pesquisa binaria canonica.
    if(entrada->situacao == 1)
    {
        while(esq <= dir)
        {
            metricas->n_comparacoes_pesquisa++;
            meio = (esq + dir) / 2;

            if(entrada->chave_buscada > pagina[meio].chave)
                esq = meio + 1;
            else if(entrada->chave_buscada < pagina[meio].chave)
                dir = meio - 1;
            else
            {
                *reg_retorno = pagina[meio];
                return true;
            }
        }
    }
    // Se o vetor esta desordenado, realiza a pesquisa binaria alternada
    else
    {
        while(esq <= dir)
        {
            meio = (esq + dir) / 2;

            if(entrada->chave_buscada < pagina[meio].chave)
                esq = meio + 1;
            else if(entrada->chave_buscada > pagina[meio].chave)
                dir = meio - 1;
            else
                return true;
        }
    }

    return false;
}

static short pesquisa(FILE *arq_bin, Entrada *entrada, Tabela *tabela, Registro *reg_retorno, Metrica *metricas)
{
    short retorno_funcao;
    long deslocamento, qtde_leitura_itens;
    int indice_pagina;
    Registro *pagina;

    indice_pagina = 0;

    ITENS_POR_PAGINA = calculaItensPorPagina(entrada->quantidade_registros);

    // Se o arquivo esta ordenado ascendentemente
    if(entrada->situacao == 1)
    {
        while(indice_pagina < tabela->qtde_indices && entrada->chave_buscada >= tabela->indices[indice_pagina])
        {
            indice_pagina++;
            metricas->n_comparacoes_pesquisa++;
        }

        // Contabiliza a ultima comparacao feita
        if(indice_pagina != 0 && indice_pagina < tabela->qtde_indices)
            metricas->n_comparacoes_pesquisa++;
    }

    // Caso contrario, o arquivo esta ordenado descendentemente
    else
    {
        while(indice_pagina < tabela->qtde_indices && entrada->chave_buscada <= tabela->indices[indice_pagina])
        {
            indice_pagina++;
            metricas->n_comparacoes_pesquisa++;
        }

        // Contabiliza a ultima comparacao feita
        if(indice_pagina != 0 && indice_pagina < tabela->qtde_indices)
            metricas->n_comparacoes_pesquisa++;
    }

    // O item buscado eh menor que o menor item do arquivo, entao ele nao existe no arquivo.
    if(indice_pagina == 0)
        return 0;

    // Se o item a ser buscado esta na ultima pagina
    if(indice_pagina == tabela->qtde_indices)
    {
        qtde_leitura_itens = entrada->quantidade_registros % ITENS_POR_PAGINA;

        if(qtde_leitura_itens == 0)
            qtde_leitura_itens = ITENS_POR_PAGINA;
    }
    else
        qtde_leitura_itens = ITENS_POR_PAGINA;

    if((pagina = alocarRegistros(qtde_leitura_itens)) == NULL)
        return -1;

    deslocamento = (indice_pagina - 1) * ITENS_POR_PAGINA * sizeof(Registro);

    fseek(arq_bin, deslocamento, SEEK_SET);
    fread(pagina, sizeof(Registro), qtde_leitura_itens, arq_bin);
    metricas->n_leitura_pesquisa++;

    // A pesquisa binaria eh feita e retorna true se achar a chave, caso contrario, retorna false.
    retorno_funcao = pesquisaBinaria(pagina, qtde_leitura_itens, entrada, reg_retorno, metricas);

    desalocarRegistros(&pagina);

    return retorno_funcao;
}

short int sequencial(FILE *arq_bin, Tabela *tabela, Entrada *entrada, Registro *reg_retorno, Metrica *metricas)
{
    short int retorno_funcao;
    // Variaveis usadas para metricas
    clock_t inicio;
    clock_t fim;

    // --- INICIO PESQUISA --- //
    inicio = clock();

    retorno_funcao = pesquisa(arq_bin, entrada, tabela, reg_retorno, metricas);

    fim = clock();

    metricas->t_pesquisa += ((double) (fim - inicio)) / CLOCKS_PER_SEC;
    // --- FIM PESQUISA --- //

    return retorno_funcao;
}
#endif