#ifndef __SEQUENCIAL__
#define __SEQUENCIAL__

#include <stdio.h>
#include "utilitarios.h"

/*
    RETURN
    [
        -1: Se houver erro ao alocar a memoria dinamicamente.
        0: A chave passada como entrada nao existe no arquivo binario.
        1: A chave passada como entrada existe no arquivo binario.
    ]
*/
short int sequencial(FILE*, Tabela*, Entrada*, Registro*, Metrica*);

/*
    RETURN
    [
        -1: Erro de alocacao dinamica de memoria.
        1: Tabela preenchida com sucesso.
    ]
*/
short preencheTabela(FILE*, Tabela*, Entrada*, Metrica*);
void printSequencial(FILE*, Tabela*, Entrada*);

#endif