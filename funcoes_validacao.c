#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <stdio.h>

#include <stdio.h>


//PODEMOS USAR COMO BASE PARA QUALQUER TIPO DE VALIDAÇÃO
int ValidarNulo(const char *arquivo)
{
    char lendo;
    char linhasCsv[10024];
    int indiceLinhas = 0;

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

        // Reconhecendo primeira coluna COMO : dd/mm/aaaa e 00:00:00 - os arquivos sao bugados, entao isso é necessario
        for (int n = 0; n < i; n++)
        {
            if (linhasCsv[n] == '/' || linhasCsv[n] == ':')
            {
                dataReconhecida++;
            }
            else if (dataReconhecida == 4)
            {
                break;
            }
        }

        // Se a data foi reconhecida, processar as colunas
        if (dataReconhecida == 4)
        {
            for (int k = 0; k <= i; k++)
            {
                static char colunasCsv[1024];
                static int j = 0;

                // Caso ele reconheça o delimitador ';' ou o final da linha '\0'
                if (linhasCsv[k] == ';' || linhasCsv[k] == '\0')
                {
                    colunasCsv[j] = '\0'; // Caracter final de cada string

                    if (colunasCsv[0] == '\0')
                    {
                        printf("linha %d coluna %d vazia\n", indiceLinhas, indiceColunas);
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
        }

        indiceLinhas++;
    }

    printf("Quantidade de linhas no arquivo: %d\n", indiceLinhas);

    fclose(arquivocsv);

    return 0;
}



