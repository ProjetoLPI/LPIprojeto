#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <ctype.h>
#include <math.h>

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
        return 1;
    }
    return 0;
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
        int colunasNulas[100]; // Assumindo um máximo de 100 colunas
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


//VALIDAÇÃO DE CEP

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

    // Loop para ler as linhas
    while ((lendo = fgetc(arquivocsv)) != EOF) {
        int i = 0;
        indiceColunas--;

        // Adicionando caracteres ao buffer da linha
        while (lendo != '\n' && lendo != EOF) {
            linhasCsv[i++] = lendo;
            lendo = fgetc(arquivocsv);
        }
        linhasCsv[i] = '\0';  // Terminando a string com nulo

        // Extraindo colunas
        if (!dataReconhecida) {
            char primeiraColuna[100];
            sscanf(linhasCsv, "%99[^;]", primeiraColuna);

            if (VerificarDataHora2(primeiraColuna)) {
                dataReconhecida = 1;
            }
        }

        // Processando colunas se a data for reconhecida
        if (dataReconhecida) {
            char colunasCsv[1024];
            int j = 0;

            for (int k = 0; k <= i; k++) {
                // Se o delimitador ';' ou fim de linha '\0' for encontrado
                if (linhasCsv[k] == ';' || linhasCsv[k] == '\0') {
                    colunasCsv[j] = '\0';  // Terminando a string com nulo

                    // Comparando strings ignorando caracteres especiais
                    if (strstr(colunasCsv, "CEP da Resid") != NULL) {
                        indiceCEP = indiceColunas;
                        fclose(arquivocsv);
                        return indiceCEP;  // Retornando o índice imediatamente ao encontrar
                    }

                    j = 0;  // Reset para a próxima coluna
                    indiceColunas++;
                } else {
                    // Adicionando elementos ao buffer da coluna
                    colunasCsv[j++] = linhasCsv[k];
                }
            }
        }
    }

    fclose(arquivocsv);

    return indiceCEP;  // Retornar -1 se não encontrado
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
    int indiceLinhasReconhecidas = 2;

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

        // Ignorar linhas vazias
        if (i == 0)
        {
            continue;
        }

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
            else
            {
                printf("linha: %d tem CEP %s valido\n", indiceLinhasReconhecidas, cep);
            }
            
            indiceLinhasReconhecidas++;
        }

        indiceLinhas++;
    }

    printf("Quantidade de linhas reconhecidas: %d\n", indiceLinhasReconhecidas);

    fclose(arquivocsv);

    return 0;
}




//VALIDAÇÃO PRONTUARIO, obs: nem todos as tabelas tem prontuario


//utiliza VerificarDataHora2 e tem a função de encontrar o indice do CEP no arquivo
//problema encontrado: as primeiras linhas do cabeçalho estao quebradas, entao precisa fzr ele n parar de contar
int EncontrarProntuario(const char *arquivo) {

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
                    if (strstr(colunasCsv, "N�mero de prontu�rio no CISAM") != NULL) {
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


//vai dentro da outra
int validarProntuario(const char *prontuario) {
    if (prontuario == NULL || strlen(prontuario) == 0) {
        return 1;  // Prontuário vazio ou nulo é inválido
    }

    // Verificar se todos os caracteres no prontuário são dígitos
    for (int i = 0; prontuario[i] != '\0'; i++) {
        if (!isdigit(prontuario[i])) {
            return 0;  // Encontrou um caractere que não é dígito
        }
    }

    return 1;  // Prontuário válido
}


//função para usar no gtk
int ValidarProntuario(gchar *arquivo)
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

    int indiceCep = EncontrarProntuario(arquivo);
    if (indiceCep == -1)
    {
        printf("Cabeçalho de CEP não encontrado neste arquivo\n");
        fclose(arquivocsv);
        return -1;
    }

    rewind(arquivocsv); // Reinicia a leitura do arquivo desde o início

    char lendo;
    char linhasCsv[10024];
    int indiceLinhas = 0;
    int indiceLinhasReconhecidas = 1;

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

        // Ignorar linhas vazias
        if (i == 0)
        {
            continue;
        }

        // Processo de extração de cada coluna
        int indiceColunas = 0;
        int dataReconhecida = 0;
        int prontuarioValido = 1;

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
            char prontuario[20]; // Buffer para armazenar o número do prontuário

            for (int k = 0; k <= i; k++)
            {
                // Caso ele reconheça o delimitador ';' ou o final da linha '\0'
                if (linhasCsv[k] == ';' || linhasCsv[k] == '\0')
                {
                    colunasCsv[j] = '\0'; // Caracter final de cada string

                    if (indiceColunas == indiceCep) // Indice da coluna de prontuário
                    {
                        strcpy(prontuario, colunasCsv); // Copiar o número do prontuário para o buffer
                        if (!validarProntuario(colunasCsv))
                        {
                            prontuarioValido = 0;
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

            if (!prontuarioValido) // Imprime apenas se o número do prontuário for inválido
            {
                printf("linha: %d tem Numero de Prontuario %s inválido\n", indiceLinhasReconhecidas, prontuario);
            }

            indiceLinhasReconhecidas++;
        }

        indiceLinhas++;
    }

    printf("Quantidade de linhas reconhecidas: %d\n", indiceLinhasReconhecidas);

    fclose(arquivocsv);

    return 0;
}



//VALIDAÇÃO DE CNS:

int EncontrarCNS(const char *arquivo) {
    if (arquivo == NULL) {
        printf("Nenhum arquivo selecionado\n");
        return -1;
    }

    char lendo;
    char linhasCsv[10024];
    int indiceCNS = -1;
    int dataReconhecida = 0;
    int indiceColunas = 1;  // Inicializa fora do loop para manter a contagem

    FILE *arquivocsv = fopen(arquivo, "r");
    if (arquivocsv == NULL) {
        perror("Erro ao abrir o arquivo");
        return -1;
    }

    // Looping para ler linhas
    while ((lendo = fgetc(arquivocsv)) != EOF) {
        int i = 0;
        indiceColunas--;

        // Adicionando caracteres ao buffer da linha
        while (lendo != '\n' && lendo != EOF) {
            linhasCsv[i++] = lendo;
            lendo = fgetc(arquivocsv);
        }
        linhasCsv[i] = '\0';  // Terminando a string com NULL

        // Extraindo colunas
        if (!dataReconhecida) {
            char primeiraColuna[100];
            sscanf(linhasCsv, "%99[^;]", primeiraColuna);

            if (VerificarDataHora2(primeiraColuna)) {
                dataReconhecida = 1;
            }
        }

        // Processando colunas se a data for reconhecida
        if (dataReconhecida) {
            char colunasCsv[1024];
            int j = 0;

            for (int k = 0; k <= i; k++) {
                // Se encontrar delimitador ';' ou fim da linha '\0'
                if (linhasCsv[k] == ';' || linhasCsv[k] == '\0') {
                    colunasCsv[j] = '\0';  // Terminando a string com NULL

                    // Comparando strings ignorando caracteres especiais
                    if (strstr(colunasCsv, "N�mero da Carteira Nacional de Sa�de") != NULL) {
                        indiceCNS = indiceColunas;
                        fclose(arquivocsv);
                        return indiceCNS;  // Retornando o índice imediatamente ao encontrar
                    }

                    j = 0;  // Reset para a próxima coluna
                    indiceColunas++;
                } else {
                    // Adicionando elementos ao buffer da coluna
                    colunasCsv[j++] = linhasCsv[k];
                }
            }
        }
    }

    fclose(arquivocsv);

    return indiceCNS;  // Retornar -1 se não encontrado
}


int validarCNS(const char *cns) {
    char cns_str[16] = {0};  // Buffer para armazenar o CNS como string

    // Verificar se o CNS é nulo ou vazio
    if (cns == NULL || strlen(cns) == 0) {
        return 1;  // Ignorar nulos
    }

    // Verificar se o CNS está no formato científico
    if (strchr(cns, 'E') || strchr(cns, 'e')) {
        double cns_double = atof(cns);
        cns_double *= pow(10, 14);
        snprintf(cns_str, sizeof(cns_str), "%.0f", cns_double);
    } else {
        strncpy(cns_str, cns, 15);
        cns_str[15] = '\0';  // Garantir que está terminado com '\0'
    }

    // Verificar se o CNS convertido tem exatamente 15 caracteres
    if (strlen(cns_str) != 15) {
        return 0;  // CNS inválido se não tiver exatamente 15 dígitos
    }

    // Verificar se todos os caracteres no CNS são dígitos
    for (int i = 0; cns_str[i] != '\0'; i++) {
        if (!isdigit(cns_str[i])) {
            return 0;  // Encontrou um caractere que não é dígito
        }
    }

    return 1;  // CNS válido
}


int ValidarCNS(gchar *arquivo) {
    if (arquivo == NULL) {
        printf("Nenhum arquivo selecionado\n");
        return -1;
    }
    
    FILE *arquivocsv = fopen(arquivo, "r");
    if (arquivocsv == NULL) {
        perror("Erro ao abrir o arquivo");
        return -1;
    }

    int indiceCNS = EncontrarCNS(arquivo);
    if (indiceCNS == -1) {
        printf("Cabeçalho de CNS não encontrado neste arquivo\n");
        fclose(arquivocsv);
        return -1;
    }

    rewind(arquivocsv); // Reinicia a leitura do arquivo desde o início

    char lendo;
    char linhasCsv[10024];
    int indiceLinhas = 0;
    int indiceLinhasReconhecidas = 1;

    // Looping para percorrer as linhas
    while ((lendo = fgetc(arquivocsv)) != EOF) {
        int i = 0;

        // Adicionando os caracteres da linha ao vetor
        while (lendo != '\n' && lendo != EOF) {
            linhasCsv[i++] = lendo;
            lendo = fgetc(arquivocsv);
        }
        linhasCsv[i] = '\0'; // Caracter final de cada string

        // Ignorar linhas vazias
        if (i == 0) {
            continue;
        }

        // Processo de extração de cada coluna
        int indiceColunas = 0;
        int dataReconhecida = 0;
        int cnsValido = 1;

        // Verificar se a primeira coluna está no formato de data e hora
        char primeiraColuna[100];
        sscanf(linhasCsv, "%99[^;]", primeiraColuna);
        if (VerificarDataHora(primeiraColuna)) {
            dataReconhecida = 1;
        }

        // Se a data foi reconhecida, processar as colunas
        if (dataReconhecida) {
            char colunasCsv[1024];
            int j = 0;
            char cns[20]; // Buffer para armazenar o CNS

            for (int k = 0; k <= i; k++) {
                // Caso ele reconheça o delimitador ';' ou o final da linha '\0'
                if (linhasCsv[k] == ';' || linhasCsv[k] == '\0') {
                    colunasCsv[j] = '\0'; // Caracter final de cada string

                    if (indiceColunas == indiceCNS) { // Indice da coluna CNS
                        strcpy(cns, colunasCsv); // Copiar o CNS para o buffer
                        if (!validarCNS(colunasCsv)) {
                            cnsValido = 0;
                        }
                    }

                    j = 0; // Reseta para a próxima coluna
                    indiceColunas++;
                } else {
                    // Adiciona os elementos na coluna
                    colunasCsv[j++] = linhasCsv[k];
                }
            }

            if (!cnsValido) { // Imprime apenas se o CNS for inválido
                printf("linha: %d tem Numero da Carteira Nacional de Saude: %s invalido\n", indiceLinhasReconhecidas, cns);
            }

            indiceLinhasReconhecidas++;
        }

        indiceLinhas++;
    }

    printf("Quantidade de linhas reconhecidas: %d\n", indiceLinhasReconhecidas);

    fclose(arquivocsv);

    return 0;
}



//VALIDAÇÃO IDADE:

// Função para encontrar a coluna de "Idade" no arquivo CSV
int EncontrarIdade(const char *arquivo) {
    if (arquivo == NULL) {
        printf("Nenhum arquivo selecionado\n");
        return -1;
    }

    char lendo;
    char linhasCsv[10024];
    int indiceIdade = -1;
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
                    if (strcmp(colunasCsv, "IDADE") == 0) {
                        indiceIdade = indiceColunas;
                        fclose(arquivocsv);
                        return indiceIdade;  // Retornando o índice imediatamente ao encontrar
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

    return indiceIdade;  // Return -1 if not found
}

// Função para validar a idade
int validarIdade(const char *idadeStr) {
    if (idadeStr == NULL || strlen(idadeStr) == 0) {
        return 1;  // Idade vazia ou nula é inválida
    }

    // Verificar se todos os caracteres na idade são dígitos
    for (int i = 0; idadeStr[i] != '\0'; i++) {
        if (!isdigit(idadeStr[i])) {
            return 0;  // Encontrou um caractere que não é dígito
        }
    }

    int idade = atoi(idadeStr);
    if (idade < 0 || idade > 120) {
        return 0;  // Idade fora do intervalo permitido
    }

    return 1;  // Idade válida
}

// Função para usar no GTK
int ValidarIdade(gchar *arquivo) {
    if (arquivo == NULL) {
        printf("Nenhum arquivo selecionado\n");
        return -1;
    }

    FILE *arquivocsv = fopen(arquivo, "r");
    if (arquivocsv == NULL) {
        perror("Erro ao abrir o arquivo");
        return -1;
    }

    int indiceIdade = EncontrarIdade(arquivo);
    if (indiceIdade == -1) {
        printf("Cabeçalho de Idade não encontrado neste arquivo\n");
        fclose(arquivocsv);
        return -1;
    }

    rewind(arquivocsv); // Reinicia a leitura do arquivo desde o início

    char lendo;
    char linhasCsv[10024];
    int indiceLinhas = 0;
    int indiceLinhasReconhecidas = 1;

    // Looping para percorrer as linhas
    while ((lendo = fgetc(arquivocsv)) != EOF) {
        int i = 0;

        // Adicionando os caracteres da linha ao vetor
        while (lendo != '\n' && lendo != EOF) {
            linhasCsv[i++] = lendo;
            lendo = fgetc(arquivocsv);
        }
        linhasCsv[i] = '\0'; // Caracter final de cada string

        // Ignorar linhas vazias
        if (i == 0) {
            continue;
        }

        // Processo de extração de cada coluna
        int indiceColunas = 0;
        int dataReconhecida = 0;
        int idadeValida = 1;

        // Verificar se a primeira coluna está no formato de data e hora
        char primeiraColuna[100];
        sscanf(linhasCsv, "%99[^;]", primeiraColuna);
        if (VerificarDataHora(primeiraColuna)) {
            dataReconhecida = 1;
        }

        // Se a data foi reconhecida, processar as colunas
        if (dataReconhecida) {
            char colunasCsv[1024];
            int j = 0;
            char idade[10]; // Buffer para armazenar a Idade

            for (int k = 0; k <= i; k++) {
                // Caso ele reconheça o delimitador ';' ou o final da linha '\0'
                if (linhasCsv[k] == ';' || linhasCsv[k] == '\0') {
                    colunasCsv[j] = '\0'; // Caracter final de cada string

                    if (indiceColunas == indiceIdade) // Índice da coluna idade
                    {
                        strcpy(idade, colunasCsv); // Copiar a idade para o buffer
                        if (!validarIdade(colunasCsv)) {
                            idadeValida = 0;
                        }
                    }

                    j = 0; // Reseta para a próxima coluna
                    indiceColunas++;
                } else {
                    // Adiciona os elementos na coluna
                    colunasCsv[j++] = linhasCsv[k];
                }
            }

            if (!idadeValida) // Imprime apenas se a idade for inválida
            {
                printf("linha: %d tem Idade %s invalida\n", indiceLinhasReconhecidas, idade);
            }

            indiceLinhasReconhecidas++;
        }

        indiceLinhas++;
    }

    printf("Quantidade de linhas reconhecidas: %d\n", indiceLinhasReconhecidas);

    fclose(arquivocsv);

    return 0;
}



