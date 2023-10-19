#ifndef  __SEQUENCIAL__
#define __SEQUENCIAL__

#include <stdio.h>
#include <stdlib.h>
#include "sequencial.h"
#include "utilitarios.h"

Registro *alocarPagina(int qtde_itens_por_pagina)
{
    Registro *registros = (Registro *) malloc(qtde_itens_por_pagina * sizeof(Registro));

    return registros;
}

int calculaItensPorPagina(int qtde_registros_arquivo)
{
    return (qtde_registros_arquivo / MAX_TABELA) + 1;
}

short preencheTabela(FILE *arq_bin, Tabela* tabela)
{
    Registro *pagina;

    tabela->qtde_indices = 0;

    pagina = alocarPagina(ITENS_POR_PAGINA);

    if(pagina == NULL)
        return -1;

    // Cria uma tabela de indices
    while(fread(pagina, sizeof(Registro), ITENS_POR_PAGINA, arq_bin) != 0)
        tabela->indices[tabela->qtde_indices++] = pagina[0].chave;

    return 1;
}

bool pesquisa(FILE *arq_bin, Entrada *entrada, Tabela *tabela);

int sequencial(FILE *arq_bin, Entrada *entrada, Registro *registro_saida)
{
    Tabela tabela;

    ITENS_POR_PAGINA = calculaItensPorPagina(entrada->quantidade_registros);

    if(preencheTabela(arq_bin, &tabela) == -1)
        return -1;

    return 1;
}
#endif