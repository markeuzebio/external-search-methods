#include "arvoreBs.h"

void printArvoreB_(Pagina_ *raiz)
{
    if(raiz->tp == FOLHA)
    {
        while(raiz != NULL)
        {
            for(int i = 0 ; i < raiz->Uniao.Folha.n ; i++)
                printf("%d\n", raiz->Uniao.Folha.registros[i].chave);

            raiz = raiz->Uniao.Folha.prox;
        }
    }
    else
        return printArvoreB_(raiz->Uniao.Indice.prox_pagina[0]);
}

void desalocarArvoreB_(Pagina_ **raiz)
{
    if((*raiz)->Uniao.Indice.prox_pagina[0]->tp == FOLHA)
    {
        for(int i = 0 ; i <= (*raiz)->Uniao.Indice.n ; i++)
            free((*raiz)->Uniao.Indice.prox_pagina[i]);

        free(*raiz);
    }
    else
    {
        for(int i = 0 ; i <= (*raiz)->Uniao.Indice.n ; i++)
            desalocarArvoreB_(&(*raiz)->Uniao.Indice.prox_pagina[i]);

        free(*raiz);
    }
}

void iniciaArvore_(Pagina_ **raiz)
{
    *raiz = NULL;
}

void insereRegistroNaPagina_(Pagina_ *pagina, Registro *registro)
{
    short int k;

    k = pagina->Uniao.Folha.n;

    while(k > 0 && pagina->Uniao.Folha.registros[k - 1].chave > registro->chave)
    {
        pagina->Uniao.Folha.registros[k] = pagina->Uniao.Folha.registros[k - 1];
        k--;
    }

    pagina->Uniao.Folha.registros[k] = *registro;
    pagina->Uniao.Folha.n++;
}

void insereIndiceNaPagina_(Pagina_ *pagina, int chave, Pagina_ *apontador_direita)
{
    short int k;

    k = pagina->Uniao.Indice.n;

    while(k > 0 && pagina->Uniao.Indice.indices[k - 1] > chave)
    {
        pagina->Uniao.Indice.indices[k] = pagina->Uniao.Indice.indices[k - 1];
        pagina->Uniao.Indice.prox_pagina[k + 1] = pagina->Uniao.Indice.prox_pagina[k];
        k--;
    }

    pagina->Uniao.Indice.indices[k] = chave;
    pagina->Uniao.Indice.prox_pagina[k + 1] = apontador_direita;
    pagina->Uniao.Indice.n++;
}

void ins_(Pagina_ *raiz, Registro *item, Pagina_ **pagina_retorno, int *chave_retorno, bool *cresceu)
{
    unsigned short int i;

    if(raiz->tp == FOLHA)
    {
        if(raiz->Uniao.Folha.n < ORDEM_MM_)
        {
            insereRegistroNaPagina_(raiz, item);
            *cresceu = false;
            return;
        }

        // Processo de criar uma nova pagina e dividr os itens entre a original e ela
        Pagina_ *nova_pagina = (Pagina_*) malloc(sizeof(Pagina_));

        if(nova_pagina != NULL)
        {
            nova_pagina->tp = FOLHA;
            nova_pagina->Uniao.Folha.n = 0;

            // Verifica se o item a ser inserido estara contido na pagina original
            if(item->chave <= raiz->Uniao.Folha.registros[ORDEM_M_].chave)
            {
                // Insere o ultimo item da pagina original na pagina nova
                insereRegistroNaPagina_(nova_pagina, &raiz->Uniao.Folha.registros[ORDEM_MM_ - 1]);
                // Insere o item na pagina original
                raiz->Uniao.Folha.n--;
                insereRegistroNaPagina_(raiz, item);
            }
            // O item estara contido na nova pagina
            else
                insereRegistroNaPagina_(nova_pagina, item);

            // Passa os itens da metade em diante da pagina original para a pagina nova
            for(int j = ORDEM_M_ ; j < ORDEM_MM_ ; j++)
                insereRegistroNaPagina_(nova_pagina, &raiz->Uniao.Folha.registros[j]);

            *cresceu = true;
            // Altera informacoes da pagina original, diminuindo a quantidade de elementos para a metade
            raiz->Uniao.Folha.n = ORDEM_M_;
            // Repassa o elemento do meio para a chamada recursiva anterior, fazendo com que esse elemento seja inserido em paginas superiores
            *chave_retorno = raiz->Uniao.Folha.registros[ORDEM_M_].chave;
            *pagina_retorno = nova_pagina;

            // Atualiza os ponteiros da lista encadeada
            nova_pagina->Uniao.Folha.prox = raiz->Uniao.Folha.prox;
            raiz->Uniao.Folha.prox = nova_pagina;
        }
        else
            exit(1);

        return;
    }

    i = 1;

    while(i < raiz->Uniao.Indice.n && item->chave > raiz->Uniao.Indice.indices[i - 1])
        i++;

    if(raiz->Uniao.Indice.indices[i - 1] > item->chave)
        ins_(raiz->Uniao.Indice.prox_pagina[i - 1], item, pagina_retorno, chave_retorno, cresceu);
    else
        ins_(raiz->Uniao.Indice.prox_pagina[i], item, pagina_retorno, chave_retorno, cresceu);

    if(*cresceu)
    {
        if(raiz->Uniao.Indice.n < ORDEM_MM_)
        {
            insereIndiceNaPagina_(raiz, *chave_retorno, *pagina_retorno);
            *cresceu = false;
            return;
        }

        // Processo de criar uma pagina nova e dividr os itens entre a original e ela
        Pagina_ *nova_pagina = (Pagina_*) malloc(sizeof(Pagina_));

        if(nova_pagina != NULL)
        {
            nova_pagina->tp = INDICE;
            nova_pagina->Uniao.Indice.n = 0;

            // Verifica se o indice retornado a ser inserido estara contido na pagina original
            if(i <= ORDEM_M_ + 1)
            {
                // Insere o ultimo indice da pagina original na pagina nova
                insereIndiceNaPagina_(nova_pagina, raiz->Uniao.Indice.indices[ORDEM_MM_ - 1], raiz->Uniao.Indice.prox_pagina[ORDEM_MM_]);
                // Insere o indice retornado na pagina original
                raiz->Uniao.Indice.n--;
                insereIndiceNaPagina_(raiz, *chave_retorno, *pagina_retorno);
            }
            // O indice retornado estara contido na pagina nova
            else
                insereIndiceNaPagina_(nova_pagina, *chave_retorno, *pagina_retorno);

            // Passa os indices da metade em diante da pagina original para a pagina nova
            for(int j = ORDEM_M_ + 1 ; j < ORDEM_MM_ ; j++)
                insereIndiceNaPagina_(nova_pagina, raiz->Uniao.Indice.indices[j], raiz->Uniao.Indice.prox_pagina[j + 1]);

            // Altera o apontador mais aa esquerda da nova pagina
            nova_pagina->Uniao.Indice.prox_pagina[0] = raiz->Uniao.Indice.prox_pagina[ORDEM_M_ + 1];
            // Altera informacoes da pagina original, diminuindo a quantidade de elementos para a metade
            raiz->Uniao.Indice.n = ORDEM_M_;
            // Repassa o elemento do meio para a chamada recursiva anterior, fazendo com que esse elemento seja inserido em paginas superiores
            *chave_retorno = raiz->Uniao.Indice.indices[ORDEM_M_];
            *pagina_retorno = nova_pagina;
        }
        else
            exit(1);
    }
}

void inserir_(Pagina_ **raiz, Registro *item)
{
    bool cresceu;
    int chave_retornada;
    Pagina_ *pagina_retornada;

    if(*raiz != NULL)
    {
        ins_(*raiz, item, &pagina_retornada, &chave_retornada, &cresceu);

        if(cresceu)
        {
            Pagina_ *nova_raiz = (Pagina_*) malloc(sizeof(Pagina_));

            if(nova_raiz != NULL)
            {
                nova_raiz->tp = INDICE;
                nova_raiz->Uniao.Indice.n = 1;
                nova_raiz->Uniao.Indice.prox_pagina[0] = *raiz;
                nova_raiz->Uniao.Indice.prox_pagina[1] = pagina_retornada;
                nova_raiz->Uniao.Indice.indices[0] = chave_retornada;
                *raiz = nova_raiz;
            }
            else
                exit(1);
        }
    }
    else
    {
        Pagina_ *nova_folha = (Pagina_*) malloc(sizeof(Pagina_));

        if(nova_folha != NULL)
        {
            nova_folha->tp = FOLHA;
            nova_folha->Uniao.Folha.n = 1;
            nova_folha->Uniao.Folha.registros[0] = *item;
            nova_folha->Uniao.Folha.prox = NULL;
            *raiz = nova_folha;
        }
        else
            exit(1);
    }
}

static bool pesquisaBinaria(Registro *pagina, int tamanho, int chave)
{
    short int esq, dir, meio;

    esq = 0;
    dir = tamanho - 1;
    
    while(esq <= dir)
    {
        meio = (esq + dir) / 2;

        if(chave > pagina[meio].chave)
            esq = meio + 1;
        else if(chave < pagina[meio].chave)
            dir = meio - 1;
        else
            return true;
    }

    return false;
}

static bool pesquisa(Pagina_ *raiz, int chave)
{
    short int i;

    if(raiz->tp == FOLHA)
        return pesquisaBinaria(raiz->Uniao.Folha.registros, raiz->Uniao.Folha.n, chave);

    i = 1;

    while(i < raiz->Uniao.Indice.n && chave > raiz->Uniao.Indice.indices[i - 1])
        i++;

    if(chave < raiz->Uniao.Indice.indices[i - 1])
        return arvoreB_(raiz->Uniao.Indice.prox_pagina[i - 1], chave);
        
    return arvoreB_(raiz->Uniao.Indice.prox_pagina[i], chave);
}

Pagina_* gerarArvoreB_(FILE *arq_bin, Entrada *entrada)
{
    /* 
        Calcula a quantidade maxima de itens que podem ser lidos de uma
        vez do arquivo oferecido. Essa quantidade maxima de itens nao
        pode exceder 128 MB em memoria principal.
    */
    const unsigned int ITENS_POR_PAGINA = 128 * 1024 * 1024 / sizeof(Registro);
    unsigned long i;
    Registro *registros;
    Pagina_ *raiz;

    iniciaArvore_(&raiz);

    if((registros = (Registro*) malloc(ITENS_POR_PAGINA * sizeof(Registro))) == NULL)
        return NULL;

    i = 0;

    fread(registros, sizeof(Registro), ITENS_POR_PAGINA, arq_bin);
    while(i < entrada->quantidade_registros)
    {
        if(i % ITENS_POR_PAGINA == 0 && i != 0)
            fread(registros, sizeof(Registro), ITENS_POR_PAGINA, arq_bin);

        inserir_(&raiz, &registros[i % ITENS_POR_PAGINA]);
        i++;
    }

    desalocarRegistros(&registros);

    return raiz;
}

bool arvoreB_(Pagina_ *raiz, int chave)
{
    return pesquisa(raiz, chave);
}