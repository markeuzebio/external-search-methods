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

short preencheTabela(FILE *arq_bin, Tabela* tabela)
{
    Registro *pagina;

    tabela->qtde_indices = 0;

    pagina = alocarRegistros(ITENS_POR_PAGINA);

    if(pagina == NULL)
        return -1;

    // Cria uma tabela de indices
    while(fread(pagina, sizeof(Registro), ITENS_POR_PAGINA, arq_bin) != 0)
        tabela->indices[tabela->qtde_indices++] = pagina[0].chave;

    desalocarRegistros(&pagina);

    return 1;
}

bool pesquisaBinaria(Registro *pagina, int tamanho, Entrada *entrada)
{
    int esq, dir, meio;

    esq = 0;
    dir = tamanho - 1;
    
    // Se o vetor esta ordenado, realiza a pesquisa binaria canonica.
    if(entrada->situacao == 1)
    {
        while(esq <= dir)
        {
            meio = (esq + dir) / 2;

            if(entrada->chave_buscada > pagina[meio].chave)
                esq = meio + 1;
            else if(entrada->chave_buscada < pagina[meio].chave)
                dir = meio - 1;
            else
                return true;
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

short pesquisa(FILE *arq_bin, Entrada *entrada, Tabela *tabela)
{
    short retorno_funcao;
    long deslocamento, qtde_leitura_itens;
    int indice_pagina;
    Registro *pagina;

    indice_pagina = 0;

    // Se o arquivo esta ordenado ascendentemente
    if(entrada->situacao == 1)
        while(indice_pagina < tabela->qtde_indices && entrada->chave_buscada >= tabela->indices[indice_pagina])
            indice_pagina++;
    // Caso contrario, o arquivo esta ordenado descendentemente
    else
        while(indice_pagina < tabela->qtde_indices && entrada->chave_buscada <= tabela->indices[indice_pagina])
            indice_pagina++;

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

    // A pesquisa binaria eh feita e retorna true se achar a chave, caso contrario, retorna false.
    retorno_funcao = pesquisaBinaria(pagina, qtde_leitura_itens, entrada);

    desalocarRegistros(&pagina);

    return retorno_funcao;
}

int sequencial(FILE *arq_bin, Entrada *entrada)
{
    Tabela tabela;

    ITENS_POR_PAGINA = calculaItensPorPagina(entrada->quantidade_registros);

    if(preencheTabela(arq_bin, &tabela) == -1)
        return -1;

    return pesquisa(arq_bin, entrada, &tabela);
}
#endif