#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void validador_de_CEP(const char *arquivo) {
    char S[10024];
    char *ptr;
    int dataReconhecida = 0;
    int inidiceerrosCEP[1000][2];

    int i = 0;
    char *errosCEP[100];

    FILE *arquivocsv = fopen(arquivo, "r");
    if (arquivocsv == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    int NumeroLinhas = 0;

    while (fgets(S, sizeof(S), arquivocsv) != NULL) {
        int NumeroColunas = 0;
        dataReconhecida = 0;
        ptr = strtok(S, ";");

        while (ptr != NULL) {
           if (!dataReconhecida) {
                // Verificar se o token é uma data no formato D/M/YYYY HH:MM:SS
                int barraCount = 0;
                int doisPontosCount = 0;

                for (char *c = ptr; *c != '\0'; c++) {
                    if (*c == '/') barraCount++;
                    if (*c == ':') doisPontosCount++;
                }

                if (barraCount == 2 && doisPontosCount == 2) {
                    dataReconhecida = 1; // A data foi reconhecida
                }
            } else {
                // Processar as colunas após reconhecer a data
                if (NumeroColunas == 5) {
                    size_t len = strlen(ptr);
                    if (len < 8 || len > 10) {
                        // Guardar os valores inválidos em uma matriz
                        inidiceerrosCEP[i][0] = NumeroLinhas;
                        inidiceerrosCEP[i][1] = NumeroColunas;
                        errosCEP[i] = strdup(ptr); // Armazenar o valor inválido
                        i++;
                    }
                }
                NumeroColunas++;
            }
            ptr = strtok(NULL, ";");
        }
        NumeroLinhas++;
    }

    fclose(arquivocsv);

    for (int n = 0; n < i; n++) {
        printf("\nCEP invalido na linha %d, coluna %d: '%s'\n", inidiceerrosCEP[n][0], inidiceerrosCEP[n][1] + 2, errosCEP[n]);
        free(errosCEP[n]); // Liberar a memória alocada
    }
}

