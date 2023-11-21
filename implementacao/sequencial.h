#ifndef __SEQUENCIAL__
#define __SEQUENCIAL__

#include <stdio.h>
#include "utilitarios.h"

/*
    RETURN
    [
        -2: Se houve erro na tentativa de abrir um arquivo binario para escrita.
        -1: Se houver erro ao alocar a memoria dinamicamente.
        0: A chave passada como entrada nao existe no arquivo binario.
        1: A chave passada como entrada existe no arquivo binario.
    ]
*/
short int sequencial(FILE*, Entrada*, Registro*);

#endif