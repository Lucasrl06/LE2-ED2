#include <stdlib.h>
#include <string.h>
#define BIGGER 1
#define EQUALS 0
#define LOWER -1
#define SUCCESS 1
#define FAIL 0

typedef struct stNode {
    void* data;
    struct stNode *left;
    struct stNode *right;
} node;

typedef struct stTree {
    node* root;
    int (*compareFunction) (void* argu1,void* argu2);
    size_t dataSize;
} treeHeader;

static void _freeAllNodesAndTheirData(node* root);
static node* _createNode(treeHeader* tree, void* dataPtr);
static int _fillNodeData(treeHeader* tree, node* emptyNode, void* dataPtr);

static node* _findParentNode(
    int compareFunction(void*, void*),
    node* localRoot,
    void* newDataPtr
);

static void _connectChildNode(
    int compareFunction(void*, void*),
    node* parentNode,
    node* newNode
);

static node* findNodeAndParent(
    treeHeader* tree,
    void* dataPtr,
    node** parent
);

void _deleteNode(node* nodeToDelete);
int _getNumberOfSons(node* localRoot);
void _removeNodeWithNoSons(treeHeader*, node*, node*);
void _removeNodeWithOneSon(treeHeader*, node*, node*);
void removeNodeWithTwoSons(treeHeader*, node*, node*);
int _isTheRightSon(node*, node*);
int _isTheLeftSon(node*, node*);
int _isTreeRoot(treeHeader*, node*);
void _removeRoot(treeHeader*);

treeHeader* createTree (int (*compareFunction) (void* arg1,void* arg2),size_t dataSize){
    if (!compareFunction || dataSize == 0) return NULL;
    treeHeader* tree = (treeHeader*) malloc(sizeof(treeHeader));
    if(!tree) return NULL;
    tree->root = NULL;
    tree->compareFunction = compareFunction;
    tree->dataSize = dataSize;
    return tree;
}

void destroyTree(treeHeader* tree){
    if(!tree) return;
    _freeAllNodesAndTheirData(tree->root);
    free(tree);
}

static void _freeAllNodesAndTheirData(node* root){
    if(!root) return;
    _freeAllNodesAndTheirData(root->left);
    _freeAllNodesAndTheirData(root->right);
    _deleteNode(root);
}

void insertData(treeHeader *tree,void* dataPtr){
    if(!tree || !dataPtr) return;
    if(!tree->root){
        tree->root = _createNode(tree,dataPtr);
        return;
    }
    node* parentNode = _findParentNode(tree->compareFunction,tree->root,dataPtr);
    if(!parentNode) return;
    node* newNode = _createNode(tree,dataPtr);
    if(!newNode) return;
    _connectChildNode(tree->compareFunction,parentNode,newNode);
}

static node* _findParentNode(int compareFunction(void* arg1,void* arg2),node* localRoot,void* newDataPtr){
    if(!localRoot) return NULL;
    int result = compareFunction(localRoot->data,newDataPtr);
    if(result == EQUALS) return NULL;
    if(
        (result == LOWER && !localRoot->right) || 
        (result == BIGGER && !localRoot->left) 
    ) 
    return localRoot;
    if(result == BIGGER) return _findParentNode(compareFunction,localRoot->left,newDataPtr);
    if(result == LOWER) return _findParentNode(compareFunction,localRoot->right,newDataPtr);
    return NULL;
}

static node* _createNode(treeHeader* tree,void* dataPtr){
    node* newNode = (node*) malloc(sizeof(node));
    if(!newNode) return NULL;
    int result = _fillNodeData(tree,newNode,dataPtr);
    if(result == SUCCESS) return newNode;
    else if (result == FAIL){
        free(newNode);
    }
    return NULL;
}
static int _fillNodeData(treeHeader *tree,node *emptyNode,void *dataPtr){
    emptyNode->data = malloc(tree->dataSize);
    if(!emptyNode->data) return FAIL;
    memcpy(emptyNode->data,dataPtr,tree->dataSize);
    emptyNode->left = NULL;
    emptyNode->right = NULL;
    return SUCCESS;
}

static void _connectChildNode(int (compareFunction)(void* arg1,void*arg2),node *parentNode,node *newNode){
    int result = compareFunction(parentNode->data,newNode->data);
    if(result ==  BIGGER) parentNode->left = newNode;
    else if(result == LOWER) parentNode->right = newNode;
}

void removeElement(treeHeader *tree,void *dataPtr){
    if (!tree || !dataPtr) return;
    node *nodeToDelete = NULL;
    node *fatherOfNodeToDelete = NULL;
    nodeToDelete = findNodeAndParent(tree, dataPtr, &fatherOfNodeToDelete);    
    if(!nodeToDelete) return;
    int numOfSons = _getNumberOfSons(nodeToDelete);
    if(numOfSons == 0){
        _removeNodeWithNoSons(tree,nodeToDelete,fatherOfNodeToDelete);
        return;
    }
    if(numOfSons == 1){
        _removeNodeWithOneSon(tree,nodeToDelete,fatherOfNodeToDelete);
        return;
    }
    if(numOfSons == 2){
        removeNodeWithTwoSons(tree,nodeToDelete,fatherOfNodeToDelete);
    }
}

static node* findNodeAndParent(
    treeHeader* tree,
    void* dataPtr,
    node** parent
) {
    *parent = NULL;

    if (!tree || !dataPtr)
        return NULL;

    node* current = tree->root;

    while (current) {
        int result = tree->compareFunction(
            current->data,
            dataPtr
        );

        if (result == EQUALS)
            return current;

        *parent = current;

        if (result == BIGGER)
            current = current->left;
        else if (result == LOWER)
            current = current->right;
        else
            return NULL;
    }

    *parent = NULL;
    return NULL;
}
static int _getNumberOfSons(node *localRoot){
    int count = 2;
    if(!localRoot->left) count--;
    if(!localRoot->right) count--;
    return count;
}

void _deleteNode(node* nodeToDelete){
    free(nodeToDelete->data);
    free(nodeToDelete);
}

void _removeNodeWithNoSons(treeHeader *tree,node *nodeToDelete,node *fatherOfNodeToDelete){
    if(_isTreeRoot(tree,nodeToDelete)){
        _removeRoot(tree);
        return;
    }
    if(_isTheRightSon(nodeToDelete,fatherOfNodeToDelete)) fatherOfNodeToDelete->right = NULL;
    if(_isTheLeftSon(nodeToDelete,fatherOfNodeToDelete)) fatherOfNodeToDelete->left = NULL;
    _deleteNode(nodeToDelete);
}
int _isTheRightSon(node *sonNode,node *fatherNode){
    if(!sonNode || !fatherNode) return 0;
    return fatherNode->right == sonNode;
}
int _isTheLeftSon(node *sonNode,node *fatherNode){
    if(!sonNode || !fatherNode) return 0;
    return sonNode == fatherNode->left;
}
int _isTreeRoot(treeHeader *tree,node* localRoot){
    if (!tree || !localRoot) return 0;
    return tree->root == localRoot;
}
void _removeRoot(treeHeader *tree){
    _deleteNode(tree->root);
    tree->root = NULL;
}
void _removeNodeWithOneSon(treeHeader *tree,node *nodeToDelete,node *fatherOfNodeToDelete){
    if(!tree || !nodeToDelete) return;
    node *sonOfNodeToDelete = NULL;
    if(nodeToDelete->right){
        sonOfNodeToDelete = nodeToDelete->right;
    }
    else if (nodeToDelete->left){
        sonOfNodeToDelete = nodeToDelete->left;
    }
    if(!sonOfNodeToDelete) return;
    if (_isTreeRoot(tree, nodeToDelete)) {
        tree->root = sonOfNodeToDelete;
    }
    else if(_isTheRightSon(nodeToDelete,fatherOfNodeToDelete)){
        fatherOfNodeToDelete->right = sonOfNodeToDelete;
    }
    else if (_isTheLeftSon(nodeToDelete,fatherOfNodeToDelete)){
        fatherOfNodeToDelete->left = sonOfNodeToDelete;
    }
    else {
        return;
    }
    _deleteNode(nodeToDelete);
}

node* findBiggerElementInTree(node *localRoot){
    if(!localRoot->right) return localRoot;
    return findBiggerElementInTree(localRoot->right);
}

node* findLowerElementInTree(node *localRoot){
    if(!localRoot->left) return localRoot;
    return findLowerElementInTree(localRoot->left);
}
void removeNodeWithTwoSons(treeHeader *tree,node *nodeToDelete,node *fatherOfNodeToDelete){
    //Encontra o sucessor imediato e seu pai
    node* successor = findLowerElementInTree(nodeToDelete->right);
    node* fatherOfSuccessor = NULL;
    findNodeAndParent(tree,successor->data,&fatherOfSuccessor);
    //Se o sucessor não for o filho direito do nó a ser deletado, ele será o esquerdo do próprio pai
    if (fatherOfSuccessor != nodeToDelete) { 
        fatherOfSuccessor->left = successor->right;
    }
    //Conectar as sub-árvores do nó a ser deletado com o successor
    successor->left = nodeToDelete->left;
    if(nodeToDelete->right != successor){ //Se o sucessor imediato for o filho do nó a ser deletado não conectamos
        successor->right = nodeToDelete->right;
    }
    //conectar o pai do nó a ser deletado com o sucessor dele
    if(_isTreeRoot(tree,nodeToDelete)){
        tree->root = successor;
    }
    else if(_isTheRightSon(nodeToDelete,fatherOfNodeToDelete)){
        fatherOfNodeToDelete->right = successor;
    }
    else {
        fatherOfNodeToDelete->left = successor;
    }
    //Deletar o nó
    _deleteNode(nodeToDelete);
}

int getTreeHeight(treeHeader *tree){
    if(!tree || !tree->root) return 0;
    return _getHeightOfBiggestSubtree(tree->root);
}

static int _getHeightOfBiggestSubtree(node *localRoot){
    if(!localRoot) return 0;
    return max(_getHeightOfBiggestSubtree(localRoot->left),_getHeightOfBiggestSubtree(localRoot->right)) + 1;
}

int max (int val1,int val2){
    return val1 > val2 ? val1 : val2;
}
