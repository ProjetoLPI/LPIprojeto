#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função auxiliar para verificar se uma string está no formato de data e hora
int VerificarDataHora(const char *str)
{
    int mes, dia, ano, hora, minuto, segundo;
    if (sscanf(str, "%d/%d/%d %d:%d:%d", &mes, &dia, &ano, &hora, &minuto, &segundo) == 6)
    {
        // Verificar limites razoáveis para data e hora
        if (mes >= 1 && mes <= 12 && dia >= 1 && dia <= 31 && ano >= 1000 && ano <= 9999 &&
            hora >= 0 && hora <= 23 && minuto >= 0 && minuto <= 59 && segundo >= 0 && segundo <= 59)
        {
            return 1;
        }
    }
    return 0;
}

// PODEMOS USAR COMO BASE PARA QUALQUER TIPO DE VALIDAÇÃO
int ValidarNulo(const char *arquivo)
{
    char lendo;
    char linhasCsv[10024];
    int indiceLinhas = 0;
    int indiceLinhasReconhecidas = 0;

    FILE *arquivocsv = fopen(arquivo, "r");
    if (arquivocsv == NULL)
    {
        perror("Erro ao abrir o arquivo");
        return -1;
    }

    // Looping para percorrer as linhas
    while ((lendo = fgetc(arquivocsv)) != EOF)
    {
        int i = 0;

        // Adicionando os caracteres da linha ao vetor
        while (lendo != '\n' && lendo != EOF)
        {
            linhasCsv[i++] = lendo;
            lendo = fgetc(arquivocsv);
        }
        linhasCsv[i] = '\0'; // Caracter final de cada string

        // Processo de extração de cada coluna
        int indiceColunas = 0;
        int dataReconhecida = 0;
        int colunasNulas[50]; // Assumindo um máximo de 50 colunas
        int nc = 0;

        // Verificar se a primeira coluna está no formato de data e hora
        char primeiraColuna[100];
        sscanf(linhasCsv, "%99[^;]", primeiraColuna);
        if (VerificarDataHora(primeiraColuna))
        {
            dataReconhecida = 1;
        }

        // Se a data foi reconhecida, processar as colunas
        if (dataReconhecida)
        {
            char colunasCsv[1024];
            int j = 0;

            for (int k = 0; k <= i; k++)
            {
                // Caso ele reconheça o delimitador ';' ou o final da linha '\0'
                if (linhasCsv[k] == ';' || linhasCsv[k] == '\0')
                {
                    colunasCsv[j] = '\0'; // Caracter final de cada string
                    // if(j == 1){
                    //     printf(colunasCsv) ;
                    // }

                    if (colunasCsv[0] == '\0')
                    {
                        colunasNulas[nc++] = indiceColunas;
                    }

                    j = 0; // Reseta para a próxima coluna
                    indiceColunas++;
                }
                else
                {
                    // Adiciona os elementos na coluna
                    colunasCsv[j++] = linhasCsv[k];
                }
                    
            }

            // Print the line number and indices of empty columns
            printf("linha: %d colunas: ", indiceLinhasReconhecidas);
            for (int ab = 0; ab < nc; ab++)
            {
                printf("%d ", colunasNulas[ab]);
            }
            printf("vazias\n");

            indiceLinhasReconhecidas++;
        }

        indiceLinhas++;
    }

    printf("Quantidade de linhas reconhecidas: %d\n", indiceLinhasReconhecidas);

    fclose(arquivocsv);

    return 0;
}

