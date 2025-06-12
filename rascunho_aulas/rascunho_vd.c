#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

struct vetor_dim {
    int* data;
    int cap;
    int size;
};

typedef struct vetor_dim vetor_dim;


vetor_dim* criar_vetor(int n){
    vetor_dim* vetor_criado = malloc(sizeof(vetor_dim));
    if (!vetor_criado) return 0;
    int* vetor = calloc(n, sizeof(int));
    if (!vetor) {
        free(vetor_criado);
        return 0;
    }
    vetor_criado->cap = n;
    vetor_criado->data = vetor;
    vetor_criado->size = 0;

    return vetor_criado;
}

int inserir_no_inicio(vetor_dim* V, int val) {
    if (V->cap == V->size) {
        int nova_cap = V->cap * 2;
        int* novo_data = calloc(nova_cap, sizeof(int));
        if (novo_data == NULL) {
            return 0;  
        }
        for (int i = 0; i < V->size; i++) {
            novo_data[i] = V->data[i];
        }
        free(V->data);
        V->cap = nova_cap;
        V->data = novo_data;
    }


    for (int k = V->size - 1; k >= 0; k--) {
        V->data[k + 1] = V->data[k];
    }

    V->data[0] = val;
    V->size += 1;
    return 1;
}

void imprimir_vetor(vetor_dim* V){
    for(int i = 0; i < V->cap; i++){
        printf("%d ", V->data[i]); 
    }
    printf("\n");
}

int inserir_no_fim(vetor_dim* V, int val){
    if (V->cap == V->size){
        int nova_cap = 2 * V->cap;
        
        int* nova_data = calloc(nova_cap, sizeof(int));
        if(!nova_data) return 0;

        for (int i=0; i < V->size; i++){
            nova_data[i] = V->data[i];
        }

        free(V->data);
        V->cap = nova_cap;
        V->data = nova_data;
    }

    V->data[V->size] = val;
    V->size++;

    return 1;
}

int remover_do_fim(vetor_dim* V){
    if (V->size == 0) return 0;

    V->data[V->size - 1] = 0;
    V->size--;

    if (V->cap > V->size * 2.5 && V->cap > 1) {
        int nova_cap = V->cap / 2;
        if (nova_cap < 1) nova_cap = 1;

        int* nova_data = calloc(nova_cap, sizeof(int));
        if (!nova_data) return 0;

        for (int i = 0; i < V->size; i++) {
            nova_data[i] = V->data[i];
        }

        free(V->data);
        V->cap = nova_cap;
        V->data = nova_data;
    }
    return 1;
}

int remover_do_inicio(vetor_dim* V){
    if (V->size == 0) return 0;


    for (int i = 1; i < V->size; i++){
        V->data[i - 1] = V->data[i];
    }
    V->data[V->size - 1] = 0;
    V->size--;

    if (V->cap > V->size * 2.5 && V->cap > 1) {
        int nova_cap = V->cap / 2;
        if (nova_cap < 1) nova_cap = 1;

        int* nova_data = calloc(nova_cap, sizeof(int));
        if (!nova_data) return 0;

        for (int i = 0; i < V->size; i++) {
            nova_data[i] = V->data[i];
        }

        free(V->data);
        V->cap = nova_cap;
        V->data = nova_data;
    }

    
    return 1;
}

int main(){
    int cap;
    scanf("%d", &cap);
    vetor_dim* vetor = criar_vetor(cap);
    while (1){
        int valor;
        scanf("%d", &valor);
        if (valor < 0) break;
        inserir_no_inicio(vetor, valor);
        inserir_no_fim(vetor, valor + 1);
        imprimir_vetor(vetor);
        remover_do_fim(vetor);
        imprimir_vetor(vetor);
    }
    return 1;
}