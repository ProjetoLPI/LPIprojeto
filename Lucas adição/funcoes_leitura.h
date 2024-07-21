#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void splitString(char* str, char delimiter) {
    char substr[100];  // Buffer para armazenar substrings temporariamente
    int j = 0;  // Índice para substrings

    for (int i = 0; i <= strlen(str); i++) {
        
        if (str[i] == delimiter || str[i] == '\0') // Se encontrar o delimitador ou o final da string
        {
            substr[j] = '\0';  // Termina a substring atual
            printf("%s\n", substr);  // Imprime a substring
            j = 0;  // Reseta o índice para a próxima substring
        } else {
            substr[j] = str[i];  // Adiciona caractere à substring
            j++;
        }
    }
}
