#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

struct no {
    int num;
    int indice;
    struct no* prox;

};

typedef struct no no;

void atualizar_indices(no* l) {
    int idx = 0;
    while (l != NULL) {
        l->indice = idx++;
        l = l->prox;
    }
}

int inserir_no_inicio(no** l, int valor){
    no* novo = malloc(sizeof(no));
    if (novo == NULL) return 0;

    novo->num = valor;
    novo->prox = *l;
    *l = novo;

    atualizar_indices(*l);
    return 1;
}

int inserir_no_fim(no** l, int valor){
    no* novo = malloc(sizeof(no));
    if (novo == NULL) return 0;

    novo->num = valor;
    novo->prox = NULL;

    if (*l == NULL) {
        *l = novo;
    } else {
        no* p = *l;
        while(p->prox != NULL){
            p = p->prox;
        }
        p->prox = novo;
    }

    atualizar_indices(*l);
    return 1;
}

no* copiar_intervalo(no* l, int i, int j) {
    no* sublista = NULL;
    no* p = l;

    if (i <= j) {
        while (p != NULL) {
            if (p->indice >= i && p->indice <= j)
                inserir_no_fim(&sublista, p->num);
            p = p->prox;
        }
    } else {
        while (p != NULL) {
            if (p->indice >= j && p->indice <= i)
                inserir_no_inicio(&sublista, p->num);
            p = p->prox;
        }
    }

    return sublista;
}

void imprimir_lista(no* l){
    no* p = l;
    printf("A: { ");
    while(p != NULL){
        printf("%d", p->num);
        if(p->prox != NULL)
            printf(", ");
        p = p->prox;
    };
    printf(" }\n");
}

void free_lista(no** l) {
    no* p;
    while (*l) {
        p = *l;
        *l = (*l)->prox;
        free(p);
    }
    *l = NULL;
}

void substituir(no** l, int i, int j){
    no* nova_lista = NULL;

    if (j >= i) {
        no* p = *l;
        while (p != NULL && p->indice < i) {
            p = p->prox;
        }
        while (p != NULL && p->indice <= j) {
            inserir_no_fim(&nova_lista, p->num);
            p = p->prox;
        }
    } else {
        no* p = *l;
        while (p != NULL) {
            if (p->indice >= j && p->indice <= i) {
                inserir_no_inicio(&nova_lista, p->num);  
            }
            p = p->prox;
        }
    }

    free_lista(l);
    *l = nova_lista;
    atualizar_indices(*l);  
}

void adicionar(no** l, int i, int j, int k) {
    no* sublista = copiar_intervalo(*l, i, j);
    if (!sublista) return;

    no* prev = NULL;
    no* p    = *l;
    while (p && p->indice != k) {
        prev = p;
        p    = p->prox;
    }

    if (p) {
        if (!prev) {
            no* ultimo = sublista;
            while (ultimo->prox) ultimo = ultimo->prox;
            ultimo->prox = *l;
            *l = sublista;
        } else {
            prev->prox = sublista;
            no* ultimo = sublista;
            while (ultimo->prox) ultimo = ultimo->prox;
            ultimo->prox = p;
        }
    }
    else {
        if (!*l) {
            *l = sublista;
        } else {
            no* tail = *l;
            while (tail->prox) tail = tail->prox;
            tail->prox = sublista;
        }
    }

    atualizar_indices(*l);
}

int main() {
    no* lista = NULL;
    char c;
    int num;

    while (scanf(" %c", &c) == 1 && c != '{');

    while (scanf(" %d%c", &num, &c) == 2) {
        if (!inserir_no_fim(&lista, num)) {
            free_lista(&lista);
            return 1;
        }
        if (c == '}') break;
    }

    char operacao;
    int i, j, k;

    while (1) {
        if (scanf(" %c", &operacao) != 1) break;
        if (operacao == 'f') break;

        if (operacao == '+') {
            if (scanf(" A[%d..%d] %d", &i, &j, &k) == 3) {
                adicionar(&lista, i, j, k);
                imprimir_lista(lista);
            }
        } else if (operacao == 's') {
            if (scanf(" A[%d..%d]", &i, &j) == 2) {
                substituir(&lista, i, j);
                imprimir_lista(lista);
            }
        }
    }
    free_lista(&lista);
    return 0;
}
