typedef struct stAVLTree AVLTree;

AVLTree* AVL_Create(int (*compare) (void*,void*));
int AVL_Count(AVLTree *tree);
int AVL_FULL(AVLTree *tree);
int AVL_AVLEmpty(AVLTree *tree);
void AVL_Insert(AVLTree *tree,void *dataPtr);
void AVL_Delete(AVLTree *tree,void *targetPtr);
void* AVL_Retrieve(AVLTree *tree,void *targetPtr);
void AVL_Traverse(AVLTree *tree,void (*compare)(void *dataPtr));
void AVL_Destroy(AVLTree *tree);