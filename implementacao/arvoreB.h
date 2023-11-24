#ifndef __ARVORE_B__
#define __ARVORE_B__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "utilitarios.h"

#define ORDEM_M 2
#define ORDEM_MM 4

/*
    n: quantidade de itens na pagina corrente.
    registros: itens lidos de um arquivo e armazenados no vetor.
    prox_pagina: vetor de ponteiros que apontam para proximas paginas filhas da atual.
*/
typedef struct pagina {
    unsigned short int n; 
    Registro registros[ORDEM_MM];
    struct pagina* prox_pagina[ORDEM_MM + 1];
} Pagina;

void printArvoreB(Pagina *raiz);
Pagina* gerarArvoreB(FILE*, Entrada*, Metrica*);
void desalocarArvoreB(Pagina**);
bool arvoreB(Pagina *raiz, int chave, Metrica*);
#endif