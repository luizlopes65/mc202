#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>


typedef struct no{
    int tamanho;
    char nome[11];
    int livre;
    int ind;
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

int converte_para_kb(int val, char unidade){
    if (unidade == 'K'){
        return val;
    } else if (unidade == 'M'){
        return val*1024;
    } else {
        return val*1024*1024;
    }
}

int add_no_fim(lista* L, int tam, char* nome_novo, int livre){
    no* novo = malloc(sizeof(no));
    if (novo == NULL) return 0;

    novo->tamanho = tam;
    strncpy(novo->nome, nome_novo, 10);
    novo->nome[10] = '\0';
    novo->livre = livre;
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


int add_no_inicio(lista* list, int tam, char* nome_novo, int livre){

    no* novo = malloc(sizeof(no));

    if (novo == NULL) return 0;

    novo->tamanho = tam;
    strncpy(novo->nome, nome_novo, 10);
    novo->nome[10] = '\0';
    novo->livre = livre;

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


lista* criar_lista(){
    lista* L = malloc(sizeof(lista));
    if (L == NULL) return NULL;
    L->size = 0;
    L->head = NULL;

    return L;
}

void imprimir_distribuicao(no* head, int capacidade_kb) {
    int fatia_kb = capacidade_kb / 8;

    for (int i = 0; i < 8; i++) {
        int slice_start = i * fatia_kb;
        int slice_end   = slice_start + fatia_kb;
        int ocupado = 0;

    int pos = 0;
    no* p = head;

    while (p != NULL) {
        if (p->livre == 0) {
            int bloco_start = pos;
            int bloco_end   = pos + p->tamanho;

            if (bloco_end > slice_start) {
                if (bloco_start < slice_end) {
                    int ini, fim;

                    if (bloco_start >= slice_start) {
                        ini = bloco_start;
                    } else {
                        ini = slice_start;
                    }

                    if (bloco_end <= slice_end) {
                        fim = bloco_end;
                    } else {
                        fim = slice_end;
                    }

                    if (fim > ini) {
                        ocupado += (fim - ini);
                    }
                }
            }
        }
        pos += p->tamanho;
        p = p->prox;
        }
            int mult = ocupado * 100;
            if      (mult <= 25 * fatia_kb) printf("[ ]");
            else if (mult <= 75 * fatia_kb) printf("[-]");
            else                            printf("[#]");      
        }
        printf("\n");
    }

void otimizar(lista* L){
    no* pp = NULL;
    no* p = L->head;
    int qtde = 0;

    while (p != NULL) {
        if (p->livre) {
            qtde += p->tamanho;

            no* temp = p;
            if (pp == NULL) {
                L->head = p->prox;
                p = L->head;
            } else {
                pp->prox = p->prox;
                p = pp->prox;
            }
            free(temp);
        } else {
            pp = p;
            p = p->prox;
        }
    }
    if (qtde > 0) {
        add_no_fim(L, qtde, "", 1);
    }

}

int inserir(lista* L, int tam, char* nome_novo, int prim_vez) {
    no* p = L->head;
    no* menor = NULL;

    while (p != NULL) {
        if (p->livre && p->tamanho >= tam) {
            if (menor == NULL || 
                p->tamanho < menor->tamanho) {
                menor = p;
                break;
            }
        }
        p = p->prox;
    }
    if (menor != NULL) {

        if (menor->tamanho == tam) {
            menor->livre = 0;
            strcpy(menor->nome, nome_novo);
        }
        else {

            int sobra = menor->tamanho - tam;

            menor->tamanho = tam;
            menor->livre  = 0;
            strcpy(menor->nome, nome_novo);

            if (sobra > 0) {
                no* resto = malloc(sizeof(no));
                resto->tamanho = sobra;
                resto->livre   = 1;
                resto->nome[0] = '\0';
                resto->prox    = menor->prox;
                menor->prox    = resto;
            }
        }
        atualizar_indices(L);
        return 1;
    }

    if (prim_vez) {
        otimizar(L);
        return inserir(L, tam, nome_novo, 0);
    }
    printf("ERRO: disco cheio\n");
    return 0;
}

int remover(lista* L, const char* nome) {
    no* p = L->head;

    while (p != NULL) {
        if (!p->livre && strcmp(p->nome, nome) == 0) {
            p->livre = 1;
            p->nome[0] = '\0';

            atualizar_indices(L);
            return 1;
        }
        p  = p->prox;
    }
    return 0;
}

void imprimir_lista(no* head) {
    no* p = head;

    while (p != NULL) {
        if (p->livre)
            printf("[livre: %dKb]", p->tamanho);
        else
            printf("[%s: %dKb]", p->nome, p->tamanho);

        if (p->prox != NULL)
            printf(" -> ");
        p = p->prox;
    }

    printf("\n");
}

int main(){
    while (1) {
        int num_op;
        int tam_inicial;
        char uni;

        scanf("%d", &num_op);
        if (num_op == 0) break;

        lista* disco = criar_lista();

        scanf("%d%cb", &tam_inicial, &uni);
        tam_inicial = converte_para_kb(tam_inicial, uni);
        add_no_inicio(disco, tam_inicial, "", 1);

        char comando[10];
        char nome[11];
        char uni_cmd;
        int tam;
        int deu_certo = 1;
        for (int i=0; i < num_op; i++) {
            scanf("%s", comando);

            if (strcmp(comando, "inserir") == 0) {
                scanf("%s %d%cb", nome, &tam, &uni_cmd);
                int tam_no = converte_para_kb(tam, uni_cmd);
                deu_certo = inserir(disco, tam_no, nome, 1);
                if (!deu_certo) {
                    for (int j = i + 1; j < num_op; j++) {
                        scanf("%s", comando);
                        if (strcmp(comando, "inserir") == 0) {
                            scanf("%*s %*d%*cb");
                        } else if (strcmp(comando, "remover") == 0) {
                            scanf("%*s");
                        }
                    }
                    break;
                }
            }
            else if (strcmp(comando, "remover") == 0) {
                scanf("%s", nome);
                remover(disco, nome);
            } else {
                otimizar(disco);
            }
        }
        if (deu_certo == 1) imprimir_distribuicao(disco->head, tam_inicial);
        free_lista(disco);
        
    }
    return 0;
}