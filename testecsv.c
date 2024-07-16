#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int lerCsv(const char *arquivo){
    char S[50];
    char *ptr;
    int contagem = 0;

    FILE *arquivocsv;
    arquivocsv = fopen(arquivo, "r");
    
    int numeroLinhas = 0;
    int numeroColunas = 0;

//looping q percorre as linhas do arquivo ccs e armaquena em um ponteiro S
    while (EOF != fscanf(arquivocsv,"%[^\n]\n" , S)) {
        numeroLinhas++ ;
        numeroColunas = 0;

        //separa cada campo de cada linha de acordo com o separador ';'.
        ptr = strtok(S, ";");

    
        
        
        while (ptr != NULL) {

            numeroColunas++;


            //dentro desse looping podemos colocar condicionais que servem como validação
            //ele nao esta funcionando com nulo - motivo: quando ptr = NULL, n entra no looping
            //ou seja, precisamos pensar em outra maneira de iterar esse looping para a validação de nulos


            printf("'%s'", ptr);
            //ex:
            if(strcmp(ptr,"nao") == 0){
                printf("\nTEM UM NÃO EM -> linha: %d coluna: %d\n", numeroLinhas, numeroColunas);
                contagem++ ;


            }

            ptr = strtok(NULL, ";");

            
        }
        printf("\n");
    }

    printf("TOTAL DE NAO: %d", contagem);


    fclose(arquivocsv);
    return 0;

}

int main() {

    const char *nomeArquivo = "tempo.csv";
    lerCsv(nomeArquivo);

    
}