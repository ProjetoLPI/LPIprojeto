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

int testCsv(const char *arquivo) {
    char lendo;
    char linhasCsv[1024];
    int indiceLinhas = 0;

    FILE *arquivocsv = fopen(arquivo, "r");
    if (arquivocsv == NULL) {
        perror("Erro ao abrir o arquivo");
        return -1; // Return an error code
    }

    //looping pra percorrer as linhas
    while ((lendo = fgetc(arquivocsv)) != EOF) {
        int i = 0;

        //adcionando as linhas a um vetor - "caracter por caracter"
        while (lendo != '\n' && lendo != EOF) {
            linhasCsv[i++] = lendo;
            lendo = fgetc(arquivocsv);
        }
        
        linhasCsv[i] = '\0'; //caracter final de cada string

        // processo de extração de cada coluna
        int indiceColunas = 0;
        char colunasCsv[1024];
        int j = 0;

        //percorrendo cada caracter da linha criada
        for (int k = 0; k <= i; k++) {
            //caso ele reconheça o delimitador ';' ou o final da linha '\0'
            if (linhasCsv[k] == ';' || linhasCsv[k] == '\0') {
                colunasCsv[j] = '\0'; // caracter final de cada string

                    //so pra observar q ele esta contando bem as colunas, e reconhece os vazios
                    if (indiceColunas == 3)
                            {
                                printf("%s| - linha %d, coluna %d", colunasCsv, indiceLinhas, indiceColunas+1);
                            }
                j = 0; // reseta para proxima coluna
                indiceColunas++;
            } else {
                //adciona os elementos na colouna
                colunasCsv[j++] = linhasCsv[k];
            }
        }

        
        
        
        printf("\n");
        indiceLinhas++;
    }

    printf("Quantidade de linhas no arquivo: %d\n", indiceLinhas);

    fclose(arquivocsv);
   
}


