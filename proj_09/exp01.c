#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

typedef struct no{
    int val;
    int cont;
    struct no* prox;
} no;

struct lista{
    no* head;
    int size;
};

typedef struct lista lista;

void free_lista(lista* L){
    while (L->head != NULL){
        no* temp = L->head;
        L->head = temp->prox;
        free(temp);
        
    }
}

int add_no_fim(lista* L, int val, int cont){
    no* novo = malloc(sizeof(no));
    if (novo == NULL) return 0;

    novo->val = val;
    novo->cont = cont;
    novo->prox = NULL;

    if (L->head == NULL){
        L->head = novo;
        novo->prox = NULL;
    } else {
        no* p = L->head;
        while (p->prox != NULL) p = p->prox;
        p->prox = novo;
    }

    return 1;
}

int add_no_inicio(lista* list, int val, int cont){

    no* novo = malloc(sizeof(no));

    if (novo == NULL) return 0;

    novo->val = val;
    novo->cont = cont;
    novo->prox = NULL;

    if (list->head == NULL){
        list->head = novo;
   } else{
        novo->prox = list->head;
        list->head = novo;
   }

   return 1;
}


lista* criar_lista(){
    lista* L = malloc(sizeof(lista));
    if (L == NULL) return NULL;
    L->size = 0;
    L->head = NULL;

    return L;
}

void imprimir_lista(no* l){
    no* p = l;
    printf("A: { ");
    while(p != NULL){
        printf("%d", p->val);
        if(p->prox != NULL)
            printf(", ");
        p = p->prox;
    };
    printf(" }\n");
}

int executar_busca_seq(lista* L, int chave){
    no* p = L->head;
    int contador = 0;
    int found = 0;

    while (p != NULL){
        contador++; 
        if (p->val == chave){
            found = 1;
            break;
        }
        p = p->prox;
    }

    if (found == 0){
        add_no_fim(L, chave, 0);
    }

    return contador;
}

int executar_busca_mtf(lista* L, int chave){
    no* p = L->head;
    no* ant = NULL;
    int contador = 0;

    while (p != NULL){
        contador++;

        if (p->val == chave){
            if (ant != NULL){
                ant->prox = p->prox;
                p->prox = L->head;
                L->head = p;
            }
            return contador;
        }

        ant = p;
        p = p->prox;
    }

    add_no_inicio(L, chave, 0);
    return contador;
}


int executar_busca_transp(lista* L, int chave){
    no* p = L->head;
    no* ant = NULL;
    int contador = 0;

    while (p != NULL){
        contador++;

        if (p->val == chave){
            if (ant != NULL){
                int temp = ant->val;
                ant->val = p->val;
                p->val = temp;
            }
            return contador;
        }

        ant = p;
        p = p->prox;
    }

    add_no_inicio(L, chave, 0);
    return contador;
}


int executar_busca_count(lista* L, int chave){
    no* p = L->head;
    no* ant = NULL;
    int contador = 0;

    // 1) tenta encontrar
    while (p != NULL){
        contador++;
        if (p->val == chave){
            p->cont++;

            if (ant == NULL) {
                L->head = p->prox;
            } else {
                ant->prox = p->prox;
            }

            no* atual = L->head;
            no* anterior = NULL;
            while (atual != NULL && atual->cont > p->cont) {
                anterior = atual;
                atual    = atual->prox;
            }
            if (anterior == NULL) {
                p->prox   = L->head;
                L->head    = p;
            } else {
                p->prox        = atual;
                anterior->prox = p;
            }
            return contador;
        }
        ant = p;
        p   = p->prox;
    }

    no* novo = malloc(sizeof(no));
    novo->val  = chave;
    novo->cont = 1;

    no* atual = L->head;
    no* anterior = NULL;
    while (atual != NULL && atual->cont > 1) {
        anterior = atual;
        atual    = atual->prox;
    }
    if (anterior == NULL) {
        novo->prox = L->head;
        L->head    = novo;
    } else {
        novo->prox        = atual;
        anterior->prox    = novo;
    }

    return contador;
}

int main(){
    int lim_sup;
    int num_chaves;
    scanf("%d", &lim_sup);
    scanf("%d", &num_chaves);

    lista *Lseq = criar_lista();
    lista *Lmtf = criar_lista();
    lista *Ltransp = criar_lista();
    lista *Lcount = criar_lista();

    for (int k = 1; k <= lim_sup; k++) {
        add_no_fim(Lseq, k, 0);
        add_no_fim(Lmtf, k, 0);
        add_no_fim(Ltransp, k, 0);
        add_no_fim(Lcount, k, 0);
    }

    int cont_seq = 0;
    int cont_mtf = 0;
    int cont_transp = 0;
    int cont_count = 0;

    for (int i=0; i<num_chaves; i++){
        int chave;
        scanf("%d", &chave);
        cont_seq += executar_busca_seq(Lseq, chave);
        cont_mtf += executar_busca_mtf(Lmtf, chave);
        cont_transp += executar_busca_transp(Ltransp, chave);
        cont_count += executar_busca_count(Lcount, chave);
        
    }

    printf("Sequencial: %d\n", cont_seq);
    printf("MTF: %d\n", cont_mtf);
    printf("Transpose: %d\n", cont_transp);
    printf("Count: %d\n", cont_count);
}