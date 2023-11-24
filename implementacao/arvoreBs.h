#ifndef __ARVORE_B_S__
#define __ARVORE_B_S__

#include <stdlib.h>

#include "utilitarios.h"

#define ORDEM_M_ 2
#define ORDEM_MM_ 4

enum tipo_pagina {
    INDICE,
    FOLHA
};

typedef struct pagina_ {
    enum tipo_pagina tp;

    union {
        struct {
            int n;
            int indices[ORDEM_MM_];
            struct pagina_ *prox_pagina[ORDEM_MM_ + 1];
        } Indice;
        struct {
            int n;
            Registro registros[ORDEM_MM_];
            struct pagina_ *prox;
        } Folha;
    } Uniao;
} Pagina_;

bool arvoreB_(Pagina_ *raiz, int chave, Metrica*);
Pagina_* gerarArvoreB_(FILE*, Entrada*, Metrica*);
void printArvoreB_(Pagina_*);
void desalocarArvoreB_(Pagina_**);

#endif