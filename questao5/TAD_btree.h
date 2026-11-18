#ifndef BTREE_H
#define BTREE_H

typedef struct BTree BTree;

// Criação
BTree* create_tree(void);

// Inserção
void insert(BTree* tree, int data);

// Remoção
int remove_data(BTree* tree, int data);

// Busca e recuperação
int recover_data(BTree* tree, int data);

// Impressão
void show_tree(BTree* tree);

// Liberação
void free_tree(BTree* tree);

#endif