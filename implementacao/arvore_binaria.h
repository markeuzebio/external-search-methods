#ifndef __ARVORE_BINARIA__
#define __ARVORE_BINARIA__

#include "utilitarios.h"

short int arvoreBinariaGerar(FILE* arq_bin_entrada, FILE* arq_bin_saida, Entrada*, Metrica*);
bool arvoreBinaria(FILE*, int, Metrica*);
void printArvoreBinaria(FILE*, unsigned int);

#endif