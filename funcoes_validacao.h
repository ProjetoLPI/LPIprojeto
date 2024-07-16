//  Criei o arquivo pra organizarmos as funções de validação, fica mais organizado se quisermos fazer correções, etc
//  Já fiz o #include desse arquivo no main
//  Função para validar o campo de CEP
//  !PRECISAMOS INTEGRAR ELA NO MAIN E FAZER TESTES!

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void validador_de_CEP(char *CEP)
{
    // o CEP pode ter NO MAXIMO 10 digitos, considerando que a pessoa escreva 12.345-678 ou no MINIMO 8, se a pessoa escrever 12345678
    // coloquei 11 pois sendo 10 o valor maximo ainda temos que considerar 1 para o '\0' por padrão de C

    if (strlen(CEP) < 8 || strlen(CEP) > 11) //strlen informa a quantidade de caracteres da string
    {
        printf("O CEP digitado e invalido!");
    }
    
}


