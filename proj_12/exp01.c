#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <math.h>

typedef struct {
  char* cadeia;
  int timestamp;
  int ocupado;
} entrada;

typedef struct{
    entrada* pares;
    int tamanho;
    int timestamp_tab;
} tabela;


unsigned long djb2(char *str) {
  unsigned long hash = 5381;
  int c;
  
  while ((c = *str++))
    hash = ((hash << 5) + hash) ^ c; 
  
  return hash;
}

int eh_primo(int n) {
    if (n <= 1) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

int proximo_primo(int n) {
    while (!eh_primo(n)) {
        n++;
    }
    return n;
}

tabela* criar_tabela(int n) {
    tabela* tab = malloc(sizeof(tabela));
    if (!tab) return NULL;

    int capacidade_inicial = ceil(n / 0.7);
    int capacidade = proximo_primo(capacidade_inicial);

    tab->pares = calloc(capacidade, sizeof(entrada));
    if (!tab->pares) {
        free(tab);
        return NULL;
    }

    tab->tamanho = capacidade;
    tab->timestamp_tab = 0;

    return tab;
}

void remover(char* cadeia, tabela* tab) {
    unsigned long hash = djb2(cadeia);
    int h1 = hash % tab->tamanho;
    int h2 = 1 + (hash % (tab->tamanho - 1));

    for (int i = 0; i < tab->tamanho; i++) {
        int idx = (h1 + i * h2) % tab->tamanho;
        entrada* par = &tab->pares[idx];

        if (par->ocupado == 0) {
            return;
        }
        if (par->ocupado == 1 && strcmp(cadeia, par->cadeia) == 0) {
            free(par->cadeia);
            par->cadeia = NULL;
            par->ocupado = -1; 
            par->timestamp = 0;
            return; 
        }
    }
}

int busca(char* cadeia, tabela* tab, int print) {
    unsigned long hash = djb2(cadeia);
    int h1 = hash % tab->tamanho;
    int h2 = 1 + (hash % (tab->tamanho - 1));

    for (int i = 0; i < tab->tamanho; i++) {
        int idx = (h1 + i * h2) % tab->tamanho;
        entrada* par = &tab->pares[idx];

        if (par->ocupado == 0) {
            if (print) 
                printf("[%s] nao esta na tabela\n", cadeia);
            return 0;
        }
        if (par->ocupado == 1 && strcmp(cadeia, par->cadeia) == 0) {
            if (print) 
                printf("[%s] esta na tabela, timestamp %d\n", cadeia, par->timestamp);
            return 1;
        }
    }
    if (print) 
        printf("[%s] nao esta na tabela\n", cadeia);
    return 0;
}

char* strdup(const char* s) {
    char* d = malloc(strlen(s) + 1);
    if (d) strcpy(d, s);
    return d;
}

void inserir(char* cadeia, tabela* tab) {
    int b = busca(cadeia, tab, 0);
    if (b){
        return;
    }

    unsigned long hash = djb2(cadeia);
    int h1 = hash % tab->tamanho;
    int h2 = 1 + (hash % (tab->tamanho - 1));

    for (int i = 0; i < tab->tamanho; i++) {
        int idx = (h1 + i * h2) % tab->tamanho;
        entrada* par = &tab->pares[idx];

        if (par->ocupado == 0 || par->ocupado == -1) {
            par->cadeia = strdup((char*)cadeia);
            par->ocupado = 1;
            par->timestamp = tab->timestamp_tab;
            tab->timestamp_tab = tab->timestamp_tab + 1;
            return;
        }
    }
}

void liberar_tabela(tabela* tab) {
    if (!tab) return;
    for (int i = 0; i < tab->tamanho; i++) {
        if (tab->pares[i].ocupado) {
            free(tab->pares[i].cadeia);
        }
    }
    free(tab->pares);
    free(tab);
}

int main() {
    tabela* tab = NULL;
    char comando;
    int n;
    char cadeia[251];
    
    while (1) {
        if (scanf(" %c", &comando) != 1) break;

        if (comando == 'f') {
            if (tab) {
                liberar_tabela(tab);
                tab = NULL;
            }
            break;
        }
        else if (comando == 'c') {
            if (scanf(" %d", &n) != 1) {
                int ch;
                while ((ch = getchar())!='\n' && ch!=EOF);
                continue;
            }
            int ch;
            while ((ch = getchar())!='\n' && ch!=EOF);

            if (tab) {
                liberar_tabela(tab);
                tab = NULL;
            }
            tab = criar_tabela(n);
        }
        else if (comando == 'i' || comando == 'r' || comando == 'b') {
            char sep;
            if (scanf("%c", &sep) != 1 || sep != ' ') {
                int ch;
                while ((ch = getchar())!='\n' && ch!=EOF);
                continue;
            }
            if (scanf("%250[^\n]", cadeia) != 1) {
                int ch;
                while ((ch = getchar())!='\n' && ch!=EOF);
                continue;
            }
            scanf("%*c");

            if (comando == 'i') {
                inserir(cadeia, tab);
            }
            else if (comando == 'r') {
                remover(cadeia, tab);
            }
            else { 
                busca(cadeia, tab, 1);
            
            }
        }

    }
    return 0;
}