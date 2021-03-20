/**
 * Aluno: David Fanchic Chatelard
 * Disciplina: Estrutura de Dados
 * Implementação de funções para se verificar se uma expressão matemática foi digitada corretamente
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX 101

typedef char info_t;

typedef struct{
    int len;
    info_t dados[MAX];
} pilha_t;

pilha_t* cria_pilha(){
    pilha_t* pi = malloc(sizeof(pilha_t));

    if(!pi)
        return NULL;
    pi->len = 0;
    return pi;
}

int vazia(pilha_t* pilha){
    if(!pilha)
        return -1;
    return (pilha->len == 0);
}

int cheia(pilha_t* pilha){
    if(!pilha)
        return -1;
    return (pilha->len == MAX);
}

void libera_pilha(pilha_t* pilha){
    free(pilha);
}

int empilha(info_t e, pilha_t* pilha) {
    if(!pilha)
        return 0;
    if (cheia(pilha))
        return 0;

    pilha->dados[pilha->len] = e;
    pilha->len++;

    return 1;
}

char desempilha(pilha_t* pilha){
    char aux;
    if(vazia(pilha) || !pilha)
        return '\0';

    aux = pilha->dados[pilha->len-1];
    pilha->len--;

    return aux;
}

int tamanho(pilha_t* pilha){
    if(!pilha)
        return -1;
    return pilha->len;
}

int operador(char caracter){
    return (caracter == '+' || caracter == '-' || caracter == '*' || caracter == '/' || caracter == '^');
}

_Bool valida_limitadores(const char* expressao){
    pilha_t* pilha;
    char aux;
    int parenteses = 0, colchetes = 0, chaves = 0;
    long int i, len;

    len = strlen(expressao);
    pilha = cria_pilha();

    for(i = 0;i < len;i++){
        switch(expressao[i]){
            case '(':
                parenteses++;
                aux = expressao[i];
                empilha(aux, pilha);
                break;
            case '[':
                colchetes++;
                aux = expressao[i];
                empilha(aux, pilha);
                break;
            case '{':
                chaves++;
                aux = expressao[i];
                empilha(aux, pilha);
                break;
        }
        switch(expressao[i]){
            case ')':
                parenteses--;
                aux = desempilha(pilha);
                if(!aux || aux != '(') {
                    libera_pilha(pilha);
                    return false;
                }
                break;
            case ']':
                colchetes--;
                aux = desempilha(pilha);
                if(!aux || aux != '[') {
                    libera_pilha(pilha);
                    return false;
                }
                break;
            case '}':
                chaves--;
                aux = desempilha(pilha);
                if(!aux || aux != '{') {
                    libera_pilha(pilha);
                    return false;
                }
                break;
        }
    }

    if(!vazia(pilha) || parenteses != 0 || colchetes != 0 || chaves != 0) {
        libera_pilha(pilha);
        return false;
    }
    libera_pilha(pilha);
    return true;
}

_Bool valida_operadores(const char* expressao){
    pilha_t* pilha2;
    char aux;
    long int i, len;

    len = strlen(expressao);
    pilha2 = cria_pilha();

    //Percorre a expressão
    for(i = 0;i < len;i++){
        //Empilha os números
        if(expressao[i] >= '0' && expressao[i] <= '9'){
            //verifica se é um número de mais de um digito ou ponto flutuante
            if((expressao[i+1] >= '0' && expressao[i+1] <= '9') || expressao[i+1] == '.')
                continue;
            aux = expressao[i];
            empilha(aux, pilha2);
        }

        //Empilha os operadores
        if(expressao[i] == '+' || expressao[i] == '-' || expressao[i] == '*' || expressao[i] == '/' || expressao[i] == '^'){
            aux = expressao[i];
            empilha(aux, pilha2);
        }
    }


    aux = desempilha(pilha2);
    //Caso aux não seja nulo e a expressão termine com operador
    if((aux) && (aux == '+' || aux == '-' || aux == '*' || aux == '/' || aux == '^')) {
        return false;
    }
    //Caso só tenha 1 elemento
    if(vazia(pilha2))
        return false;
    while(!vazia(pilha2)){
        //Caso a expressão comece com operador
        if((aux == '+' || aux == '-' || aux == '*' || aux == '/' || aux == '^') && (pilha2->len == 0)) {
            libera_pilha(pilha2);
            return false;
        }
        //Caso tenham dois operandos seguidos
        if((aux >= '0' && aux <= '9') && (pilha2->dados[pilha2->len-1] >= '0' && pilha2->dados[pilha2->len-1] <= '9')) {
            libera_pilha(pilha2);
            return false;
        }
        //Caso tenha um operador seguido de outro
        if((aux == '+' || aux == '-' || aux == '*' || aux == '/' || aux == '^') && (pilha2->dados[pilha2->len-1] == '+' || pilha2->dados[pilha2->len-1] == '-' || pilha2->dados[pilha2->len-1] == '*' || pilha2->dados[pilha2->len-1] == '/' || pilha2->dados[pilha2->len-1] == '^')) {
            libera_pilha(pilha2);
            return false;
        }
        aux = desempilha(pilha2);
    }

    //Caso a expressão comece com operador
    if(aux == '+' || aux == '-' || aux == '*' || aux == '/' || aux == '^'){
        libera_pilha(pilha2);
        return false;
    }
    libera_pilha(pilha2);
    return true;
}


_Bool valida(const char* expressao) {
    int i, j, len;
    char aux[101];
    len = strlen(expressao);

    if(!expressao)
        return false;
    if(expressao[0] == '\0')
        return true;

    j = 0;
    for (i = 0; i < len; i++) {
        if(expressao[i] == ' ')
            continue;
        aux[j] = expressao[i];
        j++;
    }
    aux[j] = '\0';
    for (i = 0; i < len; i++) {
        if(operador(aux[i])){
            if(aux[i-1] == '(' && aux[i+1] == ')')
                return false;
            if(aux[i+1] == ')')
                return false;
            if(aux[i-1] == '(')
                return false;
        }
    }

    if(valida_limitadores(expressao) && valida_operadores(expressao))
        return true;
    return false;
}

int main() {
    char expressao[101];

    expressao[0] = '\0';

    scanf("%100[^\n]", expressao);

    if (valida(expressao))
        printf("VALIDA\n");
    else
        printf("INVALIDA\n");

    return 0;
}