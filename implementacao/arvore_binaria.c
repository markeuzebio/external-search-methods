#include <stdio.h>
#include <stdlib.h>

#include "arvore_binaria.h"

/*
    Define a quantidade maxima de itens que uma pagina para o arquivo
    "registros.bin" pode possuir. No caso, a quantidade deve ser
    de tal modo que ocupe, no maximo, 128 MB.

    A paginacao eh feita aqui de modo a diminuir a quantidade de
    fluxo de transferencia de dados entre a RAM e o arquivo
    a ser lido (operacao de leitura).
*/
static const unsigned int ITENS_POR_PAGINA = (128 * 1024 * 1024) / sizeof(Registro);

bool arquivoEstaVazio(FILE *arq)
{
    fseek(arq, 0, SEEK_END);

    return ftell(arq) == 0;
}

void alterarApontadores(FILE *arq_arv_bin, int chave_novo_item, unsigned long posicao_no_filho)
{
    long posicao_no_pai;
    No item_atual;

    posicao_no_pai = 0;

    while(true)
    {
        fseek(arq_arv_bin, posicao_no_pai * sizeof(No), SEEK_SET);
        fread(&item_atual, sizeof(No), 1, arq_arv_bin);

        if(item_atual.registro.chave > chave_novo_item)
        {
            // O pai eh o item corrente.
            if(item_atual.esq == -1)
            {
                item_atual.esq = posicao_no_filho;

                // Volta para o registro anterior
                fseek(arq_arv_bin, -sizeof(No), SEEK_CUR);
                // Sobrescreve o registro que representa o no pai
                fwrite(&item_atual, sizeof(No), 1, arq_arv_bin);

                posicao_no_pai++;

                break;
            }
            else
                posicao_no_pai = item_atual.esq / sizeof(No);
        }
        else
        {
            // O pai eh o item corrente.
            if(item_atual.dir == -1)
            {
                item_atual.dir = posicao_no_filho;

                // Volta para o registro anterior
                fseek(arq_arv_bin, -sizeof(No), SEEK_CUR);
                // Sobrescreve o registro que representa o no pai
                fwrite(&item_atual, sizeof(No), 1, arq_arv_bin);

                posicao_no_pai++;

                break;
            }
            else
                posicao_no_pai = item_atual.dir / sizeof(No);
        }
    }
}

void inserirRegistroNoArquivo(FILE *arq_arv_bin, No *item)
{
    fwrite(item, sizeof(No), 1, arq_arv_bin);
    alterarApontadores(arq_arv_bin, item->registro.chave, ftell(arq_arv_bin) - sizeof(No));
    fseek(arq_arv_bin, 0, SEEK_END);
}

FILE* geraArquivoArvoreBinaria(FILE *arq_bin, Registro pagina[], int quantidade_registros, FILE *arq_arv_bin)
{
    int i;
    No item_arquivo_gerado;

    while(fread(pagina, sizeof(Registro), ITENS_POR_PAGINA, arq_bin) != 0)
    {
        i = 0;

        /*
            Se o arquivo gerado esta vazio, preenche-o com um registro; isso eh importante
            para que a logica restante do programa nao seja violada.
        */
        if(arquivoEstaVazio(arq_arv_bin))
        {
            item_arquivo_gerado.dir = item_arquivo_gerado.esq = -1;
            item_arquivo_gerado.registro = pagina[i++];
            fwrite(&item_arquivo_gerado, sizeof(No), 1, arq_arv_bin);
        }
        
        while(i < ITENS_POR_PAGINA && i < quantidade_registros)
        {
            item_arquivo_gerado.dir = item_arquivo_gerado.esq = -1;
            item_arquivo_gerado.registro = pagina[i++];
            inserirRegistroNoArquivo(arq_arv_bin, &item_arquivo_gerado);
        }
    }

    return arq_arv_bin;
}

short int arvoreBinariaGerar(FILE *arq_bin, FILE *arq_arv_bin, int quantidade_registros)
{
    Registro *pagina;

    if((pagina = alocarRegistros(ITENS_POR_PAGINA)) == NULL)
        return 0;

    geraArquivoArvoreBinaria(arq_bin, pagina, quantidade_registros, arq_arv_bin);

    desalocarRegistros(&pagina);

    return 1;
}

bool arvoreBinaria(FILE *arq_arv_bin, int chave)
{
    long int posicao_arquivo;
    No item;

    rewind(arq_arv_bin);

    posicao_arquivo = 0;

    while(true)
    {
        fseek(arq_arv_bin, posicao_arquivo, SEEK_SET);
        fread(&item, sizeof(No), 1, arq_arv_bin);

        if(item.registro.chave == chave)
            return true;
        else
        {
            if(item.registro.chave > chave)
            {
                if(item.esq == -1)
                    return false;
                
                posicao_arquivo = item.esq;
            }
            else
            {
                if(item.dir == -1)
                    return false;

                posicao_arquivo = item.dir;
            }
        }
    }
}