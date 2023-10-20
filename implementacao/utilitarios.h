#ifndef __UTILITARIOS__
#define __UTILITARIOS__

#include <stdbool.h>

#define TAM_TITULO 5001
#define MAX_TABELA 100      // Numero arbitrario

int long ITENS_POR_PAGINA;

typedef struct {
    int indices[MAX_TABELA];
    int qtde_indices;
} Tabela;

typedef struct {
    int chave;
    long int valor;
    char string[TAM_TITULO];
} Registro;

typedef struct {
    short int metodo;
    unsigned int quantidade_registros;
    short int situacao;
    int chave_buscada;
    char opcional[3];
} Entrada;

Registro* alocarRegistros(int);
void desalocarRegistros(Registro**);

/*
    RETURN
    [
        false: se a entrada oferecida nao eh valida
        true: se a entrada oferecida eh valida
    ]

    Sobre os valores dos parametros da struct entrada
    {
        Entrada->metodo:
        [
            (1): Acesso Sequencial Indexado
            (2): Arvore Binaria de Pesquisa
            (3): Arvore B
            (4): Arvore B*
        ]

        Entrada->quantidade_registros
        [
            Quantidade de registros ("linhas") que o arquivo
            de entrada possui.
        ]

        Entrada->situacao:
        [
            Ordenacao que o arquivo de entrada apresenta:

            (1): ordenado ascendentemente
            (2): ordenado descendentemente
            (3): ordenado aleatoriamente
        ]

        Entrada->chave_buscada:
        [
            Eh a chave buscada dentro do arquivo binario.
        ]

        Entrada->opcional
        [
            Entrada opcional de valor "-p" ou "-P" cuja funcao
            eh exibir o arquivo binario.
        ]
    }
*/
bool lerEntrada(Entrada*, int, char**);

#endif