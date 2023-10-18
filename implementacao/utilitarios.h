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
bool lerEntrada(Entrada*, int, char**);

#endif