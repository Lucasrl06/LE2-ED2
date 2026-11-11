#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "avl.h"
#include "arvoreBB.h"

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

int main(){
    srand(time(NULL));
    AVLTree* avl = AVL_Create(compareFn);
    treeHeader* abb = createTree(compareFn);
    printf("Gerando números aleatórios e preenchendo a arvore...\n");
    for(int i = 0;i<10000;i++){
        int* intPtr = malloc(sizeof(int));
        *intPtr = rand() % 10000;
        AVL_Insert(avl,intPtr);
        insertData(abb,intPtr);
    }
    printf("Numeros preenchidos!\n");
    printf("Tamanho da arvore AVL: %d\n",AVL_Height(avl));
    printf("Tamanho da arvore BB: %d",getTreeHeight(abb));
}

