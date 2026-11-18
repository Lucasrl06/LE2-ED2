#include "../questao1/avl.h"
#include "arbe.h"
#include "time.h"
#include <stdio.h>
#include <stdlib.h>

int compareFn(void* int1, void* int2){
    int val1 = *(int*)int1;
    int val2 = *(int*)int2;
    if(val1 == val2){
        return 0;
    }
    else if (val1 > val2){
        return 1;
    }
    return -1;
}

int main (){
    AVLTree *avl = AVL_Create(compareFn);
    ArvLLRB* rubroNegra = cria_ArvLLRB();
    printf("Criando 10.000 numeros aleatórios...\n");
    srand(time(NULL));
    for(int i = 0;i<10000;i++){
        int* intPtr = malloc(sizeof(int));
        *intPtr = rand() % 10000;
        AVL_Insert(avl,intPtr);
        insere_ArvLLRB(rubroNegra,*intPtr);
    }
    printf("Altura da árvore Rubro Negra caida para a esquerda: %d\n",altura_ArvLLRB(rubroNegra));
    printf("Altura da arvore AVL: %d\n",AVL_Height(avl));
}