#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

typedef struct node {
    char val;
    struct node* left;
    struct node* right;
} no;

typedef struct no_fila {
    no* val;
    struct no_fila* prox;
} no_fila;

typedef struct fila {
    no_fila* head;
    no_fila* tail;
    int size;
} fila;

void free_lista(fila* L) {
    while (L->head != NULL) {
        no_fila* temp = L->head;
        L->head = temp->prox;
        free(temp);
    }
    free(L);
}

int push(fila* L, no* val) {
    no_fila* novo = malloc(sizeof(no_fila));
    if (novo == NULL) return 0;

    novo->val = val;
    novo->prox = NULL;

    if (L->tail == NULL) {  
        L->head = L->tail = novo;
    } else {
        L->tail->prox = novo;
        L->tail = novo;
    }

    L->size++;
    return 1;
}

no_fila* eject(fila* L) {
    if (L->head == NULL) return NULL;

    no_fila* temp = L->head;
    L->head = L->head->prox;
    if (L->head == NULL) L->tail = NULL;

    temp->prox = NULL;
    L->size--;
    return temp;
}

fila* criar_fila() {
    fila* f = malloc(sizeof(fila));
    if (f == NULL) return NULL;
    f->head = f->tail = NULL;
    f->size = 0;
    return f;
}

void post_order(no* x) {
    if (x == NULL) return;
    post_order(x->left);
    post_order(x->right);
    printf("%c", x->val);
}


void busca_em_largura(no* x) {
    if (!x) return;              

    fila* queue = criar_fila();
    push(queue, x);

    while (queue->size > 0) {    
        no_fila* wrapper = eject(queue);
        no* atual = wrapper->val;

        printf("%c", atual->val);

        if (atual->left)  push(queue, atual->left);
        if (atual->right) push(queue, atual->right);

        free(wrapper);           
    }

    free_lista(queue);          
}

void free_arvore(no* raiz) {
    if (raiz == NULL) return;
    free_arvore(raiz->left);
    free_arvore(raiz->right);
    free(raiz);
}

no* reconstroi_arvore(char* pre_order, char* in_order, int len) {
    if (len <= 0) return NULL;

    char raiz = pre_order[0];
    no* novo = malloc(sizeof(no));
    novo->val = raiz;
    novo->left = novo->right = NULL;

    int pos;
    for (pos = 0; pos < len; pos++) {
        if (in_order[pos] == raiz) break;
    }

    novo->left = reconstroi_arvore(pre_order + 1, in_order, pos);
    novo->right = reconstroi_arvore(pre_order + 1 + pos, in_order + pos + 1, len - pos - 1);

    return novo;
}


int main() {
    char seq_pre[128], seq_in[128]; 
    while (scanf(" %127s %127s", seq_pre, seq_in) == 2) {
        int len = strlen(seq_pre);
        no* raiz = reconstroi_arvore(seq_pre, seq_in, len);

        post_order(raiz);
        printf(" ");

        busca_em_largura(raiz);
        printf("\n");

        free_arvore(raiz);
    }
    return 0;
}