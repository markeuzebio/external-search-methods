#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "utilitarios.h"

Registro* alocarRegistros(int n)
{
    Registro* registro = (Registro*) malloc(n * sizeof(Registro));

    return registro;
}

void desalocarRegistros(Registro **registros)
{
    free(*registros);
}

bool lerEntrada(Entrada* entrada, int argc, char** argumentos)
{
    // Verifica se o numero de argumentos esta dentro do esperado
    if(argc < 5 || argc > 6)
        return false;

    entrada->metodo = atoi(argumentos[1]);
    entrada->quantidade_registros = atoi(argumentos[2]);
    entrada->situacao = atoi(argumentos[3]);
    entrada->chave_buscada = atoi(argumentos[4]);
    
    if(argc == 6)
    {
        strcpy(entrada->opcional, argumentos[5]);

        // Converte para LOWERCASE de modo a facilitar a comparacao futura
        for(int i = 0 ; entrada->opcional[i] != '\0' ; i++)
            entrada->opcional[i] = tolower(entrada->opcional[i]);

        // Se o argumento opcional for diferente de -P, entao retorna falso
        if(strcmp(entrada->opcional, "-p"))
            return false;
    }
    else
        entrada->opcional[0] = '\0';

    if(entrada->metodo < 1 || entrada->metodo > 4 || entrada->situacao < 1 || entrada->situacao > 3)
        return false;

    return true;
}