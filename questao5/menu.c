#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../questao5/TAD_btree.h"

int main(void) {
    int opcao, valor;

    BTree* arvore = create_tree();

    srand(time(NULL));

    int quantidade = 0;

    while (quantidade < 100) {
      int numero = rand() % 1000 + 1;

      if (!recover_data(arvore, numero)) {
         insert(arvore, numero);
          quantidade++;
    }
}

    if (arvore == NULL)
        return 1;


    do {
        printf("\n===== MENU B-TREE =====\n");
        printf("1 - Inserir\n");
        printf("2 - Remover\n");
        printf("3 - Recuperar dados\n");
        printf("4 - Imprimir arvore\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Valor para inserir: ");
                scanf("%d", &valor);
                insert(arvore, valor);
                break;

            case 2:
                printf("Valor para remover: ");
                scanf("%d", &valor);
                int dado = remove_data(arvore, valor);
                printf("%d foi removido", dado);
                break;

            case 3:
                printf("Valor para buscar: ");
                scanf("%d", &valor);
                if (recover_data(arvore, valor)) {
                    printf("%d encontrado.\n", valor);
                } else {
                    printf("%d nao encontrado.\n", valor);
                    }
                break;

            case 4:
                show_tree(arvore);
                break;

            case 0:
                printf("Encerrando...\n");
                free_tree(arvore);
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}