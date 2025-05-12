#include <stdio.h>
#include <string.h>

#define EMPTY '\0'

int main () {
    int n, k;

    while (scanf(" %d", &n) == 1) { 
        char cadeia[n + 1]; 
        scanf(" %s", cadeia); 
        scanf(" %d", &k); 

        typedef struct {
            int freq;
            char subcadeia[k + 1];
        } Kmer;

        Kmer lista_de_kmers[n - k + 1];

        for (int i = 0; i < n - k + 1; i++) {
            lista_de_kmers[i].subcadeia[0] = EMPTY;
            lista_de_kmers[i].freq = 0;
        }

        int num_kmers = 0; 

        for (int j = 0; j <= n - k; j++) { 
            char k_mer[k + 1]; 

            for (int i = 0; i < k; i++) { 
                k_mer[i] = cadeia[j + i];
            }
            k_mer[k] = '\0'; 

            int encontrado = 0;

            for (int i = 0; i < num_kmers; i++) {
                if (strcmp(lista_de_kmers[i].subcadeia, k_mer) == 0) {
                    lista_de_kmers[i].freq++;
                    encontrado = 1;
                    break;
                }
            }

            if (!encontrado) {
                strcpy(lista_de_kmers[num_kmers].subcadeia, k_mer);
                lista_de_kmers[num_kmers].freq = 1;
                num_kmers++;
            }
        }

        int maior_freq = -1;
        for (int i = 0; i < num_kmers; i++) {
            if (lista_de_kmers[i].freq > maior_freq) {
                maior_freq = lista_de_kmers[i].freq;
            }
        }

        Kmer kmers_mais_frequentes[num_kmers];

        int count = 0;

        for (int i = 0; i < num_kmers; i++) {
            if (lista_de_kmers[i].freq == maior_freq) {
                kmers_mais_frequentes[count++] = lista_de_kmers[i];
            }
        }

        for (int i = 0; i < count - 1; i++) {
            for (int j = 0; j < count - i - 1; j++) {
                if (strcmp(kmers_mais_frequentes[j].subcadeia, kmers_mais_frequentes[j + 1].subcadeia) > 0) {
                    Kmer temp;  
                    temp = kmers_mais_frequentes[j];  
                    kmers_mais_frequentes[j] = kmers_mais_frequentes[j + 1];
                    kmers_mais_frequentes[j + 1] = temp;
        }
    }
}

        printf("%d %d:", k, maior_freq);
        for (int i = 0; i < count; i++) {
            printf(" %s", kmers_mais_frequentes[i].subcadeia);
        }
        printf(" \n");
    }

    return 0;
}