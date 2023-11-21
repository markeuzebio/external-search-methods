#include "arvoreB.h"

void printArvoreB(Pagina *raiz)
{
    if(raiz == NULL)
        return;

    for(int i = 0 ; i <= raiz->n ; i++)
    {
        printArvoreB(raiz->prox_pagina[i]);

        if(i < raiz->n)
            printf("%d\n", raiz->registros[i].chave);
    }
}

void desalocarArvoreB(Pagina **raiz)
{
    if(*raiz == NULL)
        return;

    for(int i = 0 ; i <= (*raiz)->n ; i++)
        desalocarArvoreB(&(*raiz)->prox_pagina[i]);

    free(*raiz);
}

void iniciaArvore(Pagina **raiz)
{
    *raiz = NULL;
}

static bool pesquisa(Pagina *raiz, int chave)
{
    unsigned short int i;

    if(raiz == NULL)
        return false;

    i = 1;

    while(i < raiz->n && chave > raiz->registros[i - 1].chave)
        i++;

    if(chave == raiz->registros[i - 1].chave)
        return true;
    else if(chave > raiz->registros[i - 1].chave)
        return pesquisa(raiz->prox_pagina[i], chave);
    else
        return pesquisa(raiz->prox_pagina[i - 1], chave);
}

void insereNaPagina(Pagina *pagina, Registro *item, Pagina *apontador_direita)
{
    unsigned short int k;

    k = pagina->n;

    while(k > 0 && pagina->registros[k - 1].chave > item->chave)
    {
        pagina->registros[k] = pagina->registros[k - 1];
        pagina->prox_pagina[k + 1] = pagina->prox_pagina[k];
        k--;
    }

    pagina->registros[k] = *item;
    pagina->prox_pagina[k + 1] = apontador_direita;
    pagina->n++;
}

void ins(Pagina *raiz, Registro *item, Pagina **pagina_retorno, Registro *registro_retorno, bool *cresceu)
{
    unsigned short int i;

    if(raiz == NULL)
    {
        *cresceu = true;
        *registro_retorno = *item;
        *pagina_retorno = NULL;
        return;
    }

    i = 1;

    while(i < raiz->n && item->chave > raiz->registros[i - 1].chave)
        i++;

    if(raiz->registros[i - 1].chave > item->chave)
        ins(raiz->prox_pagina[i - 1], item, pagina_retorno, registro_retorno, cresceu);
    else
        ins(raiz->prox_pagina[i], item, pagina_retorno, registro_retorno, cresceu);

    if(*cresceu)
    {
        if(raiz->n < ORDEM_MM)
        {
            insereNaPagina(raiz, registro_retorno, *pagina_retorno);
            *cresceu = false;
            return;
        }

        // Processo de criar uma nova pagina e dividr os itens entre a original e ela
        Pagina *nova_pagina = (Pagina*) malloc(sizeof(Pagina));

        if(nova_pagina != NULL)
        {
            nova_pagina->n = 0;
            nova_pagina->prox_pagina[0] = NULL;

            // Verifica se o registro retornado a ser inserido estara contido na pagina original
            if(i <= ORDEM_M + 1)
            {
                // Insere o ultimo item da pagina original na pagina nova
                insereNaPagina(nova_pagina, &raiz->registros[ORDEM_MM - 1], raiz->prox_pagina[ORDEM_MM]);
                // Insere o registro retornado na pagina original
                raiz->n--;
                insereNaPagina(raiz, registro_retorno, *pagina_retorno);
            }
            // O registro retornado a ser inserido estara na pagina nova
            else
                insereNaPagina(nova_pagina, registro_retorno, *pagina_retorno);

            // Move a segunda metade (tirando o elemento do meio) da pagina original para a pagina nova
            for(int j = ORDEM_M + 1 ; j < ORDEM_MM ; j++)
                insereNaPagina(nova_pagina, &raiz->registros[j], raiz->prox_pagina[j + 1]);

            // Altera o apontador mais aa esquerda da nova pagina
            nova_pagina->prox_pagina[0] = raiz->prox_pagina[ORDEM_M + 1];
            // Altera informacoes da pagina original, diminuindo a quantidade de elementos para a metade
            raiz->n = ORDEM_M;
            // Repassa o elemento do meio para a chamada recursiva anterior, fazendo com que esse elemento seja inserido em paginas superiores
            *registro_retorno = raiz->registros[ORDEM_M];
            *pagina_retorno = nova_pagina;
        }
        else
            exit(1);
    }
}

void inserir(Pagina **raiz, Registro *item)
{
    bool cresceu;
    Registro registro_retornado;
    Pagina *pagina_retornada;

    ins(*raiz, item, &pagina_retornada, &registro_retornado, &cresceu);

    if(cresceu)
    {
        Pagina *nova_raiz = (Pagina*) malloc(sizeof(Pagina));

        if(nova_raiz != NULL)
        {
            nova_raiz->n = 1;
            nova_raiz->prox_pagina[0] = *raiz;
            nova_raiz->prox_pagina[1] = pagina_retornada;
            nova_raiz->registros[0] = registro_retornado;
            *raiz = nova_raiz;
        }
        else
            exit(1);
    }
}

Pagina* gerarArvoreB(FILE *arq_bin, Entrada *entrada)
{
    /* 
        Calcula a quantidade maxima de itens que podem ser lidos de uma
        vez do arquivo oferecido. Essa quantidade maxima de itens nao
        pode exceder 128 MB em memoria principal.
    */
    const unsigned int ITENS_POR_PAGINA = 128 * 1024 * 1024 / sizeof(Registro);
    unsigned long i;
    Registro *registros;
    Pagina *raiz;

    iniciaArvore(&raiz);
    
    if((registros = (Registro*) malloc(ITENS_POR_PAGINA * sizeof(Registro))) == NULL)
        return NULL;

    i = 0;

    fread(registros, sizeof(Registro), ITENS_POR_PAGINA, arq_bin);
    while(i < entrada->quantidade_registros)
    {
        if(!(i % ITENS_POR_PAGINA == 0 && i != 0))
            fread(registros, sizeof(Registro), ITENS_POR_PAGINA, arq_bin);

        inserir(&raiz, &registros[i % ITENS_POR_PAGINA]);
        i++;
    }

    desalocarRegistros(&registros);

    return raiz;
}

bool arvoreB(Pagina *raiz, int chave)
{
    return pesquisa(raiz, chave);
}