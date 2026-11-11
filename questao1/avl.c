#include "avl.h"
#include "stdio.h"
#include <stdlib.h>

/* Fatores de balanceamento: altura(esquerda) - altura(direita). */
#define LH  1
#define EH  0
#define RH -1

typedef struct stNode {
    struct stNode *left;
    struct stNode *right;
    void *dataPtr;
    int bal;
} Node;

/* A definição concreta fica restrita a este arquivo. */
struct stAVLTree {
    int count;
    Node *root;
    AVLCompareFn compare;
};

/* Funções auxiliares privadas. */
static Node *createNode(void *dataPtr);
static Node *searchNode(Node *root, void *keyPtr, AVLCompareFn compare);
static void _traverseInOrder(Node* localRoot, void (*process)(void*));
static void _traversePreOrder(Node* localRoot, void (*process)(void*));
static void _traversePostOrder(Node* localRoot, void (*process)(void*));
static int treeHeight(Node *root);
static void destroyTree(Node *root);
static Node *insertNode(AVLTree *tree, Node *root, Node *newNode, int *taller);
static Node *insertLeftBalance(Node *root, int *taller);
static Node *insertRightBalance(Node *root, int *taller);
static void _printDataAndLevel(Node *root,void (*printFn)(void*),int nivel);
static void _hierarquia_print_nodes(Node *root,int nivel,void (*printFn)(void*));
static void _printDataAndLevel(Node *root,void (*printFn)(void*),int nivel);
static Node *deleteNode(AVLTree *tree, Node *root, void *targetPtr,int *shorter, int *success);
static Node *deleteFoundNode(AVLTree *tree, Node *root,int *shorter, int *success);
static Node *deleteLeftBalance(Node *root, int *shorter);
static Node *deleteRightBalance(Node *root, int *shorter);
static Node *rotateLeft(Node *root);
static Node *rotateRight(Node *root);

AVLTree *AVL_Create(AVLCompareFn compare)
{
    AVLTree *tree;

    if (compare == NULL) {
        return NULL;
    }

    tree = malloc(sizeof(*tree));
    if (tree == NULL) {
        return NULL;
    }

    tree->count = 0;
    tree->root = NULL;
    tree->compare = compare;

    return tree;
}

int AVL_Count(AVLTree *tree)
{
    return tree != NULL ? tree->count : 0;
}

int AVL_Height(AVLTree *tree)
{
    return tree != NULL ? treeHeight(tree->root) : 0;
}

int AVL_FULL(AVLTree *tree)
{
    Node *probe;

    /* O parâmetro é mantido por compatibilidade com a interface original. */
    (void)tree;

    probe = malloc(sizeof(*probe));
    if (probe == NULL) {
        return 1;
    }

    free(probe);
    return 0;
}

int AVL_AVLEmpty(AVLTree *tree)
{
    return tree != NULL && tree->count == 0;
}

void AVL_Insert(AVLTree *tree, void *dataPtr)
{
    Node *newNode;
    int taller = 0;

    if (tree == NULL || dataPtr == NULL) {
        return;
    }

    newNode = createNode(dataPtr);
    if (newNode == NULL) {
        return;
    }

    tree->root = insertNode(tree, tree->root, newNode, &taller);
    tree->count++;
}

void AVL_Delete(AVLTree *tree, void *targetPtr)
{
    int shorter = 0;
    int success = 0;

    if (tree == NULL || targetPtr == NULL || tree->root == NULL) {
        return;
    }

    tree->root = deleteNode(tree, tree->root, targetPtr, &shorter, &success);

    if (success) {
        tree->count--;
    }
}

void *AVL_Retrieve(AVLTree *tree, void *targetPtr)
{
    Node *found;

    if (tree == NULL || targetPtr == NULL || tree->root == NULL) {
        return NULL;
    }

    found = searchNode(tree->root, targetPtr, tree->compare);
    return found != NULL ? found->dataPtr : NULL;
}


void AVL_Destroy(AVLTree *tree)
{
    if (tree == NULL) {
        return;
    }

    destroyTree(tree->root);
    free(tree);
}

/* -------------------------------------------------------------------------- */
/* Busca, percurso, criação e destruição                                      */
/* -------------------------------------------------------------------------- */

static Node *createNode(void *dataPtr)
{
    Node *newNode;

    if (dataPtr == NULL) {
        return NULL;
    }

    newNode = malloc(sizeof(*newNode));
    if (newNode == NULL) {
        return NULL;
    }

    newNode->left = NULL;
    newNode->right = NULL;
    newNode->dataPtr = dataPtr;
    newNode->bal = EH;

    return newNode;
}

static Node *searchNode(Node *root, void *keyPtr, AVLCompareFn compare)
{
    int result;

    if (root == NULL || keyPtr == NULL || compare == NULL) {
        return NULL;
    }

    result = compare(root->dataPtr, keyPtr);

    if (result == 0) {
        return root;
    }

    if (result > 0) {
        return searchNode(root->left, keyPtr, compare);
    }

    return searchNode(root->right, keyPtr, compare);
}

void AVL_traverseInOrder(AVLTree* tree, void (*process)(void*)){
    if(!tree || !process) return;
    _traverseInOrder(tree->root, process);
}

void AVL_traversePreOrder(AVLTree* tree, void (*process)(void*)){
    if(!tree || !process) return;
    _traversePreOrder(tree->root, process);
}

void AVL_traversePostOrder(AVLTree* tree, void (*process)(void*)){
    if(!tree || !process) return;
    _traversePostOrder(tree->root, process);
}

static void _traverseInOrder(Node* localRoot, void (*process)(void*)){
    if(!localRoot || !process) return;
    _traverseInOrder(localRoot->left, process);
    process(localRoot->dataPtr);
    _traverseInOrder(localRoot->right, process);
}

static void _traversePreOrder(Node* localRoot, void (*process)(void*)){
    if(!localRoot || !process) return;
    process(localRoot->dataPtr);
    _traversePreOrder(localRoot->left, process);
    _traversePreOrder(localRoot->right, process);
}

static void _traversePostOrder(Node* localRoot, void (*process)(void*)){
    if(!localRoot || !process) return;
    _traversePostOrder(localRoot->left, process);
    _traversePostOrder(localRoot->right, process);
    process(localRoot->dataPtr);
}

static int treeHeight(Node *root)
{
    int leftHeight;
    int rightHeight;

    if (root == NULL) {
        return 0;
    }

    leftHeight = treeHeight(root->left);
    rightHeight = treeHeight(root->right);
    return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

static void destroyTree(Node *root)
{
    if (root == NULL) {
        return;
    }

    destroyTree(root->left);
    destroyTree(root->right);

    /* A biblioteca assume a propriedade dos dados inseridos. */
    free(root->dataPtr);
    free(root);
}

/* -------------------------------------------------------------------------- */
/* Inserção                                                                   */
/* -------------------------------------------------------------------------- */

static Node *insertNode(AVLTree *tree, Node *root, Node *newNode, int *taller)
{
    int result;

    if (root == NULL) {
        *taller = 1;
        return newNode;
    }

    result = tree->compare(newNode->dataPtr, root->dataPtr);

    /* Valores equivalentes são inseridos à direita, como na versão original. */
    if (result < 0) {
        root->left = insertNode(tree, root->left, newNode, taller);

        if (*taller) {
            switch (root->bal) {
                case LH:
                    root = insertLeftBalance(root, taller);
                    break;

                case EH:
                    root->bal = LH;
                    break;

                case RH:
                    root->bal = EH;
                    *taller = 0;
                    break;
            }
        }
    } else {
        root->right = insertNode(tree, root->right, newNode, taller);

        if (*taller) {
            switch (root->bal) {
                case LH:
                    root->bal = EH;
                    *taller = 0;
                    break;

                case EH:
                    root->bal = RH;
                    break;

                case RH:
                    root = insertRightBalance(root, taller);
                    break;
            }
        }
    }

    return root;
}

static Node *insertLeftBalance(Node *root, int *taller)
{
    Node *leftTree = root->left;

    switch (leftTree->bal) {
        case LH: /* Caso esquerda-esquerda: rotação simples à direita. */
            /* Salva e corrige os fatores antes da rotação: leftTree passa a ser
               a nova raiz e, portanto, não pode ser usado para identificar A. */
            root->bal = EH;
            leftTree->bal = EH;
            root = rotateRight(root);
            *taller = 0;
            break;

        case RH: { /* Caso esquerda-direita: rotação dupla. */
            Node *rightTree = leftTree->right;

            switch (rightTree->bal) {
                case LH:
                    root->bal = RH;
                    leftTree->bal = EH;
                    break;

                case EH:
                    root->bal = EH;
                    leftTree->bal = EH;
                    break;

                case RH:
                    root->bal = EH;
                    leftTree->bal = LH;
                    break;
            }

            rightTree->bal = EH;
            root->left = rotateLeft(leftTree);
            root = rotateRight(root);
            *taller = 0;
            break;
        }
    }

    return root;
}

static Node *insertRightBalance(Node *root, int *taller)
{
    Node *rightTree = root->right;

    switch (rightTree->bal) {
        case RH: /* Caso direita-direita: rotação simples à esquerda. */
            root->bal = EH;
            rightTree->bal = EH;
            root = rotateLeft(root);
            *taller = 0;
            break;

        case LH: { /* Caso direita-esquerda: rotação dupla. */
            Node *leftTree = rightTree->left;

            switch (leftTree->bal) {
                case LH:
                    root->bal = EH;
                    rightTree->bal = RH;
                    break;

                case EH:
                    root->bal = EH;
                    rightTree->bal = EH;
                    break;

                case RH:
                    root->bal = LH;
                    rightTree->bal = EH;
                    break;
            }

            leftTree->bal = EH;
            root->right = rotateRight(rightTree);
            root = rotateLeft(root);
            *taller = 0;
            break;
        }
    }

    return root;
}

/* -------------------------------------------------------------------------- */
/* Rotações estruturais                                                       */
/* -------------------------------------------------------------------------- */

static Node *rotateLeft(Node *root)
{
    Node *newRoot = root->right;

    root->right = newRoot->left;
    newRoot->left = root;

    return newRoot;
}

static Node *rotateRight(Node *root)
{
    Node *newRoot = root->left;

    root->left = newRoot->right;
    newRoot->right = root;

    return newRoot;
}

/* -------------------------------------------------------------------------- */
/* Remoção                                                                    */
/* -------------------------------------------------------------------------- */

static Node *deleteNode(AVLTree *tree, Node *root, void *targetPtr,
                        int *shorter, int *success)
{
    int result;

    if (root == NULL) {
        *shorter = 0;
        *success = 0;
        return NULL;
    }

    result = tree->compare(targetPtr, root->dataPtr);

    if (result < 0) {
        root->left = deleteNode(tree, root->left, targetPtr, shorter, success);

        if (*shorter) {
            root = deleteRightBalance(root, shorter);
        }

        return root;
    }

    if (result > 0) {
        root->right = deleteNode(tree, root->right, targetPtr, shorter, success);

        if (*shorter) {
            root = deleteLeftBalance(root, shorter);
        }

        return root;
    }

    return deleteFoundNode(tree, root, shorter, success);
}

static Node *deleteFoundNode(AVLTree *tree, Node *root,
                             int *shorter, int *success)
{
    Node *replacement;

    /* Caso 1 ou 2: o nó possui zero ou um filho. */
    if (root->left == NULL || root->right == NULL) {
        replacement = root->left != NULL ? root->left : root->right;

        free(root->dataPtr);
        free(root);

        *shorter = 1;
        *success = 1;
        return replacement;
    }

    /* Caso 3: dois filhos. Usa o predecessor em ordem (maior da esquerda). */
    {
        Node *predecessor = root->left;
        void *removedData;

        while (predecessor->right != NULL) {
            predecessor = predecessor->right;
        }

        /* Troca os ponteiros para que a remoção recursiva libere o dado antigo
           da raiz junto com o nó do predecessor. */
        removedData = root->dataPtr;
        root->dataPtr = predecessor->dataPtr;
        predecessor->dataPtr = removedData;

        root->left = deleteNode(tree, root->left, removedData, shorter, success);

        if (*shorter) {
            root = deleteRightBalance(root, shorter);
        }
    }

    return root;
}

/* A subárvore esquerda diminuiu. */
static Node *deleteRightBalance(Node *root, int *shorter)
{
    Node *rightTree;
    Node *leftTree;

    switch (root->bal) {
        case LH:
            root->bal = EH;
            break;

        case EH:
            root->bal = RH;
            *shorter = 0;
            break;

        case RH:
            rightTree = root->right;

            /* Caso direita-esquerda: rotação dupla. */
            if (rightTree->bal == LH) {
                leftTree = rightTree->left;

                switch (leftTree->bal) {
                    case LH:
                        root->bal = EH;
                        rightTree->bal = RH;
                        break;

                    case EH:
                        root->bal = EH;
                        rightTree->bal = EH;
                        break;

                    case RH:
                        root->bal = LH;
                        rightTree->bal = EH;
                        break;
                }

                leftTree->bal = EH;
                root->right = rotateRight(rightTree);
                root = rotateLeft(root);
            } else {
                /* Caso direita-direita: rotação simples à esquerda. */
                switch (rightTree->bal) {
                    case RH:
                        root->bal = EH;
                        rightTree->bal = EH;
                        break;

                    case EH:
                        root->bal = RH;
                        rightTree->bal = LH;
                        *shorter = 0;
                        break;
                }

                root = rotateLeft(root);
            }
            break;
    }

    return root;
}

/* A subárvore direita diminuiu. */
static Node *deleteLeftBalance(Node *root, int *shorter)
{
    Node *leftTree;
    Node *rightTree;

    switch (root->bal) {
        case RH:
            root->bal = EH;
            break;

        case EH:
            root->bal = LH;
            *shorter = 0;
            break;

        case LH:
            leftTree = root->left;

            /* Caso esquerda-direita: rotação dupla. */
            if (leftTree->bal == RH) {
                rightTree = leftTree->right;

                switch (rightTree->bal) {
                    case LH:
                        root->bal = RH;
                        leftTree->bal = EH;
                        break;

                    case EH:
                        root->bal = EH;
                        leftTree->bal = EH;
                        break;

                    case RH:
                        root->bal = EH;
                        leftTree->bal = LH;
                        break;
                }

                rightTree->bal = EH;
                root->left = rotateLeft(leftTree);
                root = rotateRight(root);
            } else {
                /* Caso esquerda-esquerda: rotação simples à direita. */
                switch (leftTree->bal) {
                    case LH:
                        root->bal = EH;
                        leftTree->bal = EH;
                        break;

                    case EH:
                        root->bal = LH;
                        leftTree->bal = RH;
                        *shorter = 0;
                        break;
                }

                root = rotateRight(root);
            }
            break;
    }

    return root;
}

void AVL_Impressao_Hierarquica(AVLTree *tree,void (*printFn)(void*)){
    if (!tree) return;
    _hierarquia_print_nodes(tree->root,0,printFn);
}

static void _hierarquia_print_nodes(Node *root,int nivel,void (*printFn)(void*)){
    if(!root) return;
    _hierarquia_print_nodes(root->left,nivel+1,printFn);
    _printDataAndLevel(root,printFn,nivel);
    _hierarquia_print_nodes(root->right,nivel+1,printFn);
}

static void _printDataAndLevel(Node *root,void (*printFn)(void*),int nivel){
    for(int i = 0;i<nivel;i++){
        printf(" ");
    }
    printf("\n");
    printf("%d (",nivel);
    printFn(root->dataPtr);
    printf(") ");
    printf("[%d]",root->bal);
}
