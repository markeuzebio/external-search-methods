#ifndef __UTILITARIOS__
#define __UTILITARIOS__

#include <time.h>
#include <stdio.h>
#include <stdbool.h>

#define TAM_TITULO 5001
#define MAX_TABELA 100      // Numero arbitrario

// -------------- QUESTOES DE TESTE E METRICAS -------------- //
typedef struct {
    double t_pre_processamento;
    double t_pesquisa;
    unsigned long int n_leitura_pre_processamento;
    unsigned long int n_leitura_pesquisa;
    unsigned long int n_comparacoes_pre_processamento;
    unsigned long int n_comparacoes_pesquisa;
} Metrica;
// -------------- QUESTOES DE TESTE E METRICAS -------------- //

typedef struct {
    int indices[MAX_TABELA];
    int qtde_indices;
} Tabela;

typedef struct {
    int chave;
    long int valor;
    char string[TAM_TITULO];
} Registro;

/*
    As variaveis "esq" e "dir", do tipo long,
    dizem respeito aa posicoes dentro do arquivo
    as quais guardam outros registros.
*/
typedef struct no {
    Registro registro;
    long esq;
    long dir;
} No;

typedef struct {
    short int metodo;
    unsigned int quantidade_registros;
    short int situacao;
    int chave_buscada;
    char opcional[3];
} Entrada;

Registro* alocarRegistros(int);
void desalocarRegistros(Registro**);
void resetaMetricas(Metrica*);
void exibirMetricas(Metrica*);

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