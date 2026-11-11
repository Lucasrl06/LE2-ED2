#ifndef AVL_H
#define AVL_H


typedef struct stAVLTree AVLTree;


typedef int (*AVLCompareFn)(void *arg1, void *arg2);



AVLTree *AVL_Create(AVLCompareFn compare);


int AVL_Count(AVLTree *tree);

int AVL_Height(AVLTree *tree);


int AVL_FULL(AVLTree *tree);


int AVL_AVLEmpty(AVLTree *tree);

void AVL_Insert(AVLTree *tree, void *dataPtr);


void AVL_Delete(AVLTree *tree, void *targetPtr);


void *AVL_Retrieve(AVLTree *tree, void *targetPtr);


void AVL_traverseInOrder(AVLTree* tree, void (*process)(void*));

void AVL_traversePreOrder(AVLTree *tree, void (*process)(void *dataPtr));

void AVL_traversePostOrder(AVLTree *tree, void (*process)(void *dataPtr));


void AVL_Destroy(AVLTree *tree);

void AVL_Impressao_Hierarquica(AVLTree *tree,void (*printFn)(void*));

#endif /* AVL_H */
