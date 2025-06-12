#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char** data;
    int cap;
    int size;
} string_vector;

// Cria um vetor dinâmico de strings com capacidade inicial 'n'
string_vector* create_string_vector(int n) {
    string_vector* vec = malloc(sizeof(string_vector));
    if (!vec) return NULL;
    vec->data = calloc(n, sizeof(char*));
    if (!vec->data) {
        free(vec);
        return NULL;
    }
    vec->cap = n;
    vec->size = 0;
    return vec;
}

// Adiciona uma string ao vetor dinâmico, realocando se necessário
int add_string(string_vector* vec, const char* str) {
    if (vec->size == vec->cap) {
        int new_cap = vec->cap * 2;
        char** new_data = calloc(new_cap, sizeof(char*));
        if (!new_data) return 0;
        for (int i = 0; i < vec->size; i++) {
            new_data[i] = vec->data[i];
        }
        free(vec->data);
        vec->data = new_data;
        vec->cap = new_cap;
    }
    vec->data[vec->size] = strdup(str);  // duplica a string para armazenar
    if (!vec->data[vec->size]) return 0;
    vec->size++;
    return 1;
}

// Libera toda a memória alocada pelo vetor dinâmico de strings
void free_string_vector(string_vector* vec) {
    if (vec) {
        for (int i = 0; i < vec->size; i++) {
            free(vec->data[i]);
        }
        free(vec->data);
        free(vec);
    }
}

int main(){
    int initial_cap = 2;
    string_vector* vec = create_string_vector(initial_cap);
    if (!vec) {
        fprintf(stderr, "Erro na alocação de memória para o vetor\n");
        return 1;
    }
    
    char buffer[1024];
    printf("Digite linhas de texto (pressione ENTER em uma linha vazia para sair):\n");
    while (1) {
        if (!fgets(buffer, sizeof(buffer), stdin))
            break;
        buffer[strcspn(buffer, "\n")] = '\0';  // remove a nova linha
        if (buffer[0] == '\0')  // linha vazia, encerra a leitura
            break;
        if (!add_string(vec, buffer)) {
            fprintf(stderr, "Erro ao adicionar a string\n");
            free_string_vector(vec);
            return 1;
        }
    }
    
    printf("\nLinhas digitadas em ordem reversa:\n");
    for (int i = vec->size - 1; i >= 0; i--) {
        printf("%s\n", vec->data[i]);
    }
    
    free_string_vector(vec);
    return 0;
}