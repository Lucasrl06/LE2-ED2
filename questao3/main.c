#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../questao1/avl.h"

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

void printFn(void*dataPtr){
    int data = *(int*)dataPtr;
    printf(" %d ",data);
}

int main(){
    srand(time(NULL));
    AVLTree* avl = AVL_Create(compareFn);
    printf("Gerando números aleatórios e preenchendo a arvore...\n");
    for(int i = 0;i<100;i++){
        int* intPtr = malloc(sizeof(int));
        *intPtr = rand() % 100;
        AVL_Insert(avl,intPtr);
    }
    AVL_Impressao_Hierarquica(avl,printFn);
}

