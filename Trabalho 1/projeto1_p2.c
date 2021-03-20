/**
 * Aluno: David Fanchic Chatelard
 * Disciplina: Estrutura de Dados
 * Implementação de funções para se transformar uma expressão matemática no formato infixa para posfixa(polonesa inversa)
*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define boolean int
#define TamPilha 100

typedef char elem;

typedef struct {
    int topo;
    elem A[TamPilha];
} Pilha;

/* cria uma pilha vazia P */
void Create(Pilha *P) {
    P->topo=-1;
    return;
}

/* esvazia logicamente a pilha P */
void Empty(Pilha *P) {
    P->topo=-1;
    return;
}

/* retorna true se a pilha estiver vazia; false caso contrário */
boolean IsEmpty(Pilha *P) {
    return (P->topo==-1);
}

/* verifica se P cresceu até o final do array */
boolean IsFull(Pilha *P) {
    return (P->topo==TamPilha-1);
}

/* empilha o valor da variável X na pilha P */
boolean Push(Pilha *P, elem *X) {
    if (!IsFull(P)) {
        P->topo++;
        P->A[P->topo]=*X;
        return TRUE;
    }
    return FALSE;
}

/*desempilha P e retorna em X o valor do elemento que estava no topo de P;
retorna TRUE se sucesso; FALSE, c.c. */
boolean Pop(Pilha *P, elem *X) {
    if (!IsEmpty(P)) {
        *X=P->A[P->topo];
        P->topo--;
        return TRUE;
    }
    return FALSE;
}

/* retorna o valor do elemento do topo de P, sem desempilhar – chamada apenas
se Pilha P não estiver vazia!!! */
elem Top(Pilha *P) {
    return P->A[P->topo];
}

int prioridade(char operador){
    switch (operador){
        case '+':
            return 1;
        case '-':
            return 1;
        case '*':
            return 2;
        case '/':
            return 2;
        case '^':
            return 3;
        default:
            return -1;
    }
}

int operador(char letra){
    return (letra == '+' || letra == '-' || letra == '*' || letra == '/' || letra == '^');
}

int limitador_abrindo(char letra){
    return (letra == '(' || letra == '[' || letra == '{');
}

int limitador_fechando(char letra){
    return (letra == ')' || letra == ']' || letra == '}');
}

int numero(char letra){
    return (letra >= '0' && letra <= '9');
}

void transforma(const char* infixa, char* posfixa) {
    //Pilha de operadores e parenteses
    Pilha pilha;
    int i, j = 0, len;
    char aux, pop;

    Create(&pilha);
    len = strlen(infixa);

    for (i = 0; i < len; i++) {
        //Para evitar um aviso da IDE
        aux = infixa[i];

        //Se for um espaço pula a iteração
        if(aux == ' ')
            continue;

        //Se for um número bota na expressão pós-fixa e vai para a próxima iteração
        if(numero(aux)){

            //Para botar números float na expressão pós-fixa
            while (i < len && (infixa[i] != ' ' && !operador(infixa[i]) && !limitador_abrindo(infixa[i]) && !limitador_fechando(infixa[i]))) {
                posfixa[j] = infixa[i];
                j++;
                i++;
            }
            i--;

            //Tem que ter um espaço entre cada caracter
            posfixa[j] = ' ';
            j++;
            continue;
        }

        //Se for um '(', empilha e vai para a próxima iteração
        if(limitador_abrindo(aux)) {
            Push(&pilha, &aux);
            continue;
        }

        //Se for um operador
        if(operador(aux)){

            //Se a pilha estiver vazia, empilha e vai para a próxima iteração
            if(IsEmpty(&pilha)) {
                Push(&pilha, &aux);
                continue;
            }

            //Se o topo for um '(' OU se a prioridade for maior do que a prioridade do topo da pilha, empilha e vai para a próxima iteração
            if(limitador_abrindo(Top(&pilha)) || prioridade(aux) > prioridade(Top(&pilha))) {
                Push(&pilha, &aux);
                continue;
            }

            //Se a pilha não for vazia E o topo não for '(' E se a prioridade for menor ou igual do que a prioridade do topo da pilha
            if (!IsEmpty(&pilha) && !limitador_abrindo(Top(&pilha)) && prioridade(aux) <= prioridade(Top(&pilha))) {

                //Enquanto a pilha não for vazia E o topo não for '(' E se a prioridade for menor ou igual do que a prioridade do topo da pilha
                while(!IsEmpty(&pilha) && !limitador_abrindo(Top(&pilha)) && prioridade(aux) <= prioridade(Top(&pilha))){

                    //Desempilha e bota esse caracter na expressão pós-fixa e empilha o novo caracter
                    Pop(&pilha, &pop);
                    posfixa[j] = pop;
                    j++;

                    //Tem que ter um espaço entre cada caracter
                    posfixa[j] = ' ';
                    j++;
                }
                Push(&pilha, &aux);
            }
        }

        //Se a pilha não for vazia E o topo não for '(' E se o topo ainda tiver prioridade menor ou igual do que a do anterior
        if(!IsEmpty(&pilha) && !limitador_abrindo(Top(&pilha)) && prioridade(Top(&pilha)) <= prioridade(pilha.A[pilha.topo-1])){

            //Enquanto a pilha não for vazia E o topo ainda tiver prioridade menor ou igual do que o anterior
            while(!IsEmpty(&pilha) && prioridade(Top(&pilha)) <= prioridade(pilha.A[pilha.topo-1])){
                //Desempilha e bota o caracter na expressão pós-fixa
                Pop(&pilha, &pop);
                posfixa[j] = pop;
                j++;

                //Tem que ter um espaço entre cada caracter
                posfixa[j] = ' ';
                j++;
            }
        }

        //Se for um ')'
        if(limitador_fechando(aux)){

            //Enquanto a pilha não for vazia E o topo da pilha for diferente de '('
            while(!IsEmpty(&pilha) && !limitador_abrindo(Top(&pilha))){

                //Desempilha e bota o caracter na expressão pós-fixa
                Pop(&pilha, &pop);
                posfixa[j] = pop;
                j++;

                //Tem que ter um espaço entre cada caracter
                posfixa[j] = ' ';
                j++;
            }
            Pop(&pilha, &pop);
        }
    }

    //Enquanto ainda tiver caracteres na pilha
    while(!IsEmpty(&pilha)){

        //Desempilha e bota o caracter na expressão pós-fixa
        Pop(&pilha, &pop);
        posfixa[j] = pop;
        j++;

        //Tem que ter um espaço entre cada caracter
        posfixa[j] = ' ';
        j++;
    }

    posfixa[j] = '\0';
}

int main() {
    char infixa[101], posfixa[201];

    scanf("%100[^\n]", infixa);

    transforma(infixa, posfixa);
    printf("%s\n", posfixa);

    return 0;
}