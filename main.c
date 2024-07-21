#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes_validacao.c" // Header que contem as funções de validação, deixei em aspas pra diferenciar dos include da lib de C



int main() {


    int i = 0;

    while(i == 0){
        int n;
        printf(" 1 - VALIDAR CEP\n 2 - test ");
        scanf("%d", &n);

        if(n == 1){

            //esse arquivo foi cortado para facilitar nos testes
            const char *nomeArquivo = "RespostaFormulario1.csv";
            validador_de_CEP(nomeArquivo);

        }
        else if(n == 2){
            const char *nomeArquivo = "tempo.csv";
            testCsv(nomeArquivo);
        }
        else{
            i = 1;
        }

    }


    
}