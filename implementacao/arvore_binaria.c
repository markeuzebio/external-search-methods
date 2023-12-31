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

void printArvoreBinaria(FILE* arq_arv_bin, unsigned int tam_arquivo)
{
    Registro registro;
    unsigned int contador;

    contador = 0;
    while((contador += fread(&registro, sizeof(Registro), 1, arq_arv_bin)) <= tam_arquivo)
        printf("%d\n", registro.chave);

    rewind(arq_arv_bin);
}

bool arquivoEstaVazio(FILE *arq)
{
    fseek(arq, 0, SEEK_END);

    return ftell(arq) == 0;
}

void alterarApontadores(FILE *arq_arv_bin, int chave_novo_item, unsigned long posicao_no_filho, Metrica *metricas)
{
    long posicao_no_pai;
    No item_atual;

    posicao_no_pai = 0;

    while(true)
    {
        fseek(arq_arv_bin, posicao_no_pai * sizeof(No), SEEK_SET);
        fread(&item_atual, sizeof(No), 1, arq_arv_bin);
        metricas->n_leitura_pre_processamento++;

        metricas->n_comparacoes_pre_processamento++;
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

void inserirRegistroNoArquivoAsc(FILE *arq_arv_bin, No *item, Metrica *metricas)
{
    No no_pai;

    // Insere o no mais recente no arquivo
    fwrite(item, sizeof(No), 1, arq_arv_bin);

    /* Conjunto de operacoes utilizados para pegar o no pai do no mais recentemente inserido */
    fseek(arq_arv_bin, -2 * sizeof(No), SEEK_CUR);
    fread(&no_pai, sizeof(No), 1, arq_arv_bin);
    metricas->n_leitura_pre_processamento++;

    // Atualiza o apontador direito do no pai, fazendo com que esse apontador aponte para o no mais recentemente inserido
    no_pai.dir = ftell(arq_arv_bin);

    // Atualiza o registro do no pai
    fseek(arq_arv_bin, -sizeof(No), SEEK_CUR);
    fwrite(&no_pai, sizeof(No), 1, arq_arv_bin);
    fseek(arq_arv_bin, 0, SEEK_END);
}

void inserirRegistroNoArquivoDesc(FILE *arq_arv_bin, No *item, Metrica *metricas)
{
    No no_pai;

    // Insere o no mais recente no arquivo
    fwrite(item, sizeof(No), 1, arq_arv_bin);

    /* Conjunto de operacoes utilizados para pegar o no pai do no mais recentemente inserido */
    fseek(arq_arv_bin, -2 * sizeof(No), SEEK_CUR);
    fread(&no_pai, sizeof(No), 1, arq_arv_bin);
    metricas->n_leitura_pre_processamento++;

    // Atualiza o apontador esquerdo do no pai, fazendo com que esse apontador aponte para o no mais recentemente inserido
    no_pai.esq = ftell(arq_arv_bin);

    // Atualiza o registro do no pai
    fseek(arq_arv_bin, -sizeof(No), SEEK_CUR);
    fwrite(&no_pai, sizeof(No), 1, arq_arv_bin);
    fseek(arq_arv_bin, 0, SEEK_END);
}

void inserirRegistroNoArquivoRand(FILE *arq_arv_bin, No *item, Metrica *metricas)
{
    fwrite(item, sizeof(No), 1, arq_arv_bin);
    alterarApontadores(arq_arv_bin, item->registro.chave, ftell(arq_arv_bin) - sizeof(No), metricas);
    fseek(arq_arv_bin, 0, SEEK_END);
}


FILE* geraArquivoArvoreBinaria(FILE *arq_bin, Registro pagina[], Entrada *entrada, FILE *arq_arv_bin, Metrica *metricas)
{
    int i;
    No item_arquivo_gerado;

    i = 0;

    while(i < entrada->quantidade_registros)
    {
        fread(pagina, sizeof(Registro), ITENS_POR_PAGINA, arq_bin);
        metricas->n_leitura_pre_processamento++;

        /*
            Se o arquivo gerado esta vazio, preenche-o com um registro; isso eh importante
            para que a logica restante do programa nao seja violada.
        */
        if(arquivoEstaVazio(arq_arv_bin))
        {
            item_arquivo_gerado.dir = item_arquivo_gerado.esq = -1;
            item_arquivo_gerado.registro = pagina[i % ITENS_POR_PAGINA];
            fwrite(&item_arquivo_gerado, sizeof(No), 1, arq_arv_bin);
            i++;
        }
        
        if(entrada->situacao == 1)
        {
            while(i < ITENS_POR_PAGINA && i < entrada->quantidade_registros)
            {
                item_arquivo_gerado.dir = item_arquivo_gerado.esq = -1;
                item_arquivo_gerado.registro = pagina[i % ITENS_POR_PAGINA];
                inserirRegistroNoArquivoAsc(arq_arv_bin, &item_arquivo_gerado, metricas);
                i++;
            }
        }
        else if(entrada->situacao == 2)
        {
            while(i < ITENS_POR_PAGINA && i < entrada->quantidade_registros)
            {
                item_arquivo_gerado.dir = item_arquivo_gerado.esq = -1;
                item_arquivo_gerado.registro = pagina[i % ITENS_POR_PAGINA];
                inserirRegistroNoArquivoDesc(arq_arv_bin, &item_arquivo_gerado, metricas);
                i++;
            }
        }
        // O arquivo esta ordenado randomicamente
        else
        {
            while(i < ITENS_POR_PAGINA && i < entrada->quantidade_registros)
            {
                item_arquivo_gerado.dir = item_arquivo_gerado.esq = -1;
                item_arquivo_gerado.registro = pagina[i % ITENS_POR_PAGINA];
                inserirRegistroNoArquivoRand(arq_arv_bin, &item_arquivo_gerado, metricas);
                i++;
            }
        }
    }

    return arq_arv_bin;
}

short int arvoreBinariaGerar(FILE *arq_bin, FILE *arq_arv_bin, Entrada *entrada, Metrica *metricas)
{
    Registro *pagina;
    // Variaveis usadas para metricas
    clock_t inicio;
    clock_t fim;

    // --- INICIO PRE-PROCESSAMENTO --- //
    inicio = clock();

    if((pagina = alocarRegistros(ITENS_POR_PAGINA)) == NULL)
        return 0;

    geraArquivoArvoreBinaria(arq_bin, pagina, entrada, arq_arv_bin, metricas);

    desalocarRegistros(&pagina);

    fim = clock();

    metricas->t_pre_processamento += ((double) fim - inicio) / CLOCKS_PER_SEC;
    // --- FIM PRE-PROCESSAMENTO --- //

    return 1;
}

bool arvoreBinaria(FILE *arq_arv_bin, int chave, Metrica *metricas)
{
    long int posicao_arquivo;
    No item;
    // Variaveis usadas para metricas
    clock_t inicio;
    clock_t fim;

    // --- INICIO PESQUISA --- //
    inicio = clock();

    rewind(arq_arv_bin);

    posicao_arquivo = 0;

    while(true)
    {
        fseek(arq_arv_bin, posicao_arquivo, SEEK_SET);
        fread(&item, sizeof(No), 1, arq_arv_bin);
        metricas->n_leitura_pesquisa++;

        metricas->n_comparacoes_pesquisa++;
        if(item.registro.chave == chave)
        {
            fim = clock();
            metricas->t_pesquisa += ((double) fim - inicio) / CLOCKS_PER_SEC;
            // --- FIM PESQUISA --- //

            return true;
        }
        else
        {
            if(item.registro.chave > chave)
            {
                if(item.esq == -1)
                {
                    fim = clock();
                    metricas->t_pesquisa += ((double) fim - inicio) / CLOCKS_PER_SEC;
                    // --- FIM PESQUISA --- //

                    return false;
                }
                
                posicao_arquivo = item.esq;
            }
            else
            {
                if(item.dir == -1)
                {
                    fim = clock();
                    metricas->t_pesquisa += ((double) fim - inicio) / CLOCKS_PER_SEC;
                    // --- FIM PESQUISA --- //

                    return false;
                }

                posicao_arquivo = item.dir;
            }
        }
    }
}