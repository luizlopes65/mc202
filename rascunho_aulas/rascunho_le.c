#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

struct no {
    int num;
    int ind;
    struct no* prox;
};

typedef struct no no;

struct lista{
    no* head;
    int size;
};

typedef struct lista lista;

void atualizar_indices(lista* list){
    int ind = 0;
    no* p = list->head;
    while (p != NULL){
        p->ind = ind;
        ind++;
        p = p->prox;
    }
    list->size = ind;
}

int add_no_fim(lista* L, int val){
    no* novo = malloc(sizeof(no));
    if (novo == NULL) return 0;

    novo->num = val;
    novo->prox = NULL;

    if (L->head == NULL){
        L->head = novo;
        novo->prox = NULL;
    } else {
        no* p = L->head;
        while (p->prox != NULL) p = p->prox;
        p->prox = novo;
    }

    atualizar_indices(L);

    return 1;
}


int add_no_inicio(lista* list, int val){

    no* novo = malloc(sizeof(no));

    if (novo == NULL) return 0;

    novo->num = val;

    if (list->head == NULL){
        list->head = novo;
        novo->prox = NULL;
   } else{
        novo->prox = list->head;
        list->head = novo;
   }

   atualizar_indices(list);

   return 1;
}

int add_no_ind(lista* L, int val, int ind){
    if (ind == 0){
        add_no_inicio(L, val);
        return 1;
    } else if (ind == L->size - 1){
        add_no_fim(L, val);
        return 1;
    }

    no* novo = malloc(sizeof(no));

    if (novo == NULL) return 0;

    novo->num = val;

    no* p = L->head;
    for (int i = 0; i < ind - 1; i++) {
        p = p->prox;
    }
    novo->prox = p->prox;
    p->prox = novo;
}

void imprimir_lista(lista* L){
    no* p = L->head;

    while (p != NULL){
        printf("[%d] ", p->num);
        p = p->prox;
    }

}

void free_lista(lista* L){
    while (L->head != NULL){
        no* temp = L->head;
        L->head = temp->prox;
        free(temp);
        
    }
}

lista* criar_lista(){
    lista* L = malloc(sizeof(lista));
    if (L == NULL) return NULL;
    L->size = 0;
    L->head = NULL;

    return L;
}


int main(){
    lista* list1 = criar_lista();

    add_no_inicio(list1, 2);
    add_no_inicio(list1, 3);
    imprimir_lista(list1);
    printf("\n");
    add_no_inicio(list1, 4);
    add_no_inicio(list1, 5);
    imprimir_lista(list1);
    free_lista(list1);

    return 0;

}