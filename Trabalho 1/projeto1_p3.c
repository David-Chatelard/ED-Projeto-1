/**
 * Aluno: David Fanchic Chatelard
 * Disciplina: Estrutura de Dados
 * Implementação de funções para se calcular uma expressão matemática no formato posfixa(polonesa inversa)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TRUE 1
#define FALSE 0
#define boolean int
#define TamPilha 200

typedef double elem;

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

int isNum(char letra){
    if(letra >= '0' && letra <= '9')
        return 1;
    return 0;
}

int isOperador(char letra){
    if(letra == '+' || letra == '-' || letra == '*' || letra == '/' || letra == '^')
        return 1;
    return 0;
}

double calcula(const char* posfixa) {
    Pilha pilha;
    char aux[TamPilha+1], aux2[TamPilha+1];
    int i, j = 0, len;
    double aux_f;

    Create(&pilha);
    len = strlen(posfixa);

    //Para evitar um aviso da IDE
    for(i = 0; i < len; i++){
        aux[i] = posfixa[i];
    }
    aux[i] = '\0';

    //Percorre a expressão
    for(i = 0; i < len; i++){
        //Se for um número empilha
        if(isNum(aux[i])) {
            while (i < len && (aux[i] != ' ' && !isOperador(aux[i]))) {
                aux2[j] = aux[i];
                j++;
                i++;
            }
            aux2[j] = '\0';
            aux_f = atof(aux2);
            Push(&pilha, &aux_f);
            j = 0;
        }

        //Se for um operador
        if(isOperador(aux[i])){
            switch (aux[i]) {
                case '+':
                    pilha.A[pilha.topo - 1] = pilha.A[pilha.topo-1] + pilha.A[pilha.topo];
                    break;
                case '-':
                    pilha.A[pilha.topo - 1] = pilha.A[pilha.topo-1] - pilha.A[pilha.topo];
                    break;
                case '*':
                    pilha.A[pilha.topo - 1] = pilha.A[pilha.topo-1] * pilha.A[pilha.topo];
                    break;
                case '/':
                    pilha.A[pilha.topo - 1] = pilha.A[pilha.topo-1] / pilha.A[pilha.topo];
                    break;
                case '^':
                    //Não vai compilar no CLion pq eu não sei botar o -lm, testa no sublime
                    pilha.A[pilha.topo - 1] = pow(pilha.A[pilha.topo-1], pilha.A[pilha.topo]);
                    break;
            }
            pilha.topo--;
        }

    }
    return Top(&pilha);
}

int main() {
    char posfixa[201];

    scanf("%200[^\n]", posfixa);
    printf("%.5lf\n", calcula(posfixa));

    return 0;
}
