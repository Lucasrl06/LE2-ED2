#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../questao1/avl.h"
#include "../questao1/arvoreBB.h"

void visitFn(void* dataPtr){
    int data = *(int*)dataPtr;
    printf(" %d\n",data);
}

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
    for(int i = 0;i<50;i++){
        int* intPtr = malloc(sizeof(int));
        *intPtr = rand() % 50;
        AVL_Insert(avl,intPtr);
        insertData(abb,intPtr);
    }
    printf("---------------------------------------------------------------------------------------------------------------\n");
    printf("Percurso em ordem da árvore AVL\n");
    AVL_traverseInOrder(avl,visitFn);
    printf("---------------------------------------------------------------------------------------------------------------\n");
    printf("Percurso pré ordem da árvore AVL\n");
    AVL_traversePreOrder(avl,visitFn);
    printf("---------------------------------------------------------------------------------------------------------------\n");
    printf("Percurso pós ordem da árvore AVL\n");
    AVL_traversePostOrder(avl,visitFn);
    printf("---------------------------------------------------------------------------------------------------------------\n");
    printf("Percuso em oder da árvore binária de busca\n");
    traverseInOrder(abb,visitFn);
    printf("---------------------------------------------------------------------------------------------------------------\n");
    printf("Percurso pré ordem da árvore binária de busca\n");
    traversePreOrder(abb,visitFn);
    printf("---------------------------------------------------------------------------------------------------------------\n");
    printf("Percurso pós ordem da árvore binária de buca\n");
    traversePostOrder(abb,visitFn);
    printf("---------------------------------------------------------------------------------------------------------------\n");
}