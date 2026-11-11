#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <stddef.h>

typedef struct stNode node;
typedef struct stTree treeHeader;

/*
 * A função de comparação deve retornar:
 *   -1: primeiro elemento < segundo
 *    0: elementos iguais
 *    1: primeiro elemento > segundo
 *
 * dataSize é o tamanho, em bytes, de cada elemento armazenado.
 */
treeHeader *createTree(
    int (*compareFunction)(void *arg1, void *arg2));

void destroyTree(treeHeader *tree);

void insertData(treeHeader *tree, void *dataPtr);

void removeElement(treeHeader *tree, void *dataPtr);

int getTreeHeight(treeHeader *tree);

void traverseInOrder(treeHeader *tree, void (*process)(void *dataPtr));

void traversePreOrder(treeHeader *tree, void (*process)(void *dataPtr));

void traversePostOrder(treeHeader *tree, void (*process)(void *dataPtr));

#endif /* BINARY_TREE_H */
