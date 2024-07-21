#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes_validacao.c" // Header que contem as funções de validação, deixei em aspas pra diferenciar dos include da lib de C

//EXECUTE O MAIN PARA TESTAR AS FUNÇÕES DE VALIDAÇÃO
//O MAIN PUXA AS FUNÇÕES

int main() {


    int i = 0;

    while(i == 0){
        int n;
        printf(" 1 - VALIDAR CEP\n 2 - test ");
        scanf("%d", &n);

        if(n == 1){

            const char *nomeArquivo = "RespostaFormulario1TESTE.csv";
            ValidarNulo(nomeArquivo);

        }
        else if(n == 2){
            //adcionar outra validações
        }
        else{
            i = 1;
        }

    }


    
}