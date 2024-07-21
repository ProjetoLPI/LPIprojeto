#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "funcoes_leitura.h"


int main()
{
    char linha[1024];
    char celula[1024];

    FILE *pF;
    pF = fopen("arquivo_teste.csv", "r");
    if (pF == NULL){
        printf("Erro ao abrir arquivo");
    }
    
    while (fgets(linha, sizeof(linha), pF) != NULL)
    {
        splitString(linha, ';');
    }
    
    
    fclose(pF);

    return 0;
}