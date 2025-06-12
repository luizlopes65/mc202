#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

typedef struct node {
    int val;
    int fb;
    int altura;
    int timestamp;
    struct node* left;
    struct node* right;
    struct node* parent;
} no;

typedef struct arvore {
    no* raiz;
    int timestamp;
} arvore;

arvore* criar_arvore(){
    arvore* T = malloc(sizeof(arvore));
    if (!T) return NULL;
    T->raiz = NULL;
    T->timestamp = -1;
    return T;
}

int max(int a, int b){
    if (a > b) return a;
    else return b;
}

int altura(no* p) {
    if (p == NULL) 
        return -1;   
    return 1 + max(altura(p->left), altura(p->right));
}

int fb_violado(no* r) {
    if (!r) return 0;
    if (r->fb < -2 || r->fb > 2) return 1;
    return fb_violado(r->left) || fb_violado(r->right);
}

no* criar_no(int k, int ts){
    no* p = malloc(sizeof(no));
    if (!p) return NULL;
    p->val = k;
    p->fb = 0;
    p->altura = 0;
    p->timestamp = ts;
    p->left = NULL; p->right = NULL; p->parent = NULL; 
    return p;
}

void imprimir_in_order(no *r) {
    if (!r) return;
    imprimir_in_order(r->left);
    printf("%d ", r->val);
    imprimir_in_order(r->right);
}

void imprimir(arvore *T) {
    if (!T || !T->raiz) {
        printf("arvore vazia\n");
    } else {
        printf("em-ordem: ");
        imprimir_in_order(T->raiz);
        printf("\n");
    }
}

no* buscar_no(int k, arvore* T){
    no* u = T->raiz;
    while (u != NULL && u->val != k){
        if (k < u->val) u = u->left;
        else u = u->right;
    }
    return u;
}


void inserir(int k, arvore* T){
    no* z = criar_no(k, T->timestamp);
    if (!z){
        printf("memoria insuficiente\n");
        return;
    }

    no* u = T->raiz;
    no* p = NULL;

    while(u != NULL){
        p = u;
        if (k < u->val) u = u->left;
        else u = u->right;
    }

    if (p == NULL) T->raiz = z;
    else if (k < p->val){
        p->left = z;
        z->parent = p;
    }
    else{ 
        p->right = z;
        z->parent = p;
    }

    no* q = z;
    int violou = 0;
    while (q) {
        q->altura = 1 + max(altura(q->left), altura(q->right));
        q->fb = altura(q->left) - altura(q->right);
        if (q->fb < -2 || q->fb > 2) {
            violou = 1;
            break;
        }
        q = q->parent;
    }
    if (violou) {
        if (z->parent == NULL) {
            T->raiz = NULL;
        } else if (z->parent->left == z) {
            z->parent->left = NULL;
        } else {
            z->parent->right = NULL;
        }
        free(z);
    }
}

void destruir_nos(no *r) {
    if (!r) return;
    destruir_nos(r->left);
    destruir_nos(r->right);
    free(r);
}

void destruir_arvore(arvore *T) {
    if (!T) return;
    destruir_nos(T->raiz);
    T->raiz = NULL;
}
void inserir_com_timestamp(int k, int timestamp, arvore* T) {
    no* z = malloc(sizeof(no));
    z->val = k;
    z->timestamp = timestamp;
    z->left = z->right = z->parent = NULL;
    z->altura = 0;
    z->fb = 0;

    no* u = T->raiz;
    no* p = NULL;

    while(u != NULL){
        p = u;
        if (k < u->val) u = u->left;
        else u = u->right;
    }

    if (p == NULL) T->raiz = z;
    else if (k < p->val){
        p->left = z;
        z->parent = p;
    }
    else{ 
        p->right = z;
        z->parent = p;
    }

    no* q = z;
    while (q) {
        q->altura = 1 + max(altura(q->left), altura(q->right));
        q->fb = altura(q->left) - altura(q->right);
        q = q->parent;
    }
}

no* minimo(no* u){
    if (u ==NULL) return NULL;
    while (u->left != NULL) u = u->left;
    return u;
}

no* sucessor(no* p){
    if (p->right != NULL){
        return minimo(p->right);
    } else{
        no* u = p->parent;
        while (u != NULL && p == u->right){
            p = u;
            u = u->parent;
        }
        return u;
    }
}

void troca(arvore* T, no* u, no* v){
    if (u->parent == NULL) T->raiz = v;
    else if (u == u->parent->left){
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }

    if (v != NULL) v->parent = u->parent;
}

void remover(int k, arvore* T) {
    no* z = buscar_no(k, T);
    if (!z) return;

    int val_removido = z->val;
    int ts_removido = z->timestamp;

    no* atualizacao_partida = z->parent;

    if (z->left == NULL) {
        troca(T, z, z->right);
    } else if (z->right == NULL) {
        troca(T, z, z->left);
    } else {
        no* s = sucessor(z);
        if (s->parent != z) {
            troca(T, s, s->right);
            s->right = z->right;
            if (s->right) s->right->parent = s;
        }
        troca(T, z, s);
        s->left = z->left;
        if (s->left) s->left->parent = s;
        atualizacao_partida = s;
    }

    free(z); 

    no* q = atualizacao_partida;
    while (q) {
        q->altura = 1 + max(altura(q->left), altura(q->right));
        q->fb = altura(q->left) - altura(q->right);
        q = q->parent;
    }

    if (fb_violado(T->raiz)) {
        inserir_com_timestamp(val_removido, ts_removido, T);
    }
}

int contar_nos(no *r) {
    if (!r) return 0;
    return 1
         + contar_nos(r->left)
         + contar_nos(r->right);
}

int contar_folhas(no *r) {
    if (!r) return 0;
    if (!r->left && !r->right) 
        return 1;
    return contar_folhas(r->left) + contar_folhas(r->right);
}

int altura_arvore(no *r) {
    if (!r) return -1;
    int hl = altura_arvore(r->left);
    int hr = altura_arvore(r->right);
    return 1 + (hl > hr ? hl : hr);
}
int main(void) {
    arvore *T = NULL;
    char cmd[16];
    int k;

    while (scanf("%15s", cmd) == 1) {
        if (strcmp(cmd, "criar") == 0) {
            if (T) {
                destruir_arvore(T);
                free(T);
            }
            T = criar_arvore();

        } else if (strcmp(cmd, "inserir") == 0) {
            scanf("%d", &k);
            if (!T) T = criar_arvore();
            T->timestamp++;
            if (!buscar_no(k, T))
                inserir(k, T);

        } else if (strcmp(cmd, "remover") == 0) {
            scanf("%d", &k);
            if (T) {
                T->timestamp++;
                remover(k, T);
            }

        } else if (strcmp(cmd, "buscar") == 0) {
            scanf("%d", &k);
            if (T) {
                no *p = buscar_no(k, T);
                if (p)
                    printf("chave %d: inserida no tempo %d\n", k, p->timestamp);
                else
                    printf("nao ha chave %d\n", k);
            } else {
                printf("nao ha chave %d\n", k);
            }

        } else if (strcmp(cmd, "imprimir") == 0) {
            if (!T || !T->raiz)
                printf("arvore vazia\n");
            else {
                imprimir(T);
            }

        } else if (strcmp(cmd, "info") == 0) {
            if (!T || !T->raiz) {
                printf("nos: 0, folhas: 0, altura: 0\n");
            } else {
                printf("nos: %d, folhas: %d, altura: %d\n",
                    contar_nos(T->raiz),
                    contar_folhas(T->raiz),
                    altura_arvore(T->raiz)
                );
            }

        } else {
            break;
        }
    }

    if (T) {
        destruir_arvore(T);
        free(T);
    }
    return 0;
}