#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <ctype.h>

// dd/mm/aaaa 00:00:00 - verifica os primeiros objetos das colunas
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

//Carimbo de data/hora - verifica o primeiro objeto da primeira coluna

int VerificarDataHora2(const char *str)
{
    if (strcmp(str, "Carimbo de data/hora") == 0)
    {
        printf("carimbo data encontrado");
        return 1;
    }
    return 0;
}


//essa função vai dentro de ValidarCEP
int validarCEP(const char *cep) {
    if (cep == NULL || strlen(cep) == 0) {
        return 1;  // Tratar CEP vazio como válido
    }

    int comprimento = 0;
    char cep_limpo[9];  // 8 dígitos + 1 terminador nulo
    int j = 0;

    // Iterar sobre cada caractere na string de entrada
    for (int i = 0; cep[i] != '\0'; i++) {
        if (isdigit(cep[i])) {  // Se o caractere for um dígito
            cep_limpo[j++] = cep[i];
        }
    }
    cep_limpo[j] = '\0';  // Terminar a string limpa com nulo

    // Verificar se o CEP limpo tem exatamente 8 dígitos
    comprimento = strlen(cep_limpo);
    if (comprimento != 8) {
        return 0;  // CEP inválido devido ao comprimento
    }

    // Verificar se todos os caracteres em cep_limpo são dígitos
    for (int i = 0; i < comprimento; i++) {
        if (!isdigit(cep_limpo[i])) {
            return 0;  // Encontrou um caractere que não é dígito
        }
    }

    return 1;  // CEP válido
}


// PODEMOS USAR COMO BASE PARA QUALQUER TIPO DE VALIDAÇÃO
int ValidarNulo(gchar *arquivo)
{

    if (arquivo == NULL)
    {
        printf("Nenhum arquivo selecionado\n");
        return -1;
    }
    
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
        int colunasNulas[100]; // Assumindo um máximo de 50 colunas
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

//utiliza VerificarDataHora2 e tem a função de encontrar o indice do CEP no arquivo
//problema encontrado: as primeiras linhas do cabeçalho estao quebradas, entao precisa fzr ele n parar de contar
int EncontrarCEP(const char *arquivo) {
    if (arquivo == NULL) {
        printf("Nenhum arquivo selecionado\n");
        return -1;
    }

    char lendo;
    char linhasCsv[10024];
    int indiceCEP = -1;
    int dataReconhecida = 0;
    int indiceColunas = 1;  // Inicializa fora do loop para manter a contagem

    FILE *arquivocsv = fopen(arquivo, "r");
    if (arquivocsv == NULL) {
        perror("Erro ao abrir o arquivo");
        return -1;
    }

    // Looping to read lines
    while ((lendo = fgetc(arquivocsv)) != EOF) {
        int i = 0;
        indiceColunas--;

        // Adding characters to the line buffer
        while (lendo != '\n' && lendo != EOF) {
            linhasCsv[i++] = lendo;
            lendo = fgetc(arquivocsv);
        }
        linhasCsv[i] = '\0';  // Null-terminating the string

        // Extracting columns
        if (!dataReconhecida) {
            char primeiraColuna[100];
            sscanf(linhasCsv, "%99[^;]", primeiraColuna);

            if (VerificarDataHora2(primeiraColuna)) {
                dataReconhecida = 1;
            }
        }

        // Processing columns if the date is recognized
        if (dataReconhecida) {
            char colunasCsv[1024];
            int j = 0;

            for (int k = 0; k <= i; k++) {
                // If delimiter ';' or end of line '\0' is found
                if (linhasCsv[k] == ';' || linhasCsv[k] == '\0') {
                    colunasCsv[j] = '\0';  // Null-terminating the string

                    // Comparing strings ignoring special characters
                    if (strstr(colunasCsv, "CEP da Resid") != NULL) {
                        indiceCEP = indiceColunas;
                        fclose(arquivocsv);
                        return indiceCEP;  // Retornando o índice imediatamente ao encontrar
                    }

                    j = 0;  // Reset for the next column
                    indiceColunas++;
                } else {
                    // Adding elements to the column buffer
                    colunasCsv[j++] = linhasCsv[k];
                }
            }
        }
    }

    fclose(arquivocsv);

    return indiceCEP;  // Return -1 if not found
}




//essa função utiliza EncontrarCEP, validarCEP e VerificarDataHora
int ValidarCEP(gchar *arquivo)
{
    if (arquivo == NULL)
    {
        printf("Nenhum arquivo selecionado\n");
        return -1;
    }
    
    FILE *arquivocsv = fopen(arquivo, "r");
    if (arquivocsv == NULL)
    {
        perror("Erro ao abrir o arquivo");
        return -1;
    }

    int indiceCep = EncontrarCEP(arquivo);
    if (indiceCep == -1)
    {
        printf("Cabeçalho de CEP não encontrado\n");
        fclose(arquivocsv);
        return -1;
    }

    rewind(arquivocsv); // Reinicia a leitura do arquivo desde o início

    char lendo;
    char linhasCsv[10024];
    int indiceLinhas = 0;
    int indiceLinhasReconhecidas = 0;

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
        int cepValido = 1;

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
            char cep[20]; // Buffer para armazenar o CEP

            for (int k = 0; k <= i; k++)
            {
                // Caso ele reconheça o delimitador ';' ou o final da linha '\0'
                if (linhasCsv[k] == ';' || linhasCsv[k] == '\0')
                {
                    colunasCsv[j] = '\0'; // Caracter final de cada string

                    if (indiceColunas == indiceCep) //indice da coluna cep
                    {
                        strcpy(cep, colunasCsv); // Copiar o CEP para o buffer
                        if (!validarCEP(colunasCsv))
                        {
                            cepValido = 0;
                        }
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

            if (!cepValido) // Imprime apenas se o CEP for inválido
            {
                printf("linha: %d tem CEP %s inválido\n", indiceLinhasReconhecidas, cep);
            }

            indiceLinhasReconhecidas++;
        }

        indiceLinhas++;
    }

    printf("Quantidade de linhas reconhecidas: %d\n", indiceLinhasReconhecidas);

    fclose(arquivocsv);

    return 0;
}
