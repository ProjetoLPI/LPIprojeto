#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//TEMOS UM GRANDE PROBLEMA COM A INDICAÇÃO DOS ARQUIVOS CSV, ELE FICARAM BUGADOS - quebrando linha

void validador_de_CEP(const char *arquivo) {
    char S[10024];
    char *ptr;

    FILE *arquivocsv = fopen(arquivo, "r");
    if (arquivocsv == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    
    int NumeroLinhas = 0;

    while (fgets(S, sizeof(S), arquivocsv) != NULL) {
        int NumeroColunas = 0;
        ptr = strtok(S, ";");

        while (ptr != NULL) {
            if (NumeroColunas == 6) {
                size_t len = strlen(ptr);
                if (len < 8 || len > 10) {
                    printf("\nCEP inválido na linha %d, coluna %d: '%s'\n", NumeroLinhas, NumeroColunas, ptr);
                } else {
                    printf("\nCEP válido na linha %d, coluna %d: '%s'\n", NumeroLinhas, NumeroColunas, ptr);
                }
            }
            NumeroColunas++;
            ptr = strtok(NULL, ";");
        }
        NumeroLinhas++;
    }

    fclose(arquivocsv);
}
