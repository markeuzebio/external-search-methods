#ifndef  __SEQUENCIAL__
#define __SEQUENCIAL__

#include <stdio.h>
#include <stdlib.h>
#include "sequencial.h"
#include "utilitarios.h"

Registro *alocarPagina(int qtde_registros_arquivo)
{
    Registro *registros = (Registro *) malloc(qtde_registros_arquivo * sizeof(Registro));

    return registros;
}

int calculaItensPorPagina(int qtde_registros_arquivo)
{
    return (qtde_registros_arquivo / MAXTABELA) + 1;
}

void mergesort(int* v, int l, int r){
    if(l<r){
        int m = (l + r)/2;
        mergesort(v,l,m);
        mergesort(v, m+1, r);
        merge(v,l,m,r);
    }
}

int sequencial(Registro tab[], int qtde_registros_arquivo, Entrada *item, FILE *arq)
{ //
    int ITENSPAGINA;
    int i, quant_itens;
    long desloc;
    Registro pagina;

    ITENSPAGINA = calculaItensPorPagina(qtde_registros_arquivo);


    i = 0;
    while (i < qtde_registros_arquivo && tab[i].chave <= item->chave)
        i++;

    if (i == 0)
        return 0;
    else
    {
        if (i < qtde_registros_arquivo)
            quant_itens = ITENSPAGINA;
        else
        {
            fseek(arq, 0, SEEK_END);
            quant_itens = (ftell(arq) / sizeof(Entrada)) % ITENSPAGINA;
            if (quant_itens == 0)
                quant_itens = ITENSPAGINA;
        }

        desloc = (tab[i-1].dado1-1) * ITENSPAGINA * sizeof(Entrada);
        fseek (arq, desloc, SEEK_SET);
        fread (&pagina, sizeof(Entrada), quant_itens, arq);


    }
}
#endif