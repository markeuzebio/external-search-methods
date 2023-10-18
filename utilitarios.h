#ifndef __UTILITARIOS__
#define __UTILITARIOS__

#define MAXTABELA 100

typedef struct
{
    int chave;
    long dado1;
    char dado2[5001];

} Registros;

typedef struct
{
    int metodo, situacao;
    long int quantidade, chave;

} Entrada;

#endif